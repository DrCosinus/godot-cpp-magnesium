#include "palettized_material.hpp"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/core/object.hpp>
// #include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include "../helpers/logger.hpp"

using namespace godot;

namespace experimental
{
	void PalettizedMaterial::init_shaders()
	{
		// create shader and material
		Log.print("PalettizedMaterial::init_shaders: creating shader and material");
	}

	void PalettizedMaterial::finish_shaders()
	{
		// free shader and material
		Log.print("PalettizedMaterial::finish_shaders: freeing shader and material");
	}

	void PalettizedMaterial::_bind_methods()
	{
		GDVIRTUAL_BIND(_get_shader_mode);

		ClassDB::bind_method(D_METHOD("set_palettized_image", "image"), &PalettizedMaterial::set_palettized_image);
		ClassDB::bind_method(D_METHOD("get_palettized_image"), &PalettizedMaterial::get_palettized_image);
		// ClassDB::bind_method(D_METHOD("set_palette_texture", "tex"), &PalettizedMaterial::set_palette_texture);
		// ClassDB::bind_method(D_METHOD("get_palette_texture"), &PalettizedMaterial::get_palette_texture);

		// add properties for index and palette textures
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "palettized_image", PROPERTY_HINT_RESOURCE_TYPE, "PalettizedImage"), "set_palettized_image", "get_palettized_image");
		// ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "palette_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_palette_texture", "get_palette_texture");
	}

	PalettizedMaterial::PalettizedMaterial()
	{
		material_rid = RenderingServer::get_singleton()->material_create();

		current_key.invalid_key = 1; // force shader update on first use

		update_shader();
	}

	void PalettizedMaterial::_mark_initialized(const Callable& p_add_to_dirty_list, const Callable& p_update_shader)
	{
		//if (ResourceLoader::is_within_load())
		// {
		// 	DEV_ASSERT(init_state != INIT_STATE_READY);
		// }
	}

	void PalettizedMaterial::update_shader()
	{
		MaterialKey mk = _compute_key();
		if (mk == current_key)
			return;

		if (shader_map.has(current_key))
		{
			Log.print("PalettizedMaterial::update_shader: current shader users before decrement: %d", shader_map[current_key].users);
			shader_map[current_key].users--;
			if (shader_map[current_key].users <= 0)
			{
				// free shader
				Log.print("PalettizedMaterial::update_shader: freeing shader for key: %d", current_key.key);
				RenderingServer::get_singleton()->free_rid(shader_map[current_key].shader);
				shader_map.erase(current_key);
			}
		}

		current_key = mk;

		if (shader_map.has(mk))
		{
			Log.print("PalettizedMaterial::update_shader: reusing existing shader for key: %d", current_key.key);
			RenderingServer::get_singleton()->material_set_shader(material_rid, shader_map[mk].shader);
			shader_map[mk].users++;
			return;
		}
		constexpr const char* shader_source_string = R"(
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

		Log.print("PalettizedMaterial::update_shader: creating new shader for key: %d", current_key.key);
		ShaderData sd;
		sd.shader = RenderingServer::get_singleton()->shader_create();
		sd.users = 1;
		RenderingServer::get_singleton()->shader_set_code(sd.shader, shader_source_string);
		shader_map[mk] = sd;
		RenderingServer::get_singleton()->material_set_shader(material_rid, sd.shader);
	}

	void PalettizedMaterial::set_palettized_image(const godot::Ref<PalettizedImage>& p_image)
	{
		palettized_image = p_image;
		if (palettized_image.is_null())
			return;
		// set textures to shader parameters
		Log.print("PalettizedMaterial::set_palettized_image: setting index and palette textures to material");
		RenderingServer::get_singleton()->material_set_param(get_rid(), "index_tex", palettized_image->get_index_texture()->get_rid());
		RenderingServer::get_singleton()->material_set_param(get_rid(), "palette_tex", palettized_image->get_palette_texture()->get_rid());
	}
	// {
	// 	index_texture = p_texture;
	// 	// RenderingServer::get_singleton()->material_set_param(get_rid(), "index_texture", index_texture);
	// }

	// void PalettizedMaterial::set_palette_texture(const godot::Ref<godot::Texture2D>& p_texture)
	// {
	// 	palette_texture = p_texture;
	// 	// RenderingServer::get_singleton()->material_set_param(get_rid(), "palette_texture", palette_texture);
	// }
} //namespace experimental
/*
// setter --> emit bake_requested (only if not yet dirty) --> bake_texture (deferred) --> on_baked --> emit_changed

void IndexedTexture::_bind_methods()
{
	ADD_SIGNAL(godot::MethodInfo("bake_requested"));
}

IndexedTexture::IndexedTexture()
{
	connect("bake_requested", callable_mp(this, &IndexedTexture::bake_texture), CONNECT_DEFERRED);
}
// IndexedTexture::~IndexedTexture() = default;

void IndexedTexture::set_palette_image(const godot::Ref<godot::Image>& p_palette_image)
{
	palette_image = p_palette_image;
	if (palette_image->get_format() != godot::Image::FORMAT_RGBA8)
	{
		godot::UtilityFunctions::printerr("IndexedTexture: palette_image must be in FORMAT_RGBA8");
		return;
	}
	if (palette_image->get_width() < 256)
	{
		godot::UtilityFunctions::printerr("IndexedTexture: palette_image must be at least 256 pixels wide");
		return;
	}
	mark_dirty();
}

void IndexedTexture::set_indexed_image(const godot::Ref<godot::Image>& p_indexed_image)
{
	indexed_image = p_indexed_image;
	if (indexed_image->get_format() != godot::Image::FORMAT_R8)
	{
		godot::UtilityFunctions::printerr("IndexedTexture: indexed_image must be in FORMAT_R8");
		return;
	}
	// create baked texture with same size as indexed image and RGBA8 format
	baked_image = godot::Image::create_empty(indexed_image->get_width(), indexed_image->get_height(), false, godot::Image::FORMAT_RGBA8);
	baked_texture = godot::ImageTexture::create_from_image(baked_image);
	mark_dirty();
}

void IndexedTexture::bake_texture()
{
	if (!bake_dirty)
		return;
	if (indexed_image.is_null() || palette_image.is_null())
		return;
	// retrieve color data from palette image
	godot::PackedByteArray palette_data = palette_image->get_data();
	// retrieve index data from indexed image
	godot::PackedByteArray indexed_data = indexed_image->get_data();
	// create new byte array for baked texture
	godot::PackedByteArray baked_data;
	baked_data.resize(indexed_data.size() * 4);
	// for each pixel in indexed image, look up color in palette and write to baked data
	for (int64_t i = 0; i < indexed_data.size(); i++)
	{
		uint8_t index = indexed_data[i];
		baked_data[i * 4 + 0] = palette_data[index * 4 + 0];
		baked_data[i * 4 + 1] = palette_data[index * 4 + 1];
		baked_data[i * 4 + 2] = palette_data[index * 4 + 2];
		baked_data[i * 4 + 3] = palette_data[index * 4 + 3];
	}
	// set baked data to baked texture (baked_image is mutable)
	baked_image->set_data(indexed_image->get_width(), indexed_image->get_height(), false, godot::Image::FORMAT_RGBA8, baked_data);
	baked_texture->update(baked_image);
	bake_dirty = false;
	if (bake_on_change)
	{
		emit_changed();
	}
}



void plop(CanvasItem* item, IndexedMaterial2D* material)
{
	RenderingServer::get_singleton()->canvas_item_set_material(item->get_canvas_item(), material->get_rid());
}
*/