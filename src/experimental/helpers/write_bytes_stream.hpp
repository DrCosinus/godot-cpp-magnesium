#pragma once

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace experimental
{
	struct WriteBytesStream
	{
		using value_type = uint8_t;
		using size_type = ::std::size_t;
		WriteBytesStream(godot::PackedByteArray& arr) : WriteBytesStream{ arr.ptrw(), static_cast<size_type>(arr.size()) }
		{
		}
		WriteBytesStream() : data{ nullptr }, len{ 0 }
		{
		}
		WriteBytesStream(const WriteBytesStream& other) : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
		}
		WriteBytesStream(WriteBytesStream&& other) noexcept : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
			new (&other) WriteBytesStream();
		}
		WriteBytesStream& operator=(const WriteBytesStream& other)
		{
			new (this) WriteBytesStream(other);
			return *this;
		}
		WriteBytesStream& operator=(WriteBytesStream&& other) noexcept
		{
			new (this) WriteBytesStream(std::move(other));
			return *this;
		}
		bool WriteInt32(int32_t value)
		{
			ERR_FAIL_COND_V(pos + 4 > len, false);
			data[pos] = (value >> 24) & 0xFF;
			data[pos + 1] = (value >> 16) & 0xFF;
			data[pos + 2] = (value >> 8) & 0xFF;
			data[pos + 3] = value & 0xFF;
			pos += 4;
			return true;
		}
		bool WriteInt16(int16_t value)
		{
			ERR_FAIL_COND_V(pos + 2 > len, false);
			data[pos] = (value >> 8) & 0xFF;
			data[pos + 1] = value & 0xFF;
			pos += 2;
			return true;
		}
		bool WriteInt8(uint8_t value)
		{
			ERR_FAIL_COND_V(pos + 1 > len, false);
			data[pos] = value;
			pos += 1;
			return true;
		}
		bool WriteBytes(const uint8_t* bytes, size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, false);
			memcpy(data + pos, bytes, length);
			pos += length;
			return true;
		}
		bool WriteBytes(const godot::PackedByteArray& arr)
		{
			return WriteBytes(arr.ptr(), static_cast<size_type>(arr.size()));
		}
		bool WriteString(const godot::String& str)
		{
			// for now we encode strings as UTF-8 with a 32-bit length prefix. We can add support for other encodings later if needed.
			godot::PackedByteArray utf8 = str.to_utf8_buffer();
			if (!WriteInt32(static_cast<int32_t>(utf8.size())))
				return false;
			return WriteBytes(utf8.ptr(), static_cast<size_type>(utf8.size()));
		}
		bool WriteFixedLengthString(const godot::String& str, size_type fixed_length)
		{
			auto str_length = str.length();
			bool res = WriteBytes(reinterpret_cast<const uint8_t*>(str.utf8().ptr()), fixed_length);
			if (res && fixed_length > str_length)
			{
				// pad with zeros if the string is shorter than the fixed length
				size_type padding_length = fixed_length - str_length;
				memset(data + pos, 0, padding_length);
				pos += padding_length;
			}
			return res;
		}

	private:
		WriteBytesStream(uint8_t* data, size_type size) : data{ data }, len{ size }
		{
		}

		uint8_t* const data;
		size_type pos{ 0 };
		size_type len;
	};
} //namespace experimental