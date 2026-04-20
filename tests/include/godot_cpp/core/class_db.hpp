#pragma once

#include <string>
#include "../typed_array.hpp"
#include "../dictionary.hpp"
#include "../variant/string_name.hpp"

#ifndef D_METHOD
#define D_METHOD(name, ...) name
#endif

namespace godot
{
    struct MethodInfo
    {
        StringName name;
        TypedArray<Dictionary> arguments;
        int flags = 0;
    };

    struct ClassDB
    {
        template <typename F>
        static void bind_method(const char* /*name*/, F /*f*/) {}

        template <typename F>
        static void bind_vararg_method(int /*flags*/, const char* /*name*/, F /*f*/, const MethodInfo& /*mi*/) {}

        static TypedArray<Dictionary> class_get_method_list(const StringName& /*class_name*/) { return TypedArray<Dictionary>(); }
    };
} // namespace godot
