#pragma once

#include "variant/string.hpp"
#include <vector>

namespace godot
{
    struct PackedStringArray
    {
        std::vector<String> data;
        PackedStringArray() = default;
        size_t size() const { return data.size(); }
        bool is_empty() const { return data.empty(); }
        void resize(size_t n) { data.resize(n); }
        void push_back(const String& s) { data.push_back(s); }
        String& operator[](size_t i) { return data[i]; }
        const String& operator[](size_t i) const { return data[i]; }
    };
} // namespace godot
