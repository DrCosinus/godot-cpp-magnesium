#pragma once

#include "serializer.hpp"
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace experimental
{
	struct WriteStreamSerializer : public Serializer
	{
		WriteStreamSerializer(godot::PackedByteArray& stream) : Serializer{ true }, stream{ stream } {}

		bool ProcessInt8(godot::StringName name, int8_t& value) override
		{
			(void)name;
			stream.append(value);
			return true; // return true if the value was successfully written
		}

		bool ProcessInt16(godot::StringName name, int16_t& value) override
		{
			(void)name;
			// endianess: we will write in big-endian format (most significant
			// byte first) regardless of the platform's native endianess, to
			// ensure consistent serialization across platforms
			stream.append(static_cast<uint8_t>((value >> 8) & 0xFF));
			stream.append(static_cast<uint8_t>(value & 0xFF));
			return true;
		}

		bool ProcessInt32(godot::StringName name, int32_t& value) override
		{
			(void)name;
			stream.append(static_cast<uint8_t>((value >> 24) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 16) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 8) & 0xFF));
			stream.append(static_cast<uint8_t>(value & 0xFF));
			return true;
		}

		bool ProcessInt64(godot::StringName name, int64_t& value) override
		{
			(void)name;
			stream.append(static_cast<uint8_t>((value >> 56) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 48) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 40) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 32) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 24) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 16) & 0xFF));
			stream.append(static_cast<uint8_t>((value >> 8) & 0xFF));
			stream.append(static_cast<uint8_t>(value & 0xFF));
			return true;
		}

		bool ProcessPackedByteArray(godot::StringName name, godot::PackedByteArray& value, size_t& size) override
		{
			(void)name;
			stream.append_array(value);
			size = value.size();
			return true;
		}

	private:
		godot::PackedByteArray& stream;
	};
} //namespace experimental