#include "palettized_image_importer.hpp"

#include "palettized_png_loader.hpp"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

using namespace godot;

namespace experimental
{
	String PalettizedImageImporter::_get_importer_name() const
	{
		return String("palettized_image.importer");
	}

	String PalettizedImageImporter::_get_visible_name() const
	{
		return String("Palettized Image (palette + index texture)");
	}

	int32_t PalettizedImageImporter::_get_preset_count() const
	{
		return 1;
	}

	String PalettizedImageImporter::_get_preset_name(int32_t index) const
	{
		if (index == 0)
			return "Default";
		return "";
	}

	PackedStringArray PalettizedImageImporter::_get_recognized_extensions() const
	{
		// for now let's just support PNG, since it has good lossless compression
		// and supports indexed colors. We can add more formats later if needed.
		static const PackedStringArray exts = {
			"png", // "gif", "bmp", "tga", "jpg"
		};

		return exts;
	}

	TypedArray<Dictionary> PalettizedImageImporter::_get_import_options(const String& path, int32_t preset_index) const
	{
		TypedArray<Dictionary> options;

		Dictionary opt;
		opt["name"] = "dummy_option";
		opt["type"] = Variant::BOOL;
		opt["default_value"] = false;
		options.push_back(opt);

		return options;
	}

	String PalettizedImageImporter::_get_save_extension() const
	{
		return "palimg";
	}

	String PalettizedImageImporter::_get_resource_type() const
	{
		return "PalettizedImage";
	}

	float PalettizedImageImporter::_get_priority() const
	{
		// higher means higher priority
		return 1.0;
	}

	int32_t PalettizedImageImporter::_get_import_order() const
	{
		return IMPORT_ORDER_DEFAULT;
	}

	int32_t PalettizedImageImporter::_get_format_version() const
	{
		return 1;
	}

	bool PalettizedImageImporter::_get_option_visibility(const String& path, const StringName& option_name, const Dictionary& options) const
	{
		return true; // show all options for now
	}

	bool PalettizedImageImporter::_can_import_threaded() const
	{
		return false;
	}

	Error PalettizedImageImporter::_import(const String& source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const
	{
		auto pal_img = experimental::PalettizedPNGLoader::LoadPalettizedImage(source_file);

		if (pal_img.is_null())
		{
			Log.print_error("PalettizedImageImportPlugin: failed to load '%s'.", source_file);
			return Error::FAILED;
		}

		auto index_filename = vformat("%s.%s", p_save_path, _get_save_extension());
		Log.print("Saving palettized texture to '%s' %dx%d (%d colors x %d rows)...",
				  index_filename,
				  pal_img->get_index_texture()->get_width(),
				  pal_img->get_index_texture()->get_height(),
				  pal_img->get_palette_texture()->get_width(),
				  pal_img->get_palette_texture()->get_height());
		auto* resource_saver = ResourceSaver::get_singleton();
		if (auto error = resource_saver->save(pal_img, index_filename); error != Error::OK)
		{
			Log.print_error("PalettizedImageImportPlugin: failed to save '%s'. %d", index_filename, int(error));
			return error;
		}
		/*
				pal_img->get_index_texture()->set_path(index_filename);

				if (auto error = resource_saver->save(pal_img->get_index_texture()); error != Error::OK)
				{
					Log.print_error("PalettizedImageImportPlugin: failed to save '%s'. %d", index_filename, int(error));
					return error;
				}

				auto palette_filename = vformat("%s.palette", p_save_path);
				// Log.print("Saving palette texture to '%s'...", palette_filename);
				if (auto error = resource_saver->save(pal_img->get_palette_texture(), vformat("%s_palette.res", p_save_path)); error != Error::OK)
				{
					Log.print_error("PalettizedImageImportPlugin: failed to save '%s'. %d", palette_filename, int(error));
					return error;
				}
				auto& gen_files = reinterpret_cast<PackedStringArray&>(const_cast<TypedArray<String>&>(p_gen_files));
				gen_files.clear();
				// gen_files.append(index_filename);
				gen_files.append(palette_filename);
		*/
		return Error::OK;
	}

} //namespace experimental
