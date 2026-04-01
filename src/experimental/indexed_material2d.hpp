#pragma once

#include <godot_cpp/classes/canvas_item_material.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

class IndexedMaterial2D : public godot::CanvasItemMaterial
{
	GDCLASS(IndexedMaterial2D, godot::CanvasItemMaterial);

	godot::Ref<godot::Texture2D> index_texture;
	godot::Ref<godot::Texture2D> palette_texture;

	// palette animation parameters (assuming palette is a horizontal strip of palette_size colors)
	// palette_row is the current row of the palette (0-based)
	// animation (to be replaced with multiple range supports in the future):
	// - palette_offset is the horizontal offset in the palette (0 to palette_size-1)
	// - palette_speed is the speed of the animation in palette entries per second
	float palette_size{ 256.0f };
	float palette_row{ 0.0f };
	float palette_offset{ 0.0f };
	float palette_speed{ 0.0f };

	// godot::RID shader_rid;
	// godot::RID material_rid;

protected:
	static void _bind_methods();

public:
	IndexedMaterial2D();
	~IndexedMaterial2D() override = default;

	// godot::RID _get_rid() const override
	// {
	// 	return material_rid;
	// }

	GDVIRTUAL0RC(godot::Shader::Mode, _get_shader_mode);
	godot::Shader::Mode _get_shader_mode() const override
	{
		return godot::Shader::MODE_CANVAS_ITEM;
	}

	void set_index_texture(const godot::Ref<godot::Texture2D> &p_texture);
	godot::Ref<godot::Texture2D> get_index_texture() const { return index_texture; }
	void set_palette_texture(const godot::Ref<godot::Texture2D> &p_texture);
	godot::Ref<godot::Texture2D> get_palette_texture() const { return palette_texture; }
	// need setters for palette animation parameters as well, but not implemented yet
};

/*
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
// #include <godot_cpp/variant/color.hpp>

class IndexedTexture : public godot::Texture2D
{
	GDCLASS(IndexedTexture, godot::Texture2D)
protected:
	static void _bind_methods();
public:
	IndexedTexture();
	~IndexedTexture() override = default;

	int32_t _get_width() const override
	{
		if (indexed_image.is_null())
			return 0;
		return indexed_image->get_width();
	}

	int32_t _get_height() const override
	{
		if (indexed_image.is_null())
			return 0;
		return indexed_image->get_height();
	}

	bool _is_pixel_opaque(int32_t p_x, int32_t p_y) const override
	{
		if (indexed_image.is_null())
			return false;
		uint8_t index = indexed_image->get_data()[(p_y * indexed_image->get_width() + p_x)];
		return palette_image->get_data()[index * 4 + 3] > 0;
	}

	bool _has_alpha() const override
	{
		if (palette_image.is_null())
			return false;
		for (int64_t i = 0; i < palette_image->get_data_size(); i += 4)
		{
			if (palette_image->get_data()[i + 3] > 0)
				return true;
		}
		return false;
	}

	void _draw(const godot::RID& p_to_canvas_item, const godot::Vector2& p_pos, const godot::Color& p_modulate, bool p_transpose) const override
	{
		if (!baked_texture.is_null())
		{
			baked_texture->draw(p_to_canvas_item, p_pos, p_modulate, p_transpose);
		}
	}

	void _draw_rect(const godot::RID& p_to_canvas_item, const godot::Rect2& p_rect, bool p_tile, const godot::Color& p_modulate, bool p_transpose) const override
	{
		if (!baked_texture.is_null())
		{
			baked_texture->draw_rect(p_to_canvas_item, p_rect, p_tile, p_modulate, p_transpose);
		}
	}

	void _draw_rect_region(const godot::RID& p_to_canvas_item, const godot::Rect2& p_rect, const godot::Rect2& p_src_rect, const godot::Color& p_modulate, bool p_transpose, bool p_clip_uv) const override
	{
		if (!baked_texture.is_null())
		{
			baked_texture->draw_rect_region(p_to_canvas_item, p_rect, p_src_rect, p_modulate, p_transpose, p_clip_uv);
		}
	}

	godot::RID _get_rid() const override
	{
		if (baked_texture.is_null())
			return godot::RID();
		return baked_texture->get_rid();
	}

	void mark_dirty()
	{
		if (!bake_dirty)
		{
			if (bake_on_change)
			{
				emit_signal("bake_requested");
			}
			bake_dirty = true;
		}
	}

private:
	void set_indexed_image(const godot::Ref<godot::Image>& p_indexed_image);
	void set_palette_image(const godot::Ref<godot::Image>& p_palette_image);
	void bake_texture();

	bool bake_dirty{ true };
	bool bake_on_change{ true };
	godot::Ref<godot::Image> palette_image;
	godot::Ref<godot::Image> indexed_image;
	godot::Ref<godot::Image> baked_image;
	godot::Ref<godot::ImageTexture> baked_texture;
};
*/