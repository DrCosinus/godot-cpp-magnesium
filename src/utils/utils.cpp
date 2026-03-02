#include "utils.h"

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

namespace magnesium::utils
{
	void MgUtils::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("print_type", "variant"), &MgUtils::print_type);
		ClassDB::bind_static_method(get_class_static(), D_METHOD("try_call_method", "obj", "method_name"), &MgUtils::try_call_method);
	}

	void MgUtils::print_type(const Variant& p_variant) const
	{
		print_line(vformat("Type: %d", p_variant.get_type()));
	}

	Variant MgUtils::try_call_method(Object* obj, const StringName& method_name)
	{
		// Fortunately, Object::call() is safe to call on an invalid method name, it will just return an empty Variant. So we don't need to check if the method exists before calling it.
		return obj ? obj->call(method_name) : Variant{};
	}
} //namespace magnesium::utils