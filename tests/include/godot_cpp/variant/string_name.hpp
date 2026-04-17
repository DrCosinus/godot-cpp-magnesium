#pragma once

#include "string.hpp"

namespace godot
{
    struct StringName
    {
        String s;
        StringName() = default;
        StringName(const char* c) : s(c) {}
        StringName(const String& o) : s(o) {}
        bool operator==(const StringName& o) const { return s.s == o.s.s; }
        bool operator<(const StringName& o) const { return s.s < o.s.s; }
        operator String() const { return s; }
    };
} // namespace godot
