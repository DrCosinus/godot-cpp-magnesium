#include "palettized_image_saver.hpp"

#include "../helpers/packed_byte_array_writer.hpp"
// #include "../helpers/write_bytes_stream.hpp"
#include "../resources/palettized_image.hpp"
#include <godot_cpp/classes/file_access.hpp>
// #include <godot_cpp/classes/image.hpp>
// #include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

using namespace godot;

namespace experimental
{
	godot::PackedStringArray PalettizedImageSaver::_get_recognized_extensions(const godot::Ref<godot::Resource>& p_resource) const
	{
		static PackedStringArray exts = {
			"palimg",
		};
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

		PackedByteArray buffer;
		WriteStreamSerializer serializer{ buffer };
		pal_img->serialize(serializer);

		Error err;
		auto file = FileAccess::open(p_path, FileAccess::WRITE);
		if (file.is_null())
		{
			return Error::ERR_FILE_CANT_OPEN;
		}

		file->store_buffer(buffer);
		print_line(vformat("Finished saving palettized image to %s", p_path));
		return Error::OK;
	}
} //namespace experimental