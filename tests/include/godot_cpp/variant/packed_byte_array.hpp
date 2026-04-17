#pragma once

#include <cstddef>
#include <vector>

namespace godot
{
	class PackedByteArray
	{
		std::vector<unsigned char> data;

	public:
		PackedByteArray() = default;
		PackedByteArray(size_t n) : data(n) {}
		bool is_empty() const { return data.empty(); }
		size_t size() const { return data.size(); }
		unsigned char& operator[](size_t i) { return data[i]; }
		const unsigned char& operator[](size_t i) const { return data[i]; }
		const unsigned char* ptr() const { return data.empty() ? nullptr : &data[0]; }
		void resize(size_t n) { data.resize(n); }
	};
} //namespace godot
