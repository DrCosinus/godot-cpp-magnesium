#pragma once

#include "image.hpp"
#include "ref.hpp"

namespace godot
{
	struct ImageTexture
	{
		Image img;
		ImageTexture() = default;
		ImageTexture(const Image& i) : img(i) {}
		static Ref<ImageTexture> create_from_image(const Image& i) { return Ref<ImageTexture>(new ImageTexture(i)); }
		Image* get_image() { return &img; }
		int get_width() const { return img.get_width(); }
		int get_height() const { return img.get_height(); }
	};
} //namespace godot
