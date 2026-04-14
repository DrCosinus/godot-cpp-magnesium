#pragma once

#include "serializable.hpp"
#include <cstdint>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace godot
{
	class PackedByteArray;
}

namespace experimental
{
	struct Serializer
	{
		Serializer(bool writing) : is_writing(writing) {}
		virtual ~Serializer() = default;

		bool IsWriting() const { return is_writing; }

		virtual bool ProcessInt8(godot::StringName name, int8_t& value) = 0;
		virtual bool ProcessInt16(godot::StringName name, int16_t& value) = 0;
		virtual bool ProcessInt32(godot::StringName name, int32_t& value) = 0;
		virtual bool ProcessInt64(godot::StringName name, int64_t& value) = 0;

		virtual bool ProcessPackedByteArray(godot::StringName name, godot::PackedByteArray& value, size_t& size) = 0;

	protected:
		bool is_writing;
	};
} // namespace experimental
