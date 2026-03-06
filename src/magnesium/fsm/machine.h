#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
// #include "godot_cpp/classes/script.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include "magnesium/stl_map.hpp"

namespace godot
{
	class Object;
	class Script;
	class Variant;
} //namespace godot

namespace magnesium::fsm
{
	class machine : public godot::RefCounted
	{
		GDCLASS(machine, godot::RefCounted)

	protected:
		static void _bind_methods();
		void allow_transitions(bool enable)
		{
			transitions_allowed = enable;
		}

	public:
		machine() = default;
		~machine() override = default;

		void travel_to(godot::Object* context, godot::Script* new_state);
		godot::Variant call_on_state(const godot::Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error);
		void update(godot::Object* context, float delta);

		godot::Script* get_current_state(godot::Object* context) const;
		godot::StringName get_state_name(godot::Script* state) const;
		godot::StringName get_current_state_name(godot::Object* context) const;
		void set_current_state(godot::Object* context, godot::Script* new_state)
		{
			context_state_map[context] = new_state;
		}

	private:
		StlMap<godot::Object*, godot::Script*> context_state_map;
		bool transitions_allowed{ false };
	};
} //namespace magnesium::fsm