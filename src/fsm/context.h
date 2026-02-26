#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

namespace magnesium::fsm
{
	class context :public godot::RefCounted
	{
		GDCLASS(context, godot::RefCounted);

	protected:
		static void _bind_methods();

	public:
		context() = default;
		~context() = default;

		double value{ 0.0 };
		double get_value() const { return value; }
		void set_value(double new_value) { value = new_value; }
	};
} //namespace magnesium::fsm