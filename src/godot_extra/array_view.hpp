#pragma once

#include "godot_cpp/variant/array.hpp"
#include <type_traits>

namespace godot_extra
{
	template <typename T>
	struct array_view
	{
		const T* data;
		GDExtensionInt size;

		array_view(const T* data, GDExtensionInt size) : data(data), size(size)
		{
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
	};
} //namespace godot_extra