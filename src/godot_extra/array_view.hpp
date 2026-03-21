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
	struct to_array_helper
	{
		using type = godot::Array;
	};
	template <>
	struct to_array_helper<godot::String>
	{
		using type = godot::PackedStringArray;
	};
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
		using to_array_result_type = typename to_array_helper<T>::type;
		to_array_result_type to_array() const
		{
			to_array_result_type arr;
			arr.resize(len);
			for (GDExtensionInt i = 0; i < len; ++i)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					arr[i] = *data[i];
				}
				else
				{
					arr[i] = data[i];
				}
			}
			return arr;
		}

	private:
		const T* data;
		GDExtensionInt len;
	};

	template<typename C, typename = void>
	struct reserve_helper
	{
		static void reserve(C&, size_t) {}
	};
	template<typename C>
	struct reserve_helper<C, std::void_t<decltype(std::declval<C&>().reserve(std::declval<size_t>()))>>
	{
		static void reserve(C& c, size_t n) { c.reserve(n); }
	};
	template <typename R, typename... Ts>
	using always_t = R;
	template <typename C>
	struct reserve_helper<C, always_t<int, decltype(std::declval<C&>().resize(std::declval<size_t>()))>>
	{
		static void resize(C& c, size_t n) { c.resize(n); }
	};

	// supports godot arrays, typed arrays, std::vector, array_views, and any other container with begin(), end(), and size()
	// indeed, for now container must support back_inserter, and so push_back
	template <template <typename...> typename OutputContainer = std::vector,typename C, typename F>
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