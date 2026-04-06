#pragma once

#include "../resources/palettized_image.hpp"
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace experimental::PNGLoader
{
	godot::Ref<PalettizedImage> LoadPalettizedImage(const godot::String& filename);
} //namespace experimental::PNGLoader