/* LodePNG version 20200306 simplified single-header inclusion.
   Original author: Lode Vandevenne
   License: zlib/png (permissive). This is a vendored copy used for PNG palette decoding.
*/
#ifndef LODEPNG_H
#define LODEPNG_H

#include <string>
#include <vector>

namespace lodepng
{

	enum ColorType
	{
		LCT_GREY = 0,
		LCT_RGB = 2,
		LCT_PALETTE = 3,
		LCT_GREY_ALPHA = 4,
		LCT_RGBA = 6
	};

	struct InfoColor
	{
		std::vector<unsigned char> palette; /* RGB triplets or RGBA (we'll treat as RGB) */
		unsigned palettesize = 0;
		ColorType colortype = LCT_RGBA;
		unsigned bitdepth = 8;
	};

	struct InfoPNG
	{
		InfoColor color;
	};

	struct State
	{
		InfoPNG info_png;
		struct Decoder
		{
			int color_convert = 1;
		} decoder;
	};

	/* Load file into vector */
	unsigned load_file(std::vector<unsigned char>& out, const std::string& filename);

	/* Inspect PNG and decode raw pixels honoring info_raw when decoder.color_convert==0 */
	unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h, State& state, const unsigned char* in, size_t insize);
	unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h, State& state, const std::vector<unsigned char>& in);

	const char* error_text(unsigned code);

} // namespace lodepng

#endif // LODEPNG_H
