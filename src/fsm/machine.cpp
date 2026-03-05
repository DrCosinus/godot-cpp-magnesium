#include "machine.h"
#include "context.h"
#include "godot_cpp_ex.hpp"
#include "state.h"
#include "magnesium/array_view.hpp"

// #include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

namespace magnesium::fsm
{
	// the state name is determined by the "state_name" property of the state script if it exists,
	// otherwise it falls back to the global name of the script or the class name if there is no global name.
	StringName machine::get_state_name(STATE_BASE_TYPE* state) const
	{
		if (!state)
		{
			return "None";
		}

		if (Variant var = state->get("state_name"); var.get_type() == Variant::STRING_NAME)
		{
			return var;
		}

		if (StringName global_name = state->get_global_name(); !global_name.is_empty())
		{
			return global_name;
		}

		return String("State<%016X>") % (Array::make(reinterpret_cast<GDExtensionInt>(state)));
	}

	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
		ClassDB::bind_method(D_METHOD("travel_to", "context", "to_state"), &machine::travel_to);
		ClassDB::bind_method(D_METHOD("get_current_state", "context"), &machine::get_current_state);
		ClassDB::bind_method(D_METHOD("get_current_state_name", "context"), &machine::get_current_state_name);
		ClassDB::bind_method(D_METHOD("get_state_name", "state"), &machine::get_state_name);
		{
			MethodInfo mi;
			mi.arguments.push_back(PropertyInfo(Variant::OBJECT, "context"));
			mi.arguments.push_back(PropertyInfo(Variant::STRING_NAME, "method_name"));
			mi.name = "call_on_state";
			mi.flags = METHOD_FLAG_STATIC | METHOD_FLAG_VARARG;
			ClassDB::bind_vararg_method(mi.flags, "call_on_state", &machine::call_on_state, mi);
		}

		ADD_SIGNAL(MethodInfo(
				"changed",
				PropertyInfo(Variant::OBJECT, "from", PROPERTY_HINT_RESOURCE_TYPE, "Script"),
				PropertyInfo(Variant::OBJECT, "to", PROPERTY_HINT_RESOURCE_TYPE, "Script"),
				PropertyInfo(Variant::OBJECT, "context")));

		//  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_state", PROPERTY_HINT_TYPE_STRING, "state"), "", "get_current_state");
	}

	StringName machine::get_current_state_name(Object* p_context) const
	{
		STATE_BASE_TYPE* const current_state = get_current_state(p_context);
		return get_state_name(current_state);
	}

	void machine::update(Object* p_context, float delta)
	{
		{
			STATE_BASE_TYPE* const current_state = get_current_state(p_context);
			if (current_state)
			{
				allow_transitions(true);
				current_state->call("orchestrate", p_context);
				allow_transitions(false);
			}
		}
		{
			STATE_BASE_TYPE* const current_state = get_current_state(p_context);
			if (current_state)
			{
				current_state->call("update", p_context, delta);
			}
		}
	}

	void machine::travel_to(Object* p_context, STATE_BASE_TYPE* to_state)
	{
		STATE_BASE_TYPE* const from_state = get_current_state(p_context);
		if (from_state && !transitions_allowed)
		{
			ERR_PRINT("Transitions are not allowed at this time. Make sure to call travel_to from within the 'orchestrate' method of the current state or there is no current state.");
			return;
		}
		if (from_state == to_state)
		{
			return;
		}
		if (from_state)
		{
			from_state->call("exit", p_context);
		}
		set_current_state(p_context, to_state);
		if (to_state)
		{
			to_state->call("enter", p_context);
		}
		emit_signal("changed", from_state, to_state, p_context);
	}

	Variant machine::call_on_state(const Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error)
	{
		if (arg_count < 2)
		{
			error.error = GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS;
			error.argument = arg_count;
			error.expected = 2;
			return Variant{};
		}
		if (args[0]->get_type() != Variant::OBJECT)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 0;
			error.expected = Variant::OBJECT;
			return Variant{};
		}
		if (args[1]->get_type() != Variant::STRING_NAME)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 1;
			error.expected = Variant::STRING_NAME;
			return Variant{};
		}
		Object* context = static_cast<Object*>(*args[0]);
		const StringName method_name = static_cast<StringName>(*args[1]);
		array_view<const Variant*> arr{ args, arg_count };
		arr.skip(2);

		STATE_BASE_TYPE* const current_state = get_current_state(context);
		if (current_state)
		{
			return current_state->callv(method_name, arr.to_array());
		}
		else
		{
			return Variant{};
		}
	}

} //namespace magnesium::fsm