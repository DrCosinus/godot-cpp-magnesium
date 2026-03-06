#include "machine.h"
#include "state.h"

#include "godot_extra/array_view.hpp"

#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

namespace magnesium::fsm
{
	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("travel_to", "context", "to_state"), &machine::travel_to);
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
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

	Script* machine::get_current_state(godot::Object* context) const
	{
		auto it = context_state_map.find(context);
		return it != context_state_map.end() ? it->second : nullptr;
	}

	void machine::travel_to(Object* context, Script* to_state)
	{
		Script* const from_state = get_current_state(context);
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
			from_state->call("exit", context);
		}
		set_current_state(context, to_state);
		if (to_state)
		{
			to_state->call("enter", context);
		}
		emit_signal("changed", from_state, to_state, context);
	}

	void machine::update(Object* context, float delta)
	{
		{
			Script* const current_state = get_current_state(context);
			if (current_state)
			{
				allow_transitions(true);
				current_state->call("orchestrate", context);
				allow_transitions(false);
			}
		}
		// state may change during the orchestrate call, so we need to get the current state again before calling update on it
		{
			Script* const current_state = get_current_state(context);
			if (current_state)
			{
				current_state->call("update", context, delta);
			}
		}
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
		godot_extra::array_view<const Variant*> arr{ args, arg_count };
		arr.skip(2);

		Script* const current_state = get_current_state(context);
		if (current_state)
		{
			return current_state->callv(method_name, arr.to_array());
		}
		else
		{
			return Variant{};
		}
	}

	// the state name is determined by the "state_name" property of the state script if it exists,
	// otherwise it falls back to the global name of the script or the string including the hex representation of the script pointer if its global name is empty.
	StringName machine::get_state_name(Script* state) const
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

	StringName machine::get_current_state_name(Object* context) const
	{
		Script* const current_state = get_current_state(context);
		return get_state_name(current_state);
	}

} //namespace magnesium::fsm