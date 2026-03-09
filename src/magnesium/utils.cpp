#include "utils.h"

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "godot_extra/array_view.hpp"

using namespace godot;

namespace magnesium
{
	void utils::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("print_type", "variant"), &utils::print_type);
		ClassDB::bind_method(D_METHOD("dump", "script"), &utils::dump);
		{
			MethodInfo mi;
			mi.arguments.push_back(PropertyInfo(Variant::OBJECT, "object"));
			mi.arguments.push_back(PropertyInfo(Variant::STRING_NAME, "method_name"));
			mi.name = "try_call_method";
			mi.flags = METHOD_FLAG_STATIC | METHOD_FLAG_VARARG;
			ClassDB::bind_vararg_method(mi.flags, "try_call_method", &utils::try_call_method, mi);
		}
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
			return {};
		}
		if (args[0]->get_type() != Variant::OBJECT)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 0;
			error.expected = Variant::OBJECT;
			return {};
		}
		if (args[1]->get_type() != Variant::STRING_NAME)
		{
			error.error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
			error.argument = 1;
			error.expected = Variant::STRING_NAME;
			return {};
		}
		const auto obj{ static_cast<Object*>(*args[0]) };
		const auto method_name{ static_cast<StringName>(*args[1]) };

		// Fortunately, Object::callv() is safe to call on an invalid method name, it will just return an empty Variant. So we don't need to check if the method exists before calling it.
		godot_extra::array_view<const Variant*> arr{ args, arg_count };
		arr.skip(2);
		return obj ? obj->callv(method_name, arr.to_array()) : Variant{};
	}

	void utils::dump(const Variant& script) const
	{
		if (script.get_type() != Variant::OBJECT)
		{
			print_line("Not an object");
			return;
		}
		const auto obj{ static_cast<Object*>(script) };
		const auto property_list{ obj->get_property_list() };
		for (int i = 0; i < property_list.size(); ++i)
		{
			const auto& prop = static_cast<Dictionary>(property_list[i]);
			auto name = static_cast<String>(prop["name"]);
			auto type = prop["type"].stringify();
			auto usage = prop["usage"].stringify();
			auto value = obj->get(name);

			print_line(vformat("Property %d: %s (%s, %s) = %s", i, name, type, usage, value));
		}

		const auto method_list{ obj->get_method_list() };
		for (int i = 0; i < method_list.size(); ++i)
		{
			const auto& method = static_cast<Dictionary>(method_list[i]);
			auto name = static_cast<String>(method["name"]);
			auto args = method["args"];
			auto return_type = method["return"].stringify();

			print_line(vformat("Method %d: %s", i, name));
		}
	}
} //namespace magnesium