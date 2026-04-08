#pragma once

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace experimental
{
	template <bool ENABLE>
	struct Logger
	{
		const char* const prefix;
		constexpr Logger(const char* prefix = nullptr) : prefix(prefix) {}

		template <typename... Args>
		void print(godot::String message, Args... args)
		{
			if constexpr (ENABLE)
			{
				if (prefix != nullptr)
				{
					message = vformat("[%s]: %s", prefix, message);
				}
				UtilityFunctions::print(vformat(message, args...));
			}
		}

		template <typename... Args>
		void print_error(godot::String message, Args... args)
		{
			if constexpr (ENABLE)
			{
				if (prefix != nullptr)
				{
					message = vformat("[%s][ERROR]: %s", prefix, message);
				}
				UtilityFunctions::printerr(vformat(message, args...));
			}
		}
	};
} //namespace experimental