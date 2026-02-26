#include "machine.h"
#include "context.h"
#include "state.h"

// #include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

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
			current_state->call(update_method_name, p_context, delta);
		}
	}

	void machine::travel_to(context* p_context, state* new_state)
	{
		// static StringName exit_method_name{ "exit" };
		// static StringName enter_method_name{ "enter" };
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