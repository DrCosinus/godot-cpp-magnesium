#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../vendor/doctest.h"

#include "test_palettized_image.hpp"

using namespace experimental;

TEST_CASE("palettized image roundtrip")
{
	// create a 2x2 index image with values 0..3
	godot::PackedByteArray index_data;
	index_data.resize(4);
	index_data[0] = (uint8_t)0;
	index_data[1] = (uint8_t)1;
	index_data[2] = (uint8_t)2;
	index_data[3] = (uint8_t)3; 

	// create a 2x2 palette (RGBA) => 4 pixels * 4 = 16 bytes
	godot::PackedByteArray palette_data;
	palette_data.resize(16);
	for (int i = 0; i < 16; ++i)
		palette_data[i] = (uint8_t)(i * 7 + 1);

	auto img = PalettizedImage::from_raw_data(2, 2, index_data, 2, 2, palette_data);

	REQUIRE(img.is_valid());
	REQUIRE(img.get_index_texture()->get_width() == 2);
	REQUIRE(img.get_index_texture()->get_height() == 2);
	REQUIRE(img.get_palette_texture()->get_width() == 2);
	REQUIRE(img.get_palette_texture()->get_height() == 2);
}
