#pragma once

#include "godot_cpp/core/memory.hpp"

namespace magnesium
{
	template <typename T>
	class InstanceProvider
	{
	public:
		static T*& instance()
		{
			static T* instance{ nullptr };
			return instance;
		}
		static void create_instance()
		{
			if (!instance())
			{
				instance() = memnew(T);
			}
		}
		static void destroy_instance()
		{
			if (instance())
			{
				memdelete(instance());
				instance() = nullptr;
			}
		}
	};
} // namespace magnesium