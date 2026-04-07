#pragma once

#include "../resources/palettized_image.hpp"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/string.hpp>

namespace experimental::PalettizedPNGLoader
{
	godot::Ref<PalettizedImage> LoadPalettizedImage(const godot::String& filename);
} //namespace experimental::PalettizedPNGLoader