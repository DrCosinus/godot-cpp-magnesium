#include "machine.h"
#include "context.h"
#include "state.h"

// #include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

StringName get_state_name(Object* obj)
{
	if (!obj)
	{
		return "None";
	}
	Script* script = static_cast<Script*>(static_cast<Object*>(obj->get_script()));
	return script ? script->get_global_name() : StringName{ obj->get_class() };
}

namespace magnesium::fsm
{
	static StringName get_changed_signal_name()
	{
		static StringName signal_name{ "changed" };
		return signal_name;
	}

	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
		ClassDB::bind_method(D_METHOD("travel_to", "context", "new_state"), &machine::travel_to);
		ClassDB::bind_method(D_METHOD("get_current_state"), &machine::get_current_state);
		// GDREGISTER_FUNCTION("get_current_state", &machine::get_current_state);

		// ADD_SIGNAL(MethodInfo(
		// 		get_changed_signal_name(),
		// 		PropertyInfo(Variant::OBJECT, "from", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "state"),
		// 		PropertyInfo(Variant::OBJECT, "p_context")));
		ADD_SIGNAL(MethodInfo(
				get_changed_signal_name(),
				PropertyInfo(Variant::OBJECT, "from", PROPERTY_HINT_TYPE_STRING, "state"),
				PropertyInfo(Variant::OBJECT, "p_context")));
		ClassDB::get_method(get_class_static(), "update");

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_state", PROPERTY_HINT_TYPE_STRING, "state"), "", "get_current_state");
	}

	void machine::update(Object* p_context, float delta)
	{
		static StringName update_method_name{ "update" };
		if (current_state)
		{
			GDVIRTUAL_CALL_PTR(current_state, update, p_context, delta);
		}
	}

	void machine::travel_to(Object* p_context, state* new_state)
	{
		// static StringName exit_method_name{ "exit" };
		// static StringName enter_method_name{ "enter" };
		print_line(vformat("Machine travel from state '%s' to state '%s'", get_state_name(current_state), get_state_name(new_state)));
		if (current_state)
		{
			GDVIRTUAL_CALL_PTR(current_state, exit, p_context);
		}

		current_state = new_state;

		if (current_state)
		{
			GDVIRTUAL_CALL_PTR(current_state, enter, p_context);
		}
		emit_signal(get_changed_signal_name(), current_state, p_context);
	}
} //namespace magnesium::fsm