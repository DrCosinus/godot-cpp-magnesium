#include "indexed_material2d.hpp"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/core/object.hpp>
// #include <godot_cpp/classes/canvas_item.hpp>

using namespace godot;

void IndexedMaterial2D::_bind_methods()
{
	GDVIRTUAL_BIND(_get_shader_mode);

	ClassDB::bind_method(D_METHOD("set_index_texture", "tex"), &IndexedMaterial2D::set_index_texture);
	ClassDB::bind_method(D_METHOD("get_index_texture"), &IndexedMaterial2D::get_index_texture);
	ClassDB::bind_method(D_METHOD("set_palette_texture", "tex"), &IndexedMaterial2D::set_palette_texture);
	ClassDB::bind_method(D_METHOD("get_palette_texture"), &IndexedMaterial2D::get_palette_texture);

	// add properties for index and palette textures
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "index_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_index_texture", "get_index_texture");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "palette_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_palette_texture", "get_palette_texture");
}

IndexedMaterial2D::IndexedMaterial2D()
{
	constexpr const char* shader_source_string = R"(
shader_type canvas_item;

uniform sampler2D index_tex : source_color;
uniform sampler2D palette_tex : source_color;

uniform float palette_size = 256.0;
uniform float palette_row = 0.0; // pour animations
uniform float palette_offset = 0.0; // cycling
uniform float palette_speed = 0.0; // cycling auto

void fragment() {
    float index = texture(index_tex, UV).r * palette_size;

    index = mod(index + palette_offset + TIME * palette_speed, palette_size);

    float u = (index + 0.5) / palette_size;
    float v = (palette_row + 0.5) / float(textureSize(palette_tex, 0).y);

    COLOR = texture(palette_tex, vec2(u, v));
}
)";

	// RID shader_rid = RenderingServer::get_singleton()->shader_create();
	// RenderingServer::get_singleton()->shader_set_code(shader_rid, shader_source_string);

	// RID material_rid = RenderingServer::get_singleton()->material_create();
	// RenderingServer::get_singleton()->material_set_shader(material_rid, shader_rid);
}

void IndexedMaterial2D::set_index_texture(const godot::Ref<godot::Texture2D>& p_texture)
{
	index_texture = p_texture;
	// RenderingServer::get_singleton()->material_set_param(get_rid(), "index_texture", index_texture);
}

void IndexedMaterial2D::set_palette_texture(const godot::Ref<godot::Texture2D>& p_texture)
{
	palette_texture = p_texture;
	// RenderingServer::get_singleton()->material_set_param(get_rid(), "palette_texture", palette_texture);
}

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