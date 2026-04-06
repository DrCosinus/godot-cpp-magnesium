#include "palettized_image_loader.hpp"

#include "../readonly_bytes_stream.hpp"
#include "palettized_image.hpp"
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
		// exts.push_back("palimg");
		return exts;
	}

	bool PalettizedImageLoader::_handles_type(const StringName& p_type) const
	{
		print_line(vformat("PalettizedImageLoader::_handles_type: checking if '%s' is a type we handle", p_type));
		static StringName expected_type{ "PalettizedImage" };
		return p_type == expected_type;
	}

	Variant PalettizedImageLoader::_load(const String& p_path, const String& p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const
	{
		print_line(vformat("PalettizedImageLoader::_load: loading resource from '%s' (original path: '%s')", p_path, p_original_path));
		// for now we ignore the original path, since we don't have any dependencies to worry about. We can use it later if we want to support loading from a source file (e.g. PNG) instead of a .res file.
		(void)p_original_path;

		PackedByteArray bytes = FileAccess::get_file_as_bytes(p_path);
		readonly_bytes_stream stream{ bytes };

		const auto width{ stream.ReadInt16() };
		const auto height{ stream.ReadInt16() };
		print_line(vformat("PalettizedImageLoader::_load: image size is %dx%d", width, height));
		const auto data = stream.ReadBytes(width * height);
		auto index_texture = ImageTexture::create_from_image(Image::create_from_data(width, height, false, Image::FORMAT_L8, data));
		const auto palette_size{ stream.ReadInt16() };
		const auto palette_count{ stream.ReadInt16() };
		print_line(vformat("PalettizedImageLoader::_load: %d palette(s) with %d colors", palette_count, palette_size));
		const auto palette = stream.ReadBytes(palette_count * palette_size * 4);
		auto palette_texture = ImageTexture::create_from_image(Image::create_from_data(palette_size, palette_count, false, Image::FORMAT_RGBA8, palette));
		return Ref<PalettizedImage>(memnew(PalettizedImage(std::move(index_texture), std::move(palette_texture))));
	}
} //namespace experimental