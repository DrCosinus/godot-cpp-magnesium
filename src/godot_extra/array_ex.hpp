#pragma once

#include "array_traits.hpp"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot
{
	inline godot::Variant* begin(godot::Array& arr)
	{
		return reinterpret_cast<Variant*>(*(*reinterpret_cast<uint64_t**>(arr._native_ptr()) + 2));
	}

	inline godot::Variant* end(godot::Array& arr)
	{
		return begin(arr) + arr.size();
	}

	template <typename T>
	T* begin(godot::TypedArray<T>& arr)
	{
		return reinterpret_cast<T*>(*reinterpret_cast<uint64_t**>(arr._native_ptr()) + 2);
	}

	template <typename T>
	T* end(godot::TypedArray<T>& arr)
	{
		return begin(arr) + arr.size();
	}

	inline godot::String* begin(godot::PackedStringArray& arr)
	{
		return reinterpret_cast<godot::String*>(*(reinterpret_cast<uint64_t*>(arr._native_ptr()) + 1));
	}

	inline godot::String* end(godot::PackedStringArray& arr)
	{
		return begin(arr) + arr.size();
	}
} //namespace godot

namespace godot_extra
{
	template <typename OC = godot::Array, typename IC>
	auto array_transform(IC&& view)
	{
		OC arr;
		arr.resize(view.size());

		GDExtensionInt i{ 0 };
		for (auto item : view)
		{
			if constexpr (array_traits<std::decay_t<IC>>::value_is_pointer)
				arr[i] = *item;
			else
				arr[i] = item;
			++i;
		}
		return arr;
	}

	// supports godot arrays, typed arrays, std::vector, array_views, and any other container with begin(), end(), and size()
	// indeed, for now container must support back_inserter, and so push_back
	template <template <typename...> typename OutputContainer = std::vector, typename C, typename F>
	auto array_select(C&& view, F&& func)
	{
		using T = typename array_traits<std::decay_t<C>>::value_type;
		using R = std::decay_t<std::invoke_result_t<F, const T&>>;
		using Out = OutputContainer<R>;
		Out result;
		result.resize(view.size());
		GDExtensionInt i{ 0 };
		for (const auto& item : view)
		{
			result[i++] = func(item);
		}
		return result;
	}

	template <typename PackedOutputContainer, typename C, typename F>
	auto array_select(C&& view, F&& func)
	{
		PackedOutputContainer result;
		result.resize(view.size());
		GDExtensionInt i{ 0 };
		for (const auto& item : view)
		{
			result[i++] = func(item);
		}
		return result;
	}
} //namespace godot_extra