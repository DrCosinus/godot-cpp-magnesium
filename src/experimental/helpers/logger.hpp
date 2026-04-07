#pragma once

#include <godot_cpp/variant/string.hpp>

namespace experimental
{
	template <typename T>
	static constexpr bool log_enabled = false;

#define ENABLE_LOG(type) \
	template <>          \
	static constexpr bool log_enabled<type> = true;

#define LOG_MESSAGE(type, message, ...) Logger<type>::print(message, ##__VA_ARGS__)

	template <typename T>
	struct Logger
	{
		template <typename... Args>
		static void print(godot::String message, Args... args)
		{
			if constexpr (log_enabled<T>)
			{
				print_line(vformat(message, args...));
			}
		}
	};
} //namespace experimental