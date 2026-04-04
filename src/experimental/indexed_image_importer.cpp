#include "indexed_image_importer.hpp"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/os.hpp>
#include "vendor/PNGLoader.hpp"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

IndexedImageImporter::IndexedImageImporter() = default;

String IndexedImageImporter::_get_importer_name() const
{
	return String("indexed_image.importer");
}

String IndexedImageImporter::_get_visible_name() const
{
	return String("Indexed Image (palette + index)");
}

int32_t IndexedImageImporter::_get_preset_count() const
{
	return 1;
}

String IndexedImageImporter::_get_preset_name(int32_t index) const
{
	if (index == 0)
		return "Default";
	return "";
}

PackedStringArray IndexedImageImporter::_get_recognized_extensions() const
{
	PackedStringArray exts;
	exts.push_back("png"); // for now let's just support PNG, since it has good lossless compression and supports indexed colors. We can add more formats later if needed.
	// exts.push_back("gif");
	// exts.push_back("bmp");
	// exts.push_back("tga");
	// exts.push_back("jpg");
	return exts;
}

TypedArray<Dictionary> IndexedImageImporter::_get_import_options(const String& path, int32_t preset_index) const
{
	TypedArray<Dictionary> options;

	Dictionary opt;
	opt["name"] = "dummy_option";
	opt["type"] = Variant::BOOL;
	opt["default_value"] = false;
	options.push_back(opt);

	return options;
}

String IndexedImageImporter::_get_save_extension() const
{
	return "res";
}

String IndexedImageImporter::_get_resource_type() const
{
	return "Texture2D"; // not really sure...
}

float IndexedImageImporter::_get_priority() const
{
	return 1.0; // higher means higher priority
}

int32_t IndexedImageImporter::_get_import_order() const
{
	return IMPORT_ORDER_DEFAULT;
}

int32_t IndexedImageImporter::_get_format_version() const
{
	return 1;
}

bool IndexedImageImporter::_get_option_visibility(const String& path, const StringName& option_name, const Dictionary& options) const
{
	return true; // show all options for now
}

bool IndexedImageImporter::_can_import_threaded() const
{
	return false;
}

Error IndexedImageImporter::_import(const String& source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const
{
	// String src_path = source_file;
	// if (src_path.begins_with("res://"))
	// {
	// 	src_path = ProjectSettings::get_singleton()->globalize_path(src_path);
	// }

	IndexedImageRawData raw = experimental::PNGLoader::LoadRaw(source_file);

	if (raw.index_texture.is_null() || raw.palette_texture.is_null())
	{
		UtilityFunctions::printerr(String("IndexedImageImportPlugin: failed to load '") + source_file + String("'."));
		return Error::FAILED;
	}

	auto index_filename = vformat("%s.%s", p_save_path, _get_save_extension());
	print_line(vformat("Saving index texture to '%s' %dx%d...", index_filename, raw.index_texture->get_width(), raw.index_texture->get_height()	));
	raw.index_texture->set_path(index_filename);
	auto* resource_saver = ResourceSaver::get_singleton();

	if (auto error =resource_saver->save(raw.index_texture); error != Error::OK)
	{
		print_error(vformat("IndexedImageImportPlugin: failed to save '%s'. %d", index_filename, int(error)));
		return error;
	}

	auto palette_filename = vformat("%s.palette", p_save_path);
	// print_line(vformat("Saving palette texture to '%s'...", palette_filename));
	if (auto error = resource_saver->save(raw.palette_texture, vformat("%s_palette.res", p_save_path)); error != Error::OK)
	{
		print_error(vformat("IndexedImageImportPlugin: failed to save '%s'. %d", palette_filename, int(error)));
		return error;
	}
	auto& gen_files = reinterpret_cast<PackedStringArray&>(const_cast<TypedArray<String>&>(p_gen_files));
	gen_files.clear();
	// gen_files.append(index_filename);
	gen_files.append(palette_filename);

	// save the index_image tresource
	// Ref<ResourceSaver> saver = ResourceSaver::get_singleton();
	// String idx_res_path = p_save_path + "_index.tres";
	// Error save_err = saver->save(idx_res_path, raw.index_image);
	// if (save_err != Error::OK)
	// {
	// 	UtilityFunctions::printerr(String("IndexedImageImportPlugin: failed to save

	// print_line(vformat("Loaded source image: %dx%d, format=%d", src->get_width(), src->get_height(), src->get_format()));
	return Error::OK;
	// // Ensure RGBA8 for easier processing
	// if (src->get_format() != Image::FORMAT_RGBA8)
	// {
	// 	src->convert(Image::FORMAT_RGBA8);
	// }

	/*
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

	UtilityFunctions::print(String("IndexedImageImportPlugin: wrote ") + idx_path + String(" and ") + pal_path);*/
	return Error::OK;
}
