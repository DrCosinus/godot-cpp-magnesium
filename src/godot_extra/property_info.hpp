#pragma once

#include "godot_cpp/variant/string.hpp"

namespace godot_extra::property_info
{
	godot::String get_usage_names(uint32_t usage);
} // namespace godot_extra::property_info