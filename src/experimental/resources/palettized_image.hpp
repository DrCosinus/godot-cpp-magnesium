#pragma once

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <vector>

namespace experimental
{
	struct PalettizedImage : public godot::Resource
	{
		GDCLASS(PalettizedImage, godot::Resource)
	protected:
		static void _bind_methods() {}

	public:
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

	private:
		godot::Ref<godot::ImageTexture> index_texture;
		godot::Ref<godot::ImageTexture> palette_texture;
	};
} //namespace experimental