#pragma once

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include "godot_cpp/core/gdvirtual.gen.inc"

namespace magnesium::fsm
{
	class state : public godot::RefCounted
	{
		GDCLASS(state, godot::RefCounted);
		static const auto METHOD_FLAG_VIRTUAL = godot::METHOD_FLAG_VIRTUAL;
		static const auto METHOD_FLAG_VIRTUAL_REQUIRED = godot::METHOD_FLAG_VIRTUAL_REQUIRED;

	protected:
		static void _bind_methods();

	public:
		~state() override = default;

		godot::StringName get_class_name() const;
		GDVIRTUAL1(enter, godot::Object*);
		GDVIRTUAL1(exit, godot::Object*);
		GDVIRTUAL2_REQUIRED(update, godot::Object*, float);
	};
} //namespace magnesium::fsm