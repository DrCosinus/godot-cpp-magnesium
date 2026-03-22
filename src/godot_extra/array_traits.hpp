#pragma once

// #include "array_view.hpp"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <vector>

namespace godot_extra
{
	template <typename T>
	struct array_view;

	template <typename T>
	struct array_traits
	{
		static_assert(sizeof(T) == 0, "array_traits not defined for type T");
		using type = void;
		using value_type = void;
		static constexpr const bool value_is_pointer{ false };
	};

	template <typename T>
	struct array_traits<std::vector<T>>
	{
		using type = std::vector<T>;
		using value_type = T;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
	};

	template <typename T>
	struct array_traits<array_view<T>>
	{
		using type = array_view<T>;
		using value_type = T;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
	};

	template <typename T>
	struct array_traits<godot::TypedArray<T>>
	{
		using type = godot::TypedArray<T>;
		using value_type = T;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
	};

	template <>
	struct array_traits<godot::Array>
	{
		using type = godot::Array;
		using value_type = godot::Variant;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
	};

	template <>
	struct array_traits<godot::PackedStringArray>
	{
		using type = godot::PackedStringArray;
		using value_type = godot::String;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
	};

}

/*
	template <typename T>
	struct array_proxy;
	template <>
	struct array_proxy<godot::Array>
	{
		using type = godot::Array;
		using value_type = godot::Variant;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
		array_proxy(type& arr) : arr{ arr }
		{
		}
		GDExtensionInt size() const
		{
			return arr.size();
		}
		void resize(GDExtensionInt count)
		{
			arr.resize(count);
		}
		void set(GDExtensionInt index, const godot::Variant& value)
		{
			arr[index] = value;
		}
		godot::Variant* native_ptr()
		{
			return reinterpret_cast<godot::Variant*>(arr._native_ptr());
		}

	private:
		type& arr;
	};
	template <>
	struct array_proxy<godot::PackedStringArray>
	{
		using type = godot::PackedStringArray;
		using value_type = godot::String;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
		array_proxy(type& arr) : arr{ arr }
		{
		}
		GDExtensionInt size() const
		{
			return arr.size();
		}
		void resize(GDExtensionInt count)
		{
			arr.resize(count);
		}
		void set(GDExtensionInt index, const godot::String& value)
		{
			arr[index] = value;
		}

	private:
		type& arr;
	};

	template <typename T>
	struct array_proxy<std::vector<T>>
	{
		using type = std::vector<T>;
		using value_type = T;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
		array_proxy(type& arr) : arr{ arr }
		{
		}
		GDExtensionInt size() const
		{
			return arr.size();
		}
		void resize(GDExtensionInt count)
		{
			arr.resize(count);
		}
		void set(GDExtensionInt index, const T& value)
		{
			arr[index] = value;
		}
		T* native_ptr()
		{
			return arr.data();
		}

	private:
		type& arr;
	};

	template <typename T>
	struct array_proxy<array_view<T>>
	{
		using type = array_view<T>;
		using value_type = T;
		static constexpr const bool value_is_pointer{ std::is_pointer_v<value_type> };
		array_proxy(type view) : view{ view }
		{
		}
		GDExtensionInt size() const
		{
			return view.size();
		}
		const T* native_ptr()
		{
			return view.begin();
		}

	private:
		type& view;
	};
*/