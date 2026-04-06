#include "palettized_image_saver.hpp"

#include "../resources/palettized_image.hpp"
#include "../write_bytes_stream.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

using namespace godot;

namespace experimental
{
	godot::PackedStringArray PalettizedImageSaver::_get_recognized_extensions(const godot::Ref<godot::Resource>& p_resource) const
	{
		PackedStringArray exts;
		exts.push_back("palimg");
		return exts;
	}

	bool PalettizedImageSaver::_recognize(const Ref<Resource>& p_resource) const
	{
		// print_line("PalettizedImageSaver::_recognize called");
		return Object::cast_to<PalettizedImage>(p_resource.ptr()) != nullptr;
	}

	Error PalettizedImageSaver::_save(const Ref<Resource>& p_resource, const String& p_path, uint32_t p_flags)
	{
		// print_line(vformat("Saving palettized image to %s", p_path));
		auto pal_img = Object::cast_to<PalettizedImage>(p_resource.ptr());
		if (pal_img == nullptr)
		{
			return Error::ERR_INVALID_PARAMETER;
		}

		auto index_texture = pal_img->get_index_texture();
		auto palette_texture = pal_img->get_palette_texture();

		if (index_texture.is_null() || palette_texture.is_null())
		{
			return Error::ERR_INVALID_DATA;
		}

		auto index_image = index_texture->get_image();
		auto palette_image = palette_texture->get_image();

		if (index_image.is_null() || palette_image.is_null())
		{
			return Error::ERR_INVALID_DATA;
		}

		PackedByteArray index_data = index_image->get_data(); // ensure image data is loaded
		PackedByteArray palette_data = palette_image->get_data();

		int64_t width = index_image->get_width();
		int64_t height = index_image->get_height();
		int64_t palette_size = palette_image->get_width(); // RGBA8
		int64_t palette_count = palette_image->get_height();

		PackedByteArray bytes;
		// pre-allocate the exact size we need
		bytes.resize(sizeof(int16_t) * 4 + index_data.size() + palette_data.size());

		WriteBytesStream stream{ bytes };
		const bool success =
				stream.WriteInt16(static_cast<int16_t>(width)) &&
				stream.WriteInt16(static_cast<int16_t>(height)) &&
				stream.WriteBytes(index_data) &&
				stream.WriteInt16(static_cast<int16_t>(palette_size)) &&
				stream.WriteInt16(static_cast<int16_t>(palette_count)) &&
				stream.WriteBytes(palette_data);
		if (!success)
		{
			return Error::ERR_INVALID_DATA;
		}
		Error err;
		auto file = FileAccess::open(p_path, FileAccess::WRITE);
		if (file.is_null())
		{
			return Error::ERR_FILE_CANT_OPEN;
		}

		file->store_buffer(bytes);
		print_line(vformat("Finished saving palettized image to %s", p_path));
		return Error::OK;
	}
} //namespace experimental