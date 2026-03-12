#pragma once

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/script.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"

namespace magnesium
{
	class utils : public godot::Object
	{
		GDCLASS(utils, Object)

	protected:
		static void _bind_methods();

	public:
		utils() = default;
		~utils() override = default;

		void print_type(const godot::Variant& p_variant) const;
		godot::Variant try_call_method(const godot::Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error);
		void dump(const godot::Variant& variant) const;
		godot::TypedArray<godot::StringName> get_own_methods(godot::Object* obj) const;
	};
} // namespace magnesium
