#pragma once

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace experimental
{
	struct readonly_bytes_stream
	{
		using value_type = uint8_t;
		using size_type = ::std::size_t;

		readonly_bytes_stream(const uint8_t* data, size_type size) : data{ data }, len{ size }
		{
		}
		readonly_bytes_stream(const godot::PackedByteArray& arr) : readonly_bytes_stream{ arr.ptr(), static_cast<size_type>(arr.size()) }
		{
		}
		readonly_bytes_stream() : data{ nullptr }, len{ 0 }
		{
		}
		readonly_bytes_stream(const readonly_bytes_stream& other) : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
		}
		readonly_bytes_stream(readonly_bytes_stream&& other) noexcept : data{ other.data }, pos{ other.pos }, len{ other.len }
		{
			new (&other) readonly_bytes_stream();
		}
		readonly_bytes_stream& operator=(const readonly_bytes_stream& other)
		{
			new (this) readonly_bytes_stream(other);
			return *this;
		}
		readonly_bytes_stream& operator=(readonly_bytes_stream&& other) noexcept
		{
			new (this) readonly_bytes_stream(std::move(other));
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
		uint8_t ReadInt8()
		{
			ERR_FAIL_COND_V(pos + 1 > len, 0);
			uint8_t value = data[pos];
			pos += 1;
			return value;
		}
		godot::String ReadString(size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, "");
			godot::String str = godot::String::utf8(reinterpret_cast<const char*>(data + pos), static_cast<int64_t>(length));
			pos += length;
			return str;
		}
		readonly_bytes_stream slice(size_type length)
		{
			ERR_FAIL_COND_V(pos + length > len, {});
			return { data + pos, length };
		}

	private:
		const uint8_t* const data;
		size_type pos{ 0 };
		size_type len;
	};
} //namespace experimental