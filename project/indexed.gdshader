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