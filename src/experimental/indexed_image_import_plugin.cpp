#include "indexed_image_import_plugin.hpp"

#include "vendor/lodepng.h"
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/os.hpp>
// #include <godot_cpp/core/ustring.hpp>
// #include <godot_cpp/core/utility_functions.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

using namespace godot;

void IndexedImageImportPlugin::_bind_methods()
{
	// no methods to bind for now
}

IndexedImageImportPlugin::IndexedImageImportPlugin()
{
}

String IndexedImageImportPlugin::_get_importer_name() const
{
	return String("indexed_image");
}

String IndexedImageImportPlugin::_get_visible_name() const
{
	return String("Indexed Image (palette + index)");
}

PackedStringArray IndexedImageImportPlugin::_get_recognized_extensions() const
{
	PackedStringArray exts;
	exts.push_back(String("png"));
	// exts.push_back(String("gif"));
	// exts.push_back(String("bmp"));
	// exts.push_back(String("tga"));
	// exts.push_back(String("jpg"));
	return exts;
}

String IndexedImageImportPlugin::_get_save_extension() const
{
	// We don't produce a single Godot resource; keep a neutral extension.
	return String("indexed");
}

String IndexedImageImportPlugin::_get_resource_type() const
{
	return String("Resource");
}

Error IndexedImageImportPlugin::_import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const
{
	// Pure C++ importer: load image via Godot Image, build a palette (up to 256 colors)
	using namespace godot;

	Ref<Image> src = Image::load_from_file(p_source_file);
	if (src.is_null())
	{
		UtilityFunctions::printerr(String("IndexedImageImportPlugin: failed to load '") + p_source_file + String("'."));
		return Error::FAILED;
	}

	// Ensure RGBA8 for easier processing
	if (src->get_format() != Image::FORMAT_RGBA8)
	{
		src->convert(Image::FORMAT_RGBA8);
	}

	int width = src->get_width();
	int height = src->get_height();

	PackedByteArray src_data = src->get_data();
	int pixel_count = width * height;

	// Build palette: map color (RGBA32) -> index
	std::vector<uint32_t> palette;
	palette.reserve(256);

	PackedByteArray index_bytes;
	index_bytes.resize(pixel_count);

	auto get_u32 = [&](int px) -> uint32_t {
		int off = px * 4;
		uint8_t r = (uint8_t)src_data[off + 0];
		uint8_t g = (uint8_t)src_data[off + 1];
		uint8_t b = (uint8_t)src_data[off + 2];
		uint8_t a = (uint8_t)src_data[off + 3];
		return (uint32_t(r) << 24) | (uint32_t(g) << 16) | (uint32_t(b) << 8) | uint32_t(a);
	};

	auto color_dist_sq = [](uint32_t c1, uint32_t c2) -> uint32_t {
		int r1 = (c1 >> 24) & 0xFF;
		int g1 = (c1 >> 16) & 0xFF;
		int b1 = (c1 >> 8) & 0xFF;
		int r2 = (c2 >> 24) & 0xFF;
		int g2 = (c2 >> 16) & 0xFF;
		int b2 = (c2 >> 8) & 0xFF;
		int dr = r1 - r2;
		int dg = g1 - g2;
		int db = b1 - b2;
		return uint32_t(dr * dr + dg * dg + db * db);
	};

	for (int i = 0; i < pixel_count; ++i)
	{
		uint32_t col = get_u32(i);

		// search palette
		int found = -1;
		for (size_t pi = 0; pi < palette.size(); ++pi)
		{
			if (palette[pi] == col)
			{
				found = int(pi);
				break;
			}
		}
		if (found == -1)
		{
			if (palette.size() < 256)
			{
				palette.push_back(col);
				found = (int)palette.size() - 1;
			}
			else
			{
				// find nearest color
				uint32_t bestd = 0xFFFFFFFFu;
				int besti = 0;
				for (size_t pi = 0; pi < palette.size(); ++pi)
				{
					uint32_t d = color_dist_sq(col, palette[pi]);
					if (d < bestd)
					{
						bestd = d;
						besti = int(pi);
					}
				}
				found = besti;
			}
		}
		index_bytes[i] = uint8_t(found & 0xFF);
	}

	// Build palette image (1 x palette_size) RGBA8
	int pal_size = (int)palette.size();
	Ref<Image> pal_img = Image::create(pal_size, 1, false, Image::FORMAT_RGBA8);
	PackedByteArray pal_data;
	pal_data.resize(pal_size * 4);
	for (int i = 0; i < pal_size; ++i)
	{
		uint32_t c = palette[i];
		int off = i * 4;
		pal_data[off + 0] = uint8_t((c >> 24) & 0xFF);
		pal_data[off + 1] = uint8_t((c >> 16) & 0xFF);
		pal_data[off + 2] = uint8_t((c >> 8) & 0xFF);
		pal_data[off + 3] = uint8_t((c) & 0xFF);
	}
	pal_img->set_data(pal_size, 1, false, Image::FORMAT_RGBA8, pal_data);

	// Build index image R8
	Ref<Image> idx_img = Image::create(width, height, false, Image::FORMAT_R8);
	idx_img->set_data(width, height, false, Image::FORMAT_R8, index_bytes);

	// Save outputs into p_save_path folder
	String base = p_save_path.rstrip("/");
	String idx_path = base + String("/") + String(p_source_file.get_file().get_basename()) + String("_index.png");
	String pal_path = base + String("/") + String(p_source_file.get_file().get_basename()) + String("_palette.png");

	Error e1 = idx_img->save_png(idx_path);
	Error e2 = pal_img->save_png(pal_path);

	if (e1 != Error::OK || e2 != Error::OK)
	{
		UtilityFunctions::printerr(String("IndexedImageImportPlugin: failed to save generated images: ") + String::num_int64((int)e1) + String(" ") + String::num_int64((int)e2));
		return Error::FAILED;
	}

	UtilityFunctions::print(String("IndexedImageImportPlugin: wrote ") + idx_path + String(" and ") + pal_path);
	return Error::OK;
}
