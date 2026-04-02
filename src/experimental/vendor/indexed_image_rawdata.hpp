#pragma once

#include <godot_cpp/variant/color.hpp>
#include <vector>

struct IndexedImageRawData
{
	int width;
	int height;
	std::vector<unsigned char> pixel_data;
	std::vector<godot::Color> palette_data;
};