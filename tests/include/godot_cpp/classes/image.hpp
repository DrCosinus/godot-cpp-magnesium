#pragma once

#include "../variant/packed_byte_array.hpp"

namespace godot
{
	struct Image
	{
		static const int FORMAT_L8 = 0;
		static const int FORMAT_RGBA8 = 1;
		int width = 0;
		int height = 0;
		PackedByteArray data;
		static Image create_from_data(int w, int h, bool /*mipmaps*/, int /*fmt*/, const PackedByteArray& arr)
		{
			Image i;
			i.width = w;
			i.height = h;
			i.data = arr;
			return i;
		}
		PackedByteArray get_data() const { return data; }
		int get_width() const { return width; }
		int get_height() const { return height; }
	};
} //namespace godot
