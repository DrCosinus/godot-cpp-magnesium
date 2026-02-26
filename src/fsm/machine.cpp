#include "machine.h"
#include "context.h"
#include "state.h"

// #include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/classes/script.hpp"

using namespace godot;

StringName get_state_name(Object *obj)
{
	if (!obj)
	{
		return "None";
	}
	Script *script = static_cast<Script*>(static_cast<Object*>(obj->get_script()));
	return script ? script->get_global_name() : StringName{ obj->get_class() };
	// print_line(vformat("Class name: %s", obj->get_class()));
}

namespace magnesium::fsm
{
	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
		ClassDB::bind_method(D_METHOD("travel_to", "context", "new_state"), &machine::travel_to);
	}

	void machine::update(context* p_context, float delta)
	{
		static StringName update_method_name{ "update" };
		if (current_state && current_state->has_method(update_method_name))
		{
			current_state->GDVIRTUAL_CALL(update, p_context, delta);
		}
	}

	void machine::travel_to(context* p_context, state* new_state)
	{
		// static StringName exit_method_name{ "exit" };
		// static StringName enter_method_name{ "enter" };
		print_line(vformat("Machine travel from state '%s' to state '%s'", get_state_name(current_state), get_state_name(new_state)));
		if (current_state)
		{
			current_state->GDVIRTUAL_CALL(exit, p_context);
		}

		current_state = new_state;

		if (current_state)
		{
			current_state->GDVIRTUAL_CALL(enter, p_context);
		}
	}
} //namespace magnesium::fsm