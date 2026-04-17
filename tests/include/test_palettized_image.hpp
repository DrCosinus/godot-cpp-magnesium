#pragma once

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/image_texture.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include <cstdint>

namespace experimental
{
	struct PalettizedImage
	{
		godot::Ref<godot::ImageTexture> index_texture;
		godot::Ref<godot::ImageTexture> palette_texture;

		static PalettizedImage from_raw_data(int16_t width, int16_t height, const godot::PackedByteArray& index_data, int16_t palette_size, int16_t palette_count, const godot::PackedByteArray& palette_data)
		{
			PalettizedImage out;
			if (width > 0 && height > 0 && !index_data.is_empty())
			{
				auto idx_img = godot::Image::create_from_data(width, height, false, godot::Image::FORMAT_L8, index_data);
				out.index_texture = godot::ImageTexture::create_from_image(idx_img);
			}
			if (palette_size > 0 && palette_count > 0 && !palette_data.is_empty())
			{
				auto pal_img = godot::Image::create_from_data(palette_size, palette_count, false, godot::Image::FORMAT_RGBA8, palette_data);
				out.palette_texture = godot::ImageTexture::create_from_image(pal_img);
			}
			return out;
		}

		bool is_valid() const { return !index_texture.is_null() && !palette_texture.is_null(); }
		godot::Ref<godot::ImageTexture> get_index_texture() const { return index_texture; }
		godot::Ref<godot::ImageTexture> get_palette_texture() const { return palette_texture; }
	};
} //namespace experimental
