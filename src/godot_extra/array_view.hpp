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
		T* data;
		GDExtensionInt size;

		array_view(T* data, GDExtensionInt size) : data{ data }, size{ size }
		{
		}
		array_view(std::vector<T>& vec) : array_view{ vec.data(), static_cast<GDExtensionInt>(vec.size()) }
		{
		}
		array_view(godot::TypedArray<T>& arr) : array_view{ static_cast<T*>(arr._native_ptr()), static_cast<GDExtensionInt>(arr.size()) }
		{
		}
		T* begin() const
		{
			return data;
		}
		T* end() const
		{
			return data + size;
		}
		const T& operator[](GDExtensionInt index) const
		{
			ERR_FAIL_COND_V(index >= size, T{});
			return data[index];
		}
		GDExtensionInt length() const
		{
			return size;
		}
		void skip(GDExtensionInt count)
		{
			ERR_FAIL_COND(count > size);
			data += count;
			size -= count;
		}
		godot::Array to_array() const
		{
			godot::Array arr;
			arr.resize(size);
			for (GDExtensionInt i = 0; i < size; ++i)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					arr[i] = *data[i]; // Assuming T is a pointer type, we dereference it to get the actual value. If T is not a pointer type, this will need to be adjusted.
				}
				else
				{
					arr[i] = data[i];
				}
			}
			return arr;
		}
		// map method to convert the array view to a vector of another type using a provided mapping function
		template <typename U>
		std::vector<U> map(std::function<U(const T&)> func) const
		{
			std::vector<U> result;
			result.reserve(size);
			std::transform(begin(), end(), std::back_inserter(result), std::move(func));
			return result;
		}
	};

	// deduction guides
	template <typename T>
	array_view(const T* data, GDExtensionInt size) -> array_view<T>;

	template <typename T>
	array_view(const std::vector<T>& vec) -> array_view<T>;

	template <typename T>
	array_view(const godot::TypedArray<T>& arr) -> array_view<T>;

} //namespace godot_extra