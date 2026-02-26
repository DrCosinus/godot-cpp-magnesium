#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

// #include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/core/gdvirtual.gen.inc"

#include "context.h"

// using namespace godot;

namespace magnesium::fsm
{
	// class context;

	class state : public godot::RefCounted
	{
		GDCLASS(state, godot::RefCounted);
		static const auto METHOD_FLAG_VIRTUAL = godot::METHOD_FLAG_VIRTUAL;
	protected:
		static void _bind_methods();

	public:
		~state() override = default;

		GDVIRTUAL1(enter, context*);
		GDVIRTUAL1(exit, context*);
		GDVIRTUAL2(update, context*, float);
	};
} //namespace magnesium::fsm