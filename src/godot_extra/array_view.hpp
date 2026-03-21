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
		array_view(godot::TypedArray<T>& arr) : array_view{ static_cast<const T*>(arr._native_ptr()), static_cast<GDExtensionInt>(arr.size()) }
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

	// helper to map a function over an array_view and return a godot array
	// (or any other container with reserve() or resize())
	template <typename T>
	struct array_traits;
	template <>
	struct array_traits<godot::Array>
	{
		using type = godot::Array;
		using value_type = godot::Variant;
		static void resize(godot::Array& arr, GDExtensionInt count)
		{
			arr.resize(count);
		}
		static void set(godot::Array& arr, GDExtensionInt index, const godot::Variant& value)
		{
			arr[index] = value;
		}
	};
	template <>
	struct array_traits<godot::PackedStringArray>
	{
		using type = godot::PackedStringArray;
		using value_type = godot::String;
		static void resize(godot::PackedStringArray& arr, GDExtensionInt count)
		{
			arr.resize(count);
		}
		static void set(godot::PackedStringArray& arr, GDExtensionInt index, const godot::String& value)
		{
			arr[index] = value;
		}
	};
	template <typename T>
	struct array_traits<std::vector<T>>
	{
		using type = array_view<T>;
		using value_type = T;
		static void resize(std::vector<T>& arr, GDExtensionInt count)
		{
			arr.resize(count);
		}
		static void set(std::vector<T>& arr, GDExtensionInt index, const T& value)
		{
			arr[index] = value;
		}
	};
	template <typename T>
	struct array_traits<array_view<T>>
	{
		using type = array_view<T>;
		using value_type = T;
	};

	template <typename OC = godot::Array, typename IC>
	auto to_array(IC&& view)
	{
		OC arr;
		array_traits<OC>::resize(arr, view.size());

		GDExtensionInt i{ 0 };
		for (auto item : view)
		{
			if constexpr (std::is_pointer_v<typename std::decay_t<IC>::value_type>) // if the input container holds pointers, dereference them before setting in the output container
				array_traits<OC>::set(arr, i, *item);
			else
				array_traits<OC>::set(arr, i, item);
			++i;
		}
		return arr;
	}

	// helper to map a function over an array_view and return any container with reserve() or resize()

	template <typename C, typename = void>
	struct reserve_helper
	{
		static void reserve(C&, size_t)
		{
		}
	};
	template <typename C>
	struct reserve_helper<C, std::void_t<decltype(std::declval<C&>().reserve(std::declval<size_t>()))>>
	{
		static void reserve(C& c, size_t n)
		{
			c.reserve(n);
		}
	};
	template <typename R, typename... Ts>
	using always_t = R;
	template <typename C>
	struct reserve_helper<C, always_t<int, decltype(std::declval<C&>().resize(std::declval<size_t>()))>>
	{
		static void resize(C& c, size_t n)
		{
			c.resize(n);
		}
	};

	// supports godot arrays, typed arrays, std::vector, array_views, and any other container with begin(), end(), and size()
	// indeed, for now container must support back_inserter, and so push_back
	template <template <typename...> typename OutputContainer = std::vector, typename C, typename F>
	auto map_span(C&& view, F&& func)
	{
		using T = typename std::decay_t<C>::value_type;
		using R = std::decay_t<std::invoke_result_t<F, const T&>>;
		using Out = OutputContainer<R>;
		Out result;
		reserve_helper<Out>::reserve(result, view.size());
		std::transform(view.begin(), view.end(), std::back_inserter(result), std::forward<F>(func));
		return result;
	}

	// deduction guides
	template <typename T>
	array_view(const T* data, GDExtensionInt size) -> array_view<T>;

	template <typename T>
	array_view(const std::vector<T>& vec) -> array_view<T>;

	template <typename T>
	array_view(const godot::TypedArray<T>& arr) -> array_view<T>;
} //namespace godot_extra