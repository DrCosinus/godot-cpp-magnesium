#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class ExampleClass : public Node
{
	GDCLASS(ExampleClass, Node)

	static const StringName get_signal_name()
	{
		static StringName signal_name{ "the_signal" };
		return signal_name;
	}

protected:
	static void _bind_methods();

public:
	ExampleClass() = default;
	~ExampleClass() override = default;

	void print_type(const Variant& p_variant) const;
	// auto try_get_method(Object *obj, const StringName &method_name) const -> Callable;
};
