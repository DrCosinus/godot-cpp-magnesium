#include "utils.h"

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "array_view.hpp"

using namespace godot;

namespace magnesium
{
	void utils::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("print_type", "variant"), &utils::print_type);
		{
			MethodInfo mi;
			mi.arguments.push_back(PropertyInfo(Variant::OBJECT, "p_object"));
			mi.arguments.push_back(PropertyInfo(Variant::STRING_NAME, "p_method_name"));
			// mi.arguments.push_back(PropertyInfo(Variant::STRING, "some_argument"));
			mi.name = "try_call_method";
			mi.flags = METHOD_FLAG_STATIC | METHOD_FLAG_VARARG;
			ClassDB::bind_vararg_method(mi.flags, "try_call_method", &utils::try_call_method, mi);
		}

		// ClassDB::bind_vararg_method(get_class_static(), D_METHOD("try_call_method", "obj", "method_name"), &utils::try_call_method);
	}

	void utils::print_type(const Variant& p_variant) const
	{
		print_line(vformat("Type: %d", p_variant.get_type()));
	}

	Variant utils::try_call_method(const Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error)
	{
		if (arg_count < 2)
		{
			error.error = GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS;
			error.argument = arg_count;
			error.expected = 2;
			return Variant{};
		}
		if (args[0]->get_type() != Variant::OBJECT)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 0;
			error.expected = Variant::OBJECT;
			return Variant{};
		}
		if (args[1]->get_type() != Variant::STRING_NAME)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 1;
			error.expected = Variant::STRING_NAME;
			return Variant{};
		}
		Object* obj = static_cast<Object*>(*args[0]);
		const StringName method_name = static_cast<StringName>(*args[1]);

		// obj->get_method_argument_count(method_name); // This is used to check if the method exists, and also to trigger the "invalid method" error if it doesn't.
		// however this is another lookup, and it also doesn't work for methods that have a variable number of arguments. So instead we will just call the method, and if it doesn't exist, it will return an empty Variant.
		// Fortunately, Object::callv() is safe to call on an invalid method name, it will just return an empty Variant. So we don't need to check if the method exists before calling it.
#if 0
		Array arr;
		arr.resize(arg_count - 2);
		for (int i = 2; i < arg_count; ++i)
		{
			arr[i - 2] = *args[i];
		}
		return obj ? obj->callv(method_name, arr) : Variant{};
#else
		array_view<const Variant*> arr{ args, arg_count };
		arr.skip(2);
		return obj ? obj->callv(method_name, arr.to_array()) : Variant{};
#endif
	}
} //namespace magnesium