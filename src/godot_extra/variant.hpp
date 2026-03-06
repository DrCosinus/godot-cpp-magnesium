#pragma once

#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/variant.hpp"

#include <algorithm>
#include <array>

namespace godot_extra::variant
{
	template <typename... Args>
	godot::Variant call_static(const godot::Variant& variant, godot::Variant::Type type, const godot::StringName& method, Args... args)
	{
		std::array<godot::Variant, sizeof...(args)> vargs = { args... };
		std::array<const godot::Variant*, sizeof...(args)> argptrs;
		std::transform(vargs.begin(), vargs.end(), argptrs.begin(), [](const godot::Variant& v) { return &v; });
		godot::Variant result;
		GDExtensionCallError error;
		variant.callp_static(type, method, argptrs.data(), argptrs.size(), result, error);
		return result;
	}
} //namespace godot_extra::variant