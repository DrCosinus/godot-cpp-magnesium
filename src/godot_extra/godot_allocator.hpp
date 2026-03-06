#pragma once

#include "godot_cpp/core/memory.hpp"

template <typename T>
struct GodotAllocator
{
	using value_type = T;

	GodotAllocator()
	{
	}

	template <typename U>
	GodotAllocator(const GodotAllocator<U>&) noexcept
	{
	}

	T* allocate(std::size_t n)
	{
		return static_cast<T*>(godot::Memory::alloc_static(n * sizeof(T)));
	}

	void deallocate(T* p, std::size_t)
	{
		godot::Memory::free_static(p);
	}

	template <typename U>
	bool operator==(const GodotAllocator<U>&) const noexcept
	{
		return true;
	}

	template <typename U>
	bool operator!=(const GodotAllocator<U>&) const noexcept
	{
		return false;
	}

	using propagate_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;
};
