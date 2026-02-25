#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
// #include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/wrapped.hpp"

namespace magnesium::fsm
{
	class context;

	class state : public godot::RefCounted
	{
		GDCLASS(state, godot::RefCounted);

	protected:
		static void _bind_methods();

	public:
		~state() override = default;
		virtual void enter(context* context) = 0;
		virtual void exit(context* context) = 0;
		virtual void update(context* context, float delta) = 0;
	};
} //namespace magnesium::fsm