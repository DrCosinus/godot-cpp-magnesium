#pragma once

#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>

namespace godot_extra
{
	template <typename T>
	struct array_view
	{
		using value_type = T;
		array_view(const T* data, GDExtensionInt size) : data{ data }, len{ size }
		{
		}
		array_view(std::vector<T> vec) : array_view{ vec.data(), static_cast<GDExtensionInt>(vec.size()) }
		{
		}
		array_view(godot::TypedArray<T>& arr) : array_view{ godot::begin(arr), static_cast<GDExtensionInt>(arr.size()) }
		{
		}
		array_view(godot::Array& arr) : array_view{ godot::begin(arr), arr.size() }
		{
		}
		array_view(godot::PackedStringArray& arr) : array_view{ godot::begin(arr), arr.size() }
		{
		}
		const T* begin() const
		{
			return data;
		}
		const T* end() const
		{
			return data + len;
		}
		const T& operator[](GDExtensionInt index) const
		{
			ERR_FAIL_COND_V(index >= len, T{});
			return data[index];
		}
		std::size_t size() const
		{
			return static_cast<std::size_t>(len);
		}
		void skip(GDExtensionInt count)
		{
			ERR_FAIL_COND(count > len);
			data += count;
			len -= count;
		}

	private:
		const T* data;
		GDExtensionInt len;
	};

	template <typename T>
	const T* begin(array_view<T>& view)
	{
		return view.begin();
	}

	template <typename T>
	const T* end(array_view<T>& view)
	{
		return view.end();
	}

	// deduction guides
	template <typename T>
	array_view(const T* data, GDExtensionInt size) -> array_view<T>;

	template <typename T>
	array_view(const std::vector<T>& vec) -> array_view<T>;

	template <typename T>
	array_view(const godot::TypedArray<T>& arr) -> array_view<T>;

	array_view(const godot::Array& arr) -> array_view<godot::Variant>;

	array_view(const godot::PackedStringArray& arr) -> array_view<godot::String>;
} //namespace godot_extra