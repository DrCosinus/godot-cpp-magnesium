#pragma once

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/variant/color.hpp>
#include <vector>

struct IndexedImageRawData
{
	// int width{ 0 };
	// int height{ 0 };
	godot::Ref<godot::ImageTexture> index_texture;
	godot::Ref<godot::ImageTexture> palette_texture;
};