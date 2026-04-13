#include "palettized_material.hpp"

// #include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/core/object.hpp>
// #include <godot_cpp/classes/canvas_item.hpp>
#include <algorithm>
#include <godot_cpp/classes/resource_loader.hpp>

#include "../helpers/logger.hpp"

using namespace godot;

namespace experimental
{
	static constexpr const char* shader_source_string = R"(
	shader_type canvas_item;
	
	uniform sampler2D index_tex : source_color, filter_nearest;
	uniform sampler2D palette_tex : source_color, filter_nearest;
	
	// palette could hold multiple palettes on different row
	uniform int palette_row = 0; // pour animations
	// for a single simple animation
	uniform int palette_offset = 0; // cycling manual
	uniform float palette_speed = 0; // cycling auto
	
	void fragment() {
		int index = int(texture(index_tex, UV).r * 256.0);
	
		vec2 pal_size = vec2(textureSize(palette_tex, 0));
		index = int(mod(float(index + palette_offset) + TIME * palette_speed, pal_size.x));
	
		vec2 pal_uv = (vec2(float(index), float(palette_row)) + vec2(0.5)) / pal_size;
	
		COLOR = texture(palette_tex, pal_uv);
	}
	)";

	void PalettizedMaterial::_bind_methods()
	{
		// GDVIRTUAL_BIND(_get_shader_mode);

		ClassDB::bind_method(D_METHOD("set_palettized_image", "image"), &PalettizedMaterial::set_palettized_image);
		ClassDB::bind_method(D_METHOD("get_palettized_image"), &PalettizedMaterial::get_palettized_image);
		// ClassDB::bind_method(D_METHOD("set_palette_texture", "tex"), &PalettizedMaterial::set_palette_texture);
		// ClassDB::bind_method(D_METHOD("get_palette_texture"), &PalettizedMaterial::get_palette_texture);

		// add properties for index and palette textures
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "palettized_image", PROPERTY_HINT_RESOURCE_TYPE, "PalettizedImage"), "set_palettized_image", "get_palettized_image");
		// ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "palette_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_palette_texture", "get_palette_texture");
	}

	void PalettizedMaterial::_validate_property(godot::PropertyInfo &p_property) const
	{
		// hide the "Shader" property from the inspector since it's not meant to be used directly and would be confusing to users
		if (p_property.name.begins_with("shader"))
		{
			p_property.usage = 0; // PROPERTY_USAGE_NOEDITOR
		}
		// we also hide "Shader parameter" properties if we want to force users to use the palettized_image property instead of setting textures manually etc.
		if (p_property.name.begins_with("shader_parameter/"))
		{
			p_property.usage = 0; // PROPERTY_USAGE_NOEDITOR
		}


	}


	PalettizedMaterial::PalettizedMaterial()
	{
		shader.instantiate();
		shader->set_code(shader_source_string);
		set_shader(shader);
	}

	void PalettizedMaterial::set_palettized_image(const godot::Ref<PalettizedImage>& p_image)
	{
		palettized_image = p_image;
		set_shader_parameter("index_tex", p_image.is_valid() ? p_image->get_index_texture() : godot::Ref<godot::ImageTexture>());
		set_shader_parameter("palette_tex", p_image.is_valid() ? p_image->get_palette_texture() : godot::Ref<godot::ImageTexture>());
	}

} // namespace experimental
