#include "machine.h"
#include "context.h"
#include "state.h"
#include "godot_cpp_ex.hpp"

// #include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/variant/string_name.hpp"

using namespace godot;

namespace magnesium::fsm
{
	StringName machine::get_state_name(const STATE_BASE_TYPE* state) const
	{
		if (!state)
		{
			return "None";
		}

		return state ? state->get_global_name() : StringName{ state->get_class() };
	}

	void machine::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &machine::update);
		ClassDB::bind_method(D_METHOD("travel_to", "context", "to_state"), &machine::travel_to);
		ClassDB::bind_method(D_METHOD("get_current_state", "context"), &machine::get_current_state);
		ClassDB::bind_method(D_METHOD("get_current_state_name", "context"), &machine::get_current_state_name);
		ClassDB::bind_method(D_METHOD("get_state_name", "state"), &machine::get_state_name);

		ADD_SIGNAL(MethodInfo(
				"changed",
				PropertyInfo(Variant::OBJECT, "from", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"),
				PropertyInfo(Variant::OBJECT, "to", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"),
				PropertyInfo(Variant::OBJECT, "context")));
		ClassDB::get_method(get_class_static(), "update");

		//  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_state", PROPERTY_HINT_TYPE_STRING, "state"), "", "get_current_state");
	}

	StringName machine::get_current_state_name(Object* p_context) const
	{
		const STATE_BASE_TYPE* const current_state = get_current_state(p_context);
		return get_state_name(current_state);
	}

	void machine::update(Object* p_context, float delta)
	{
		STATE_BASE_TYPE* const current_state = get_current_state(p_context);
		if (current_state)
		{
			current_state->call("update", p_context, delta);
		}
	}

	void machine::travel_to(Object* p_context, STATE_BASE_TYPE* to_state)
	{
		STATE_BASE_TYPE* const from_state = get_current_state(p_context);
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
} //namespace magnesium::fsm