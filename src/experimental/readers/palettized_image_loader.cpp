#include "palettized_image_loader.hpp"

#include "../helpers/packed_byte_array_reader.hpp"
#include "../resources/palettized_image.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

using namespace godot;

namespace experimental
{
	PackedStringArray PalettizedImageLoader::_get_recognized_extensions() const
	{
		static PackedStringArray exts = {
			"palimg",
		};
		return exts;
	}

	bool PalettizedImageLoader::_handles_type(const StringName& p_type) const
	{
		static StringName expected_type{ "PalettizedImage" };
		return p_type == expected_type;
	}

	Variant PalettizedImageLoader::_load(const String& p_path, const String& p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const
	{
		Log.print("PalettizedImageLoader::_load: loading resource from '%s' (original path: '%s')", p_path, p_original_path);
		// for now we ignore the original path, since we don't have any dependencies to worry about. We can use it later if we want to support loading from a source file (e.g. PNG) instead of a .res file.
		(void)p_original_path;

		PackedByteArray bytes = FileAccess::get_file_as_bytes(p_path);
		PackedByteArrayReader stream{ bytes };

		Ref<PalettizedImage> imgpal;
		imgpal.instantiate();
		imgpal->serialize(stream);
		Log.print("Finished loading palettized image from '%s' %dx%d (%d colors x %d palettes)",
				  p_path,
				  imgpal->get_index_texture().is_null() ? -1 : imgpal->get_index_texture()->get_width(),
				  imgpal->get_index_texture().is_null() ? -1 : imgpal->get_index_texture()->get_height(),
				  imgpal->get_palette_texture().is_null() ? -1 : imgpal->get_palette_texture()->get_width(),
				  imgpal->get_palette_texture().is_null() ? -1 : imgpal->get_palette_texture()->get_height());
		return imgpal;
	}
} //namespace experimental