#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

namespace magnesium::fsm
{
	class state;
	class context;

	class machine : public godot::RefCounted
	{
		GDCLASS(machine, godot::RefCounted)

	protected:
		static void _bind_methods();

	public:
		machine() = default;
		~machine() = default;

		void update(context* p_context, float delta);

		void travel_to(context* p_context, state* new_state);

	private:
		state* current_state{ nullptr };
	};
} //namespace magnesium::fsm