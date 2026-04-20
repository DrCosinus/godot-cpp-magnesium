#pragma once

#include <string>
#include <vector>

namespace godot
{
    struct String
    {
        std::string s;
        String() = default;
        String(const char* c) : s(c ? c : "") {}
        String(const std::string& o) : s(o) {}
        static String utf8(const char* data, long long len)
        {
            return String(std::string(data, data + (len > 0 ? static_cast<size_t>(len) : 0)));
        }
        bool operator==(const String& o) const { return s == o.s; }
        operator std::string() const { return s; }
        std::string as_std() const { return s; }
        std::string to_std() const { return s; }

        static String join(const std::vector<String>& parts, const String& sep = String(", "))
        {
            std::string out;
            for (size_t i = 0; i < parts.size(); ++i)
            {
                if (i) out += sep.s;
                out += parts[i].s;
            }
            return String(out);
        }

        String join(const std::vector<String>& parts) const { return join(parts, *this); }
    };
} // namespace godot
