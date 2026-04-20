#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../vendor/doctest.h"

#include "../include/godot_cpp/variant/string.hpp"
#include "../include/godot_cpp/variant/string_name.hpp"
#include "../include/godot_cpp/variant/packed_byte_array.hpp"
#include "../include/godot_cpp/variant/packed_color_array.hpp"
#include "../include/godot_cpp/packed_string_array.hpp"
#include "../include/godot_cpp/typed_array.hpp"
#include "../include/godot_cpp/dictionary.hpp"
#include "../include/godot_cpp/variant/variant.hpp"
#include "../include/godot_cpp/classes/image.hpp"
#include "../include/godot_cpp/classes/image_texture.hpp"
#include "../include/godot_cpp/classes/ref.hpp"
#include "../include/godot_cpp/core/class_db.hpp"
#include "../include/godot_cpp/classes/rendering_server.hpp"

using namespace godot;

TEST_CASE("String and StringName basic")
{
    String s("hello");
    REQUIRE(s.as_std() == "hello");

    String part1("a"), part2("b");
    std::vector<String> parts{part1, part2};
    String joined = String::join(parts, String("/"));
    REQUIRE(joined.as_std() == "a/b");

    StringName sn("my_name");
    REQUIRE(static_cast<String>(sn).as_std() == "my_name");
}

TEST_CASE("PackedByteArray basic")
{
    PackedByteArray b;
    b.resize(3);
    b[0] = 1; b[1] = 2; b[2] = 3;
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
}

TEST_CASE("PackedColorArray and Color")
{
    PackedColorArray colors;
    colors.push_back(Color(0.1f, 0.2f, 0.3f, 0.4f));
    REQUIRE(colors.size() == 1);
    auto c = colors[0];
    REQUIRE(c.a == 0.4f);
}

TEST_CASE("PackedStringArray and TypedArray")
{
    PackedStringArray psa;
    psa.push_back(String("one"));
    psa.push_back(String("two"));
    REQUIRE(psa.size() == 2);

    TypedArray<StringName> ta;
    ta.push_back(StringName("m1"));
    ta.push_back(StringName("m2"));
    REQUIRE(ta.size() == 2);
}

TEST_CASE("Dictionary and Variant")
{
    Dictionary d;
    d[StringName("k")] = Variant(String("v"));

    Variant v = d[StringName("k")];
    auto vt = v.get_type();
    bool is_stringish = (vt == Variant::STRING) || (vt == Variant::STRING_NAME);
    REQUIRE(is_stringish);

    Variant s = String("abc");
    REQUIRE(Variant::get_type_name(s.get_type()) == "String");
}

TEST_CASE("Image / ImageTexture / Ref mocks")
{
    // create a simple 2x2 image
    PackedByteArray data;
    data.resize(4);
    for (size_t i = 0; i < 4; ++i) data[i] = (unsigned char)i;

    Image img = Image::create_from_data(2, 2, false, Image::FORMAT_L8, data);
    REQUIRE(img.get_width() == 2);
    REQUIRE(img.get_height() == 2);

    auto tex = ImageTexture::create_from_image(img);
    REQUIRE(!tex.is_null());
    REQUIRE(tex->get_width() == 2);
}

TEST_CASE("ClassDB and RenderingServer presence")
{
    // class_get_method_list should be callable (returns empty TypedArray by default)
    auto methods = ClassDB::class_get_method_list(StringName("NonExistent"));
    REQUIRE(methods.is_empty());

    // RenderingServer type exists
    RenderingServer rs;
    (void)rs;
}
