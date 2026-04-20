#pragma once

#include "color.hpp"
#include <vector>

namespace godot
{
    struct PackedColorArray
    {
        std::vector<Color> data;
        PackedColorArray() = default;
        size_t size() const { return data.size(); }
        bool is_empty() const { return data.empty(); }
        void resize(size_t n) { data.resize(n); }
        void push_back(const Color& c) { data.push_back(c); }
        Color& operator[](size_t i) { return data[i]; }
        const Color& operator[](size_t i) const { return data[i]; }
    };
} // namespace godot
