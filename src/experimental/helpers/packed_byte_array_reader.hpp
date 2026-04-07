#pragma once

#include "serializer.hpp"
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace experimental
{
	struct PackedByteArrayReader : public Serializer
	{
		PackedByteArrayReader(godot::PackedByteArray& stream) : Serializer{ false }, stream{ stream } {}

		template <typename... T>
		static void log(godot::String message, T... args)
		{
#if 0
			print_line(vformat(message, args...));
#endif
		}

		bool ProcessInt8(godot::StringName name, int8_t& value) override
		{
			// name is ignored for now since our WriteBytesStream doesn't
			// support named fields, but we include it in the interface in
			// case we want to add support for it later
			(void)name;
			if (stream.size() < 1)
			{
				log("[PBAReader] Not enough data in stream to read Int8: %s, expected 1 byte, actual size: %d", name, stream.size());
				return false;
			}
			value = static_cast<int8_t>(stream[0]);
			log("[PBAReader] Read Int8: %s = %d", name, value);
			stream.remove_at(0);
			return true; // return true if the value was successfully read
		}

		bool ProcessInt16(godot::StringName name, int16_t& value) override
		{
			(void)name;
			// endianess: we will write in big-endian format (most significant
			// byte first) regardless of the platform's native endianess, to
			// ensure consistent serialization across platforms
			if (stream.size() < 2)
			{
				log("[PBAReader] Not enough data in stream to read Int16: %s, expected 2 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (static_cast<int16_t>(stream[0]) << 8) | static_cast<int16_t>(stream[1]);
			log("[PBAReader] Read Int16: %s = %d", name, value);
			stream = stream.slice(2); // remove the first 2 bytes that we just read
			return true;
		}

		bool ProcessInt32(godot::StringName name, int32_t& value) override
		{
			(void)name;
			if (stream.size() < 4)
			{
				log("[PBAReader] Not enough data in stream to read Int32: %s, expected 4 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (static_cast<int32_t>(stream[0]) << 24) |
					(static_cast<int32_t>(stream[1]) << 16) |
					(static_cast<int32_t>(stream[2]) << 8) |
					static_cast<int32_t>(stream[3]);
			log("[PBAReader] Read Int32: %s = %d", name, value);
			stream = stream.slice(4); // remove the first 4 bytes that we just read
			return true;
		}

		bool ProcessInt64(godot::StringName name, int64_t& value) override
		{
			(void)name;
			if (stream.size() < 8)
			{
				log("[PBAReader] Not enough data in stream to read Int64: %s, expected 8 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (static_cast<int64_t>(stream[0]) << 56) |
					(static_cast<int64_t>(stream[1]) << 48) |
					(static_cast<int64_t>(stream[2]) << 40) |
					(static_cast<int64_t>(stream[3]) << 32) |
					(static_cast<int64_t>(stream[4]) << 24) |
					(static_cast<int64_t>(stream[5]) << 16) |
					(static_cast<int64_t>(stream[6]) << 8) |
					static_cast<int64_t>(stream[7]);
			log("[PBAReader] Read Int64: %s = %d", name, value);
			stream = stream.slice(8); // remove the first 8 bytes that we just read
			return true;
		}

		bool ProcessPackedByteArray(godot::StringName name, godot::PackedByteArray& value, size_t& size) override
		{
			(void)name;
			log("[PBAReader] Read PackedByteArray: %s, expected size: %d", name, size);
			if (size < 0)
				size = value.size(); // if size is not provided, we will read the size from the provided value array

			if (stream.size() < size)
			{
				log("[PBAReader] Not enough data in stream to read PackedByteArray: %s, expected size: %d, actual size: %d", name, size, stream.size());
				return false;
			}
			log("[PBAReader] Reading PackedByteArray: %s, size: %d from stream (remaining: %d)", name, size, stream.size());
			value.resize(size);
			memcpy(value.ptrw(), stream.ptr(), size);
			log("[PBAReader] Read PackedByteArray: %s, first byte: %d", name, value.size() > 0 ? value[0] : -1);
			stream = stream.slice(size);
			log("[PBAReader] Remaining stream size after reading PackedByteArray: %s: %d", name, stream.size());
			return true;
		}

	private:
		godot::PackedByteArray& stream;
	};
} //namespace experimental
