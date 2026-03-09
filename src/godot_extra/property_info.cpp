#include "property_info.hpp"
#include "godot_cpp/classes/global_constants.hpp"

#include <sstream>
#include <string>

using namespace godot;

namespace godot_extra::property_info
{
	String get_usage_names(uint32_t usage)
	{
		std::ostringstream result;
		if (usage & PROPERTY_USAGE_STORAGE)
			result << "STORAGE ";
		if (usage & PROPERTY_USAGE_EDITOR)
			result << "EDITOR ";
		if (usage & PROPERTY_USAGE_INTERNAL)
			result << "INTERNAL ";
		if (usage & PROPERTY_USAGE_CHECKABLE)
			result << "CHECKABLE ";
		if (usage & PROPERTY_USAGE_CHECKED)
			result << "CHECKED ";
		if (usage & PROPERTY_USAGE_CATEGORY)
			result << "CATEGORY ";
		if (usage & PROPERTY_USAGE_SUBGROUP)
			result << "SUBGROUP ";
		if (usage & PROPERTY_USAGE_CLASS_IS_BITFIELD)
			result << "CLASS_IS_BITFIELD ";
		if (usage & PROPERTY_USAGE_NO_INSTANCE_STATE)
			result << "NO_INSTANCE_STATE ";
		if (usage & PROPERTY_USAGE_RESTART_IF_CHANGED)
			result << "RESTART_IF_CHANGED ";
		if (usage & PROPERTY_USAGE_SCRIPT_VARIABLE)
			result << "SCRIPT_VARIABLE ";
		if (usage & PROPERTY_USAGE_STORE_IF_NULL)
			result << "STORE_IF_NULL ";
		if (usage & PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED)
			result << "UPDATE_ALL_IF_MODIFIED ";
		if (usage & PROPERTY_USAGE_SCRIPT_DEFAULT_VALUE)
			result << "SCRIPT_DEFAULT_VALUE ";
		if (usage & PROPERTY_USAGE_CLASS_IS_ENUM)
			result << "CLASS_IS_ENUM ";
		if (usage & PROPERTY_USAGE_NIL_IS_VARIANT)
			result << "NIL_IS_VARIANT ";
		if (usage & PROPERTY_USAGE_ARRAY)
			result << "ARRAY ";
		if (usage & PROPERTY_USAGE_ALWAYS_DUPLICATE)
			result << "ALWAYS_DUPLICATE ";
		if (usage & PROPERTY_USAGE_NEVER_DUPLICATE)
			result << "NEVER_DUPLICATE ";
		if (usage & PROPERTY_USAGE_HIGH_END_GFX)
			result << "HIGH_END_GFX ";
		if (usage & PROPERTY_USAGE_NODE_PATH_FROM_SCENE_ROOT)
			result << "NODE_PATH_FROM_SCENE_ROOT ";
		if (usage & PROPERTY_USAGE_RESOURCE_NOT_PERSISTENT)
			result << "RESOURCE_NOT_PERSISTENT ";
		if (usage & PROPERTY_USAGE_KEYING_INCREMENTS)
			result << "KEYING_INCREMENTS ";
		if (usage & PROPERTY_USAGE_DEFERRED_SET_RESOURCE)
			result << "DEFERRED_SET_RESOURCE ";
		if (usage & PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT)
			result << "EDITOR_INSTANTIATE_OBJECT ";
		if (usage & PROPERTY_USAGE_EDITOR_BASIC_SETTING)
			result << "EDITOR_BASIC_SETTING ";
		if (usage & PROPERTY_USAGE_READ_ONLY)
			result << "READ_ONLY ";
		if (usage & PROPERTY_USAGE_SECRET)
			result << "SECRET ";
		auto str = result.str();
		return str.empty() ? "NONE" : str.c_str();
	}
} // namespace godot_extra::property_info