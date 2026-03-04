#pragma once

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <type_traits>

namespace magnesium
{
	class utils : public godot::Object
	{
		GDCLASS(utils, Object)

	protected:
		static void _bind_methods();

	public:
		utils() = default;
		~utils() override = default;

		// static utils* create_singleton()
		// {
		// 	return memnew(utils);
		// }

		void print_type(const godot::Variant& p_variant) const;
		godot::Variant try_call_method(const godot::Variant** args, GDExtensionInt arg_count, GDExtensionCallError& error);
	};

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
			ERR_FAIL_COND(index >= size);
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
} // namespace magnesium
