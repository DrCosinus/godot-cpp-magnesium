#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include "string.hpp"
#include "string_name.hpp"
#include "packed_byte_array.hpp"
#include "../packed_string_array.hpp"

namespace godot
{
    struct Object;

    struct Variant
    {
        enum Type
        {
            NIL = 0,
            OBJECT,
            STRING,
            STRING_NAME,
            PACKED_BYTE_ARRAY,
            PACKED_STRING_ARRAY
        };

        Type type = NIL;
        std::variant<std::monostate, Object*, String, StringName, PackedByteArray, PackedStringArray> value;

        Variant() = default;
        Variant(Object* o) : type(OBJECT), value(o) {}
        Variant(const String& s) : type(STRING), value(s) {}
        Variant(const StringName& n) : type(STRING_NAME), value(n) {}
        Variant(const PackedByteArray& b) : type(PACKED_BYTE_ARRAY), value(b) {}
        Variant(const PackedStringArray& a) : type(PACKED_STRING_ARRAY), value(a) {}

        Type get_type() const { return type; }

        template <typename T>
        operator T() const
        {
            if constexpr (std::is_same_v<T, Object*>)
            {
                if (type == OBJECT) return std::get<Object*>(value);
                return nullptr;
            }
            if constexpr (std::is_same_v<T, String>)
            {
                if (type == STRING) return std::get<String>(value);
                return String();
            }
            if constexpr (std::is_same_v<T, StringName>)
            {
                if (type == STRING_NAME) return std::get<StringName>(value);
                if (type == STRING) return StringName(std::get<String>(value));
                return StringName();
            }
            if constexpr (std::is_same_v<T, PackedByteArray>)
            {
                if (type == PACKED_BYTE_ARRAY) return std::get<PackedByteArray>(value);
                return PackedByteArray();
            }
            if constexpr (std::is_same_v<T, PackedStringArray>)
            {
                if (type == PACKED_STRING_ARRAY) return std::get<PackedStringArray>(value);
                return PackedStringArray();
            }
            return T();
        }

        static std::string get_type_name(Type t)
        {
            switch (t)
            {
                case NIL: return "Nil";
                case OBJECT: return "Object";
                case STRING: return "String";
                case STRING_NAME: return "StringName";
                case PACKED_BYTE_ARRAY: return "PackedByteArray";
                case PACKED_STRING_ARRAY: return "PackedStringArray";
                default: return "Unknown";
            }
        }
    };
} // namespace godot
