#pragma once

#include "../helpers/logger.hpp"
#include "palettized_image.hpp"
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/rid.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace experimental
{
	class PalettizedMaterial : public godot::ShaderMaterial
	{
		GDCLASS(PalettizedMaterial, godot::ShaderMaterial);

		inline static Logger<true> Log{ "PalMat" };

	public:
		PalettizedMaterial();
		~PalettizedMaterial() override = default;

		void _validate_property(godot::PropertyInfo& p_property) const;

		godot::Shader::Mode _get_shader_mode() const override
		{
			return godot::Shader::MODE_CANVAS_ITEM;
		}

		void set_palettized_image(const godot::Ref<PalettizedImage>& p_image);
		godot::Ref<PalettizedImage> get_palettized_image() const
		{
			return palettized_image;
		}

	protected:
		static void _bind_methods();

	private:
		godot::Ref<PalettizedImage> palettized_image;
		godot::Ref<godot::Shader> shader;

		// palette animation parameters (assuming palette is a horizontal strip of palette_size colors)
		// palette_row is the current row of the palette (0-based)
		// animation (to be replaced with multiple range supports in the future):
		// - palette_offset is the horizontal offset in the palette (0 to palette_size-1)
		// - palette_speed is the speed of the animation in palette entries per second
		float palette_size{ 256.0f };
		float palette_row{ 0.0f };
		float palette_offset{ 0.0f };
		float palette_speed{ 0.0f };
	};
} //namespace experimental
