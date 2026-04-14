#pragma once

#include "logger.hpp"
#include "serializer.hpp"
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace experimental
{
	struct PackedByteArrayReader;

	struct PackedByteArrayReader : public Serializer
	{
		inline static Logger<false> Log{ "PBAReader" };

		PackedByteArrayReader(godot::PackedByteArray& stream) : Serializer{ false }, stream{ stream } {}

		bool ProcessInt8(godot::StringName name, int8_t& value) override
		{
			(void)name;
			if (stream.size() < 1)
			{
				Log.print("Not enough data in stream to read Int8: %s, expected 1 byte, actual size: %d", name, stream.size());
				return false;
			}
			value = NextByte();
			Log.print("Read Int8: %s = %d", name, value);
			return true; // return true if the value was successfully read
		}

		bool ProcessInt16(godot::StringName name, int16_t& value) override
		{
			(void)name;
			if (stream.size() < 2)
			{
				Log.print("Not enough data in stream to read Int16: %s, expected 2 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (NextByte<int16_t>() << 8) | NextByte<int16_t>();
			Log.print("Read Int16: %s = %d", name, value);
			return true;
		}

		bool ProcessInt32(godot::StringName name, int32_t& value) override
		{
			(void)name;
			if (stream.size() < 4)
			{
				Log.print("Not enough data in stream to read Int32: %s, expected 4 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (NextByte<int32_t>() << 24) | (NextByte<int32_t>() << 16) | (NextByte<int32_t>() << 8) | NextByte<int32_t>();
			Log.print("Read Int32: %s = %d", name, value);

			return true;
		}

		bool ProcessInt64(godot::StringName name, int64_t& value) override
		{
			(void)name;
			if (stream.size() < 8)
			{
				Log.print("Not enough data in stream to read Int64: %s, expected 8 bytes, actual size: %d", name, stream.size());
				return false;
			}
			value = (NextByte<int64_t>() << 56) | (NextByte<int64_t>() << 48) | (NextByte<int64_t>() << 40) | (NextByte<int64_t>() << 32) |
					(NextByte<int64_t>() << 24) | (NextByte<int64_t>() << 16) | (NextByte<int64_t>() << 8) | NextByte<int64_t>();
			Log.print("Read Int64: %s = %d", name, value);
			return true;
		}

		bool ProcessPackedByteArray(godot::StringName name, godot::PackedByteArray& value, size_t& size) override
		{
			(void)name;
			Log.print("Read PackedByteArray: %s, expected size: %d", name, size);
			if (size < 0)
				size = value.size(); // if size is not provided, we will read the size from the provided value array

			if (stream.size() < size)
			{
				Log.print("Not enough data in stream to read PackedByteArray: %s, expected size: %d, actual size: %d", name, size, stream.size());
				return false;
			}
			Log.print("Reading PackedByteArray: %s, size: %d from stream (remaining: %d)", name, size, stream.size());
			value.resize(size);
			memcpy(value.ptrw(), stream.ptr(), size);
			Log.print("Read PackedByteArray `%s`, first byte: %02X", name, value.size() > 0 ? value[0] : -1);
			stream = stream.slice(size);
			Log.print("Remaining stream size after reading PackedByteArray `%s`: %d", name, stream.size());
			return true;
		}

	private:
		int64_t RemainingSize() const { return stream.size() - index; }
		template <typename T = int8_t>
		T NextByte()
		{
			if (index >= stream.size())
			{
				Log.print_error("No more data in stream to read next byte, index: %d, stream size: %d", index, stream.size());
				return static_cast<T>(-1);
			}
			int8_t byte = static_cast<int8_t>(stream[index]);
			Log.print("Read next byte from stream: %02X at index: %d", byte, index);
			index++;
			return static_cast<T>(byte);
		}
		int64_t index{ 0 };
		godot::PackedByteArray& stream;
	};
} //namespace experimental
