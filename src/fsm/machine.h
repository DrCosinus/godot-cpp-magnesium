#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include "magnesium/stl_map.hpp"

namespace magnesium::fsm
{
	class machine : public godot::RefCounted
	{
		GDCLASS(machine, godot::RefCounted)
		using STATE_BASE_TYPE = godot::Script; // This is the base type for states. It can be changed to an interface or a custom class if needed.

	protected:
		static void _bind_methods();
		void allow_transitions(bool enable)
		{
			transitions_allowed = enable;
		}

	public:
		machine() = default;
		~machine() = default;

		void update(godot::Object* p_context, float delta);

		void travel_to(godot::Object* p_context, STATE_BASE_TYPE* new_state);
		godot::Variant call_on_state(const godot::Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error);

		STATE_BASE_TYPE* get_current_state(godot::Object* p_context) const
		{
			auto it = context_state_map.find(p_context);
			return it != context_state_map.end() ? it->second : nullptr;
		}
		godot::StringName get_state_name(STATE_BASE_TYPE* state) const;
		godot::StringName get_current_state_name(godot::Object* p_context) const;
		void set_current_state(godot::Object* p_context, STATE_BASE_TYPE* new_state)
		{
			context_state_map[p_context] = new_state;
		}

	private:
		StlMap<godot::Object*, STATE_BASE_TYPE*> context_state_map;
		bool transitions_allowed{ false };
	};
} //namespace magnesium::fsm