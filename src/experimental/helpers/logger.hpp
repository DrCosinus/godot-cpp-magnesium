#pragma once

// #include "string_literal.hpp"
#include <godot_cpp/variant/string.hpp>

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
				print_line(vformat(message, args...));
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
				print_error(vformat(message, args...));
			}
		}
	};
} //namespace experimental