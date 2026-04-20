#pragma once

#include <vector>

namespace godot
{
    template <typename T>
    struct TypedArray
    {
        std::vector<T> data;
        TypedArray() = default;
        size_t size() const { return data.size(); }
        bool is_empty() const { return data.empty(); }
        void push_back(const T& v) { data.push_back(v); }
        void push_back(T&& v) { data.push_back(std::move(v)); }
        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }
        typename std::vector<T>::iterator begin() { return data.begin(); }
        typename std::vector<T>::iterator end() { return data.end(); }
    };
} // namespace godot
