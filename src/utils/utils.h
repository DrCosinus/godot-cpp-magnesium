#pragma once

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"

namespace magnesium::utils
{
	class MgUtils : public godot::Object
	{
		GDCLASS(MgUtils, Object)

	protected:
		static void _bind_methods();

	public:
		MgUtils() = default;
		~MgUtils() override = default;

		void print_type(const godot::Variant& p_variant) const;
		static godot::Variant try_call_method(godot::Object* obj, const godot::StringName& method_name);
	};
} // namespace magnesium::utils
