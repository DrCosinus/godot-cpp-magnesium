#pragma once

#include <gdextension_interface.h>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace godot
{
	class Variant;
}

namespace magnesium
{
	template <typename T>
	static constexpr const char* dump_format = nullptr;
	template <>
	static constexpr const char* dump_format<godot::Variant> = "%s";
	template <>
	static constexpr const char* dump_format<uint8_t> = "%02X";
	template <>
	static constexpr const char* dump_format<uint32_t> = "%08X";
	template <>
	static constexpr const char* dump_format<uint64_t> = "%016X";

	template <typename U = uint8_t, typename T>
	static godot::String Dump(const T* ptr, GDExtensionInt count = 8, godot::String separator = " "	)
	{
		static_assert(dump_format<U> != nullptr, "Dump format not defined for type U");
		godot::PackedStringArray words;
		words.resize(count + 1);
		words[0] = vformat("[%016X]:", reinterpret_cast<uint64_t>(ptr));
		const U* uptr = reinterpret_cast<const U*>(ptr);
		for (int i = 0; i < count; ++i)
		{
			words[i + 1] = vformat(dump_format<U>, uptr[i]);
		}
		return separator.join(words);
	}
}