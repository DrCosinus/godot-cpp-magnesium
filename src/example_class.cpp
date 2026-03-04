#include "example_class.h"

// StringName SIGNAL_NAME{ "the_signal" };

void ExampleClass::_bind_methods()
{
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &ExampleClass::print_type);

	ADD_SIGNAL(MethodInfo(get_signal_name(), PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")));
}

void ExampleClass::print_type(const Variant& p_variant) const
{
	print_line(vformat("Type: %d", p_variant.get_type()));
}

// auto ExampleClass::try_get_method(Object *obj, const StringName &method_name) const -> Callable
// {
// 	if (!obj)
// 	{
// 		return Callable();
// 	}

// 	obj->has_method
// 	return obj->get(method_name);
// }
