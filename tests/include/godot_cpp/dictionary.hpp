#pragma once

#include "variant/string_name.hpp"
#include <map>

namespace godot
{
    struct Variant; // forward

    struct Dictionary
    {
        std::map<StringName, Variant> m;
        Variant& operator[](const StringName& k) { return m[k]; }
        bool is_empty() const { return m.empty(); }
        size_t size() const { return m.size(); }
    };
} // namespace godot
