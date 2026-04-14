#pragma once

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace experimental
{
	struct ReadBytesStream
	{
		using value_type = uint8_t;
		using size_type = ::std::size_t;

		ReadBytesStream(const uint8_t* data, size_type size) : data{ data }, len{ size }
		{
		}
		ReadBytesStream(const godot::PackedByteArray& arr) : ReadBytesStream{ arr.ptr(), static_cast<size_type>(arr.size()) }
		{
		}
		ReadBytesStream() : data{ nullptr }, len{ 0 }
		{
		}
		ReadBytesStream(const ReadBytesStream& other) : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
		}
		ReadBytesStream(ReadBytesStream&& other) noexcept : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
			new (&other) ReadBytesStream();
		}
		ReadBytesStream& operator=(const ReadBytesStream& other)
		{
			new (this) ReadBytesStream(other);
			return *this;
		}
		ReadBytesStream& operator=(ReadBytesStream&& other) noexcept
		{
			new (this) ReadBytesStream(std::move(other));
			return *this;
		}
		size_type Position() const
		{
			return pos;
		}
		size_type size() const
		{
			return len;
		}
		void advance(size_type count)
		{
			ERR_FAIL_COND(pos + count > len);
			pos += count;
		}
		int32_t ReadInt32()
		{
			ERR_FAIL_COND_V(pos + 4 > len, 0);
			int32_t value = (data[pos] << 24) | (data[pos + 1] << 16) | (data[pos + 2] << 8) | data[pos + 3];
			pos += 4;
			return value;
		}
		int16_t ReadInt16()
		{
			ERR_FAIL_COND_V(pos + 2 > len, 0);
			int16_t value = (data[pos] << 8) | data[pos + 1];
			pos += 2;
			return value;
		}
		uint8_t ReadInt8()
		{
			ERR_FAIL_COND_V(pos + 1 > len, 0);
			uint8_t value = data[pos];
			pos += 1;
			return value;
		}
		godot::String ReadFixedLengthString(size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, "");
			godot::String str = godot::String::utf8(reinterpret_cast<const char*>(data + pos), static_cast<int64_t>(length));
			pos += length;
			return str;
		}
		godot::PackedByteArray ReadBytes(size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, {});
			godot::PackedByteArray arr;
			arr.resize(length);
			memcpy(arr.ptrw(), data + pos, length);
			pos += length;
			return arr;
		}
		ReadBytesStream slice(size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, {});
			return { data + pos, length };
		}
		godot::PackedByteArray copy() const
		{
			godot::PackedByteArray arr;
			arr.resize(len - pos);
			memcpy(arr.ptrw(), data + pos, len - pos);
			return arr;
		}

	private:
		const uint8_t* const data;
		size_type pos{ 0 };
		size_type len;
	};
} //namespace experimental