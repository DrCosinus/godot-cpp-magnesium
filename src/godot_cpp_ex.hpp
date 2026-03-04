#pragma once

#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/variant.hpp"
// #include "godot_cpp/core/error_macros.hpp"

#include <array>
#include <algorithm>

namespace GodotEx::VariantEx
{
	template <typename... Args>
	godot::Variant call_static(const godot::Variant& variant, godot::Variant::Type type, const godot::StringName& method, Args... args)
	{
		std::array<godot::Variant, sizeof...(args)> vargs = { args... };
		std::array<const godot::Variant*, sizeof...(args)> argptrs;
		std::transform(vargs.begin(), vargs.end(), argptrs.begin(), [](const godot::Variant& v) { return &v; });
		// for (size_t i = 0; i < vargs.size(); ++i)
		// {
		// 	argptrs[i] = &vargs[i];
		// }
		godot::Variant result;
		GDExtensionCallError error;
		variant.callp_static(type, method, argptrs.data(), argptrs.size(), result, error);
		return result;
	}
} //namespace GodotEx::VariantEx