#pragma once

namespace godot
{
	template <typename T>
	struct Ref
	{
		T* ptr = nullptr;
		Ref() = default;
		Ref(T* p) : ptr(p) {}
		Ref(const Ref& o) : ptr(o.ptr) {}
		Ref(Ref&& o) : ptr(o.ptr) { o.ptr = nullptr; }
		Ref& operator=(const Ref& o)
		{
			ptr = o.ptr;
			return *this;
		}
		bool is_null() const { return ptr == nullptr; }
		T* operator->() const { return ptr; }
		T* operator*() const { return ptr; }
	};
} //namespace godot
