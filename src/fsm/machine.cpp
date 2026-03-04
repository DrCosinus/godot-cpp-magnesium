#include "machine.h"
#include "context.h"
#include "state.h"

// #include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

namespace magnesium::fsm
{
	StringName machine::get_state_name(const godot::Variant& state)
	{
		if (!state)
		{
			return "None";
		}
		if (state.get_type() != Variant::OBJECT)
		{
			return "NotAnObject";
		}
		Object* state_obj{ state };
		Script* script = Object::cast_to<Script>(state_obj);
		// Script* script = static_cast<Script*>(static_cast<Object*>(state_obj->get_script()));
		return script ? script->get_global_name() : StringName{ state_obj->get_class() };
	}

	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
		ClassDB::bind_method(D_METHOD("travel_to", "context", "new_state"), &machine::travel_to);
		ClassDB::bind_method(D_METHOD("get_current_state", "context"), &machine::get_current_state);
		ClassDB::bind_method(D_METHOD("get_current_state_name", "context"), &machine::get_current_state_name);
		ClassDB::bind_method(D_METHOD("get_state_name", "state"), &machine::get_state_name);

		ADD_SIGNAL(MethodInfo(
				"changed",
				PropertyInfo(Variant::OBJECT, "from"),
				PropertyInfo(Variant::OBJECT, "p_context")));
		ClassDB::get_method(get_class_static(), "update");

		//  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_state", PROPERTY_HINT_TYPE_STRING, "state"), "", "get_current_state");
	}

	StringName machine::get_current_state_name(Object* p_context)
	{
		auto current_state = get_current_state(p_context);
		return get_state_name(current_state);
	}

	void machine::update(Object* p_context, float delta)
	{
		auto current_state = get_current_state(p_context);
		if (current_state)
		{
			current_state.call_static(Variant::OBJECT, "update", p_context, delta);
			// GDVIRTUAL_CALL_PTR(current_state, update, p_context, delta);
		}
	}

	void machine::travel_to(Object* p_context, const godot::Variant& new_state)
	{
		auto current_state = get_current_state(p_context);
		if (current_state == new_state)
		{
			print_line(vformat("Machine already in state '%s'", get_state_name(current_state)));
			return;
		}
		print_line(vformat("Machine travel from state '%s' to state '%s'", get_state_name(current_state), get_state_name(new_state)));
		if (current_state)
		{
			current_state.call_static(Variant::OBJECT, StringName{ "exit" }, p_context);
			// GDVIRTUAL_CALL_PTR(current_state, exit, p_context);
		}

		set_current_state(p_context, new_state);

		if (new_state)
		{
			new_state.call_static(Variant::OBJECT, StringName{ "enter" }, p_context);
			// GDVIRTUAL_CALL_PTR(new_state, enter, p_context);
		}
		emit_signal("changed", new_state, p_context);
	}
} //namespace magnesium::fsm