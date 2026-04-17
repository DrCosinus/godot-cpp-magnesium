#pragma once

#include "../helpers/logger.hpp"
#include "../helpers/serializer.hpp"

#if defined(TEST_USE_MOCK_GODOT_CPP)
// When running unit tests, the test build will provide mock headers via the include path
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <vector>
#else
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <vector>
#endif

namespace experimental
{
	struct PalettizedImage : public godot::Resource, public Serializable
	{
		GDCLASS(PalettizedImage, godot::Resource)

		inline static Logger<false> Log{ "PalImg" };

	protected:
		static void _bind_methods() {}

	public:
		static PalettizedImage from_raw_data(int16_t width, int16_t height, const godot::PackedByteArray& index_data, int16_t palette_size, int16_t palette_count, const godot::PackedByteArray& palette_data)
		{
			PalettizedImage out;
			if (width > 0 && height > 0 && !index_data.is_empty())
			{
				auto idx_img = godot::Image::create_from_data(width, height, false, godot::Image::FORMAT_L8, index_data);
				out.index_texture = godot::ImageTexture::create_from_image(idx_img);
			}
			else
			{
				out.index_texture = nullptr;
			}
			if (palette_size > 0 && palette_count > 0 && !palette_data.is_empty())
			{
				auto pal_img = godot::Image::create_from_data(palette_size, palette_count, false, godot::Image::FORMAT_RGBA8, palette_data);
				out.palette_texture = godot::ImageTexture::create_from_image(pal_img);
			}
			else
			{
				out.palette_texture = nullptr;
			}
			return out;
		}

		PalettizedImage(godot::Ref<godot::ImageTexture>&& p_index_texture, godot::Ref<godot::ImageTexture>&& p_palette_texture) : index_texture(p_index_texture), palette_texture(p_palette_texture)
		{
		}
		PalettizedImage() = default;
		~PalettizedImage() override = default;
		bool is_valid() const
		{
			return !index_texture.is_null() && !palette_texture.is_null();
		}
		operator bool() const
		{
			return is_valid();
		}

		godot::Ref<godot::ImageTexture> get_index_texture() const
		{
			return index_texture;
		}
		godot::Ref<godot::ImageTexture> get_palette_texture() const
		{
			return palette_texture;
		}

		void serialize(Serializer& serializer) override
		{
			int16_t width;
			int16_t height;
			godot::PackedByteArray index_data;
			int16_t palette_size;
			int16_t palette_count;
			godot::PackedByteArray palette_data;

			if (serializer.IsWriting())
			{
				width = index_texture.is_null() ? -1 : index_texture->get_width();
				height = index_texture.is_null() ? -1 : index_texture->get_height();
				index_data = index_texture.is_null() ? godot::PackedByteArray() : index_texture->get_image()->get_data();
				palette_size = palette_texture.is_null() ? -1 : palette_texture->get_width();
				palette_count = palette_texture.is_null() ? -1 : palette_texture->get_height();
				palette_data = palette_texture.is_null() ? godot::PackedByteArray() : palette_texture->get_image()->get_data();
			}
			// const bool changed = serializer.ProcessInt16("width", width) |
			//      serializer.ProcessInt16("height", height) |
			//      serializer.ProcessPackedByteArray("indexes", index_data) |
			//      serializer.ProcessInt16("palette_size", palette_size) |
			//      serializer.ProcessInt16("palette_count", palette_count) |
			//      serializer.ProcessPackedByteArray("palette", palette_data);
			serializer.ProcessInt16("width", width);
			serializer.ProcessInt16("height", height);
			auto index_data_size = static_cast<size_t>(width) * static_cast<size_t>(height);
			serializer.ProcessPackedByteArray("indexes", index_data, index_data_size); // we can provide the expected size for the packed byte array to read/write, which can be used as an optimization to avoid resizing the array multiple times during reading/writing
			serializer.ProcessInt16("palette_size", palette_size);
			serializer.ProcessInt16("palette_count", palette_count);
			auto palette_data_size = static_cast<size_t>(palette_size) * static_cast<size_t>(palette_count) * 4; // assuming RGBA8 format, 4 bytes per pixel
			serializer.ProcessPackedByteArray("palette", palette_data, palette_data_size); // assuming RGBA8 format, 4 bytes per pixel

			if (!serializer.IsWriting())
			{
				Log.print("Deserialized palettized image: %dx%d, palette size: %dx%d", width, height, palette_size, palette_count);
				if (width > 0 && height > 0 && !index_data.is_empty())
				{
					index_texture = godot::ImageTexture::create_from_image(godot::Image::create_from_data(width, height, false, godot::Image::FORMAT_L8, index_data));
				}
				else
				{
					index_texture = nullptr;
				}
				if (palette_size > 0 && palette_count > 0 && !palette_data.is_empty())
				{
					palette_texture = godot::ImageTexture::create_from_image(godot::Image::create_from_data(palette_size, palette_count, false, godot::Image::FORMAT_RGBA8, palette_data));
				}
				else
				{
					palette_texture = nullptr;
				}
			}
		}

	private:
		godot::Ref<godot::ImageTexture> index_texture;
		godot::Ref<godot::ImageTexture> palette_texture;
	};
} //namespace experimental