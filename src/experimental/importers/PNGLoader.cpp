#include "PNGLoader.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>
// #include <godot_cpp/classes/image_texture.hpp>
#include "../helpers/read_bytes_stream.hpp"
#include <map>

// https://www.w3.org/TR/png-3/

using namespace godot;

namespace experimental
{
	static constexpr unsigned char PNGIdentifier[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	using ChunkStreamMap = std::map<godot::String, ReadBytesStream>;

	// bit 0: 0 - no palette, 1 - palette
	// bit 1: 0 - grayscale, 1 - color
	// bit 2: 0 - no alpha, 1 - alpha
	enum ColorType : unsigned char
	{
		Grayscale = 0, // depth allowed : 1, 2, 4, 8, 16
		RGB = 2, // depth allowed : 8, 16
		Indexed = 3, // PLTE chunk required... depth allowed : 1, 2, 4, 8
		GrayscaleAlpha = 4, // depth allowed : 8, 16
		RGBA = 6 // depth allowed : 8, 16
	};

	static String GetColorTypeName(ColorType colorType)
	{
		switch (colorType)
		{
			case ColorType::Grayscale:
				return "Grayscale";
			case ColorType::RGB:
				return "RGB";
			case ColorType::Indexed:
				return "Indexed";
			case ColorType::GrayscaleAlpha:
				return "Grayscale with alpha";
			case ColorType::RGBA:
				return "RGBA";
			default:
				return "Unknown";
		}
	}

	// enum RenderingIntent : unsigned char
	// {
	// 	Perceptual = 0, // Perceptual intent is for images preferring good adaptation to the output device gamut at the expense of colorimetric accuracy, like photographs.
	// 	RelativeColorimetric = 1, // Relative colorimetric intent is for images requiring color appearance matching (relative to the output device white point), like logos.
	// 	Saturation = 2, // Saturation intent is for images preferring preservation of saturation at the expense of hue and lightness, like charts and graphs.
	// 	AbsoluteColorimetric = 3 // Absolute colorimetric intent is for images requiring preservation of absolute colorimetry, like proofs (previews of images destined for a different output device).
	// };

	// enum FilterType : unsigned char
	// {
	// 	None = 0,
	// 	Sub = 1,
	// 	Up = 2,
	// 	Average = 3,
	// 	Paeth = 4
	// };

	static ChunkStreamMap ExtractChunkStreamMap(ReadBytesStream& stream)
	{
		ChunkStreamMap chunks;

		// skip the PNG identifier
		stream.advance(sizeof(PNGIdentifier));

		// chunk size(4) + chunk header(4) + ...data.. + chunk checksum(4) = 12 bytes (minimum chunk size)
		while (stream.Position() + 12 <= stream.size())
		{
			// read the chunk length
			int chunkLength = stream.ReadInt32();

			// read the chunk name
			auto name = stream.ReadFixedLengthString(4);
			chunks[name] = stream.slice(chunkLength);

			// advance to past the chunk data and checksum
			stream.advance(chunkLength + 4);
		}
		return chunks;
	}

	namespace PNGLoader
	{
		Ref<PalettizedImage> LoadPalettizedImage(const String& filename)
		{
			PackedByteArray bytes = FileAccess::get_file_as_bytes(filename);
			if (bytes.size() <= sizeof(PNGIdentifier) || memcmp(bytes.ptr(), PNGIdentifier, sizeof(PNGIdentifier)) != 0)
			{
				print_error(vformat("PNG identifier sequence not found in file \"%s\"!", filename));
				return {};
			}

			ReadBytesStream stream{ bytes };

			auto chunks = ExtractChunkStreamMap(stream);

			int width, height, bitDepth;
			ColorType colorType;
			int8_t compressionMethod, filterMethod, interlaceMethod;

			PackedStringArray errors;

			if (auto ihdrChunkIt = chunks.find("IHDR"); ihdrChunkIt == chunks.end())
			{
				errors.push_back("Can not find the IHDR chunk!");
			}
			else
			{
				auto ihdrData = ihdrChunkIt->second;
				width = ihdrData.ReadInt32();
				height = ihdrData.ReadInt32();
				bitDepth = ihdrData.ReadInt8();
				colorType = (ColorType)ihdrData.ReadInt8();
				compressionMethod = ihdrData.ReadInt8(); // for now always 0
				filterMethod = ihdrData.ReadInt8(); // for now always 0
				interlaceMethod = ihdrData.ReadInt8();

				if (colorType != ColorType::Indexed)
				{
					errors.push_back(vformat("Not indexed color image! colorType is %s!", GetColorTypeName(colorType)));
				}
				if (compressionMethod != 0)
				{
					errors.push_back(vformat("Not supported compression method! compressionMethod is %d!", compressionMethod));
				}
				if (filterMethod != 0)
				{
					errors.push_back(vformat("Not supported filter method! filterMethod is %d!", filterMethod));
				}
			}

			// if (auto srgbChunkIt = chunks.find("sRGB"); srgbChunkIt != chunks.end())
			// {
			// 	auto srgbData = srgbChunkIt->second;
			// 	uint8_t renderingIntent = srgbData.ReadInt8();
			// 	print_line(vformat("sRGB chunk: renderingIntent=%d", renderingIntent));
			// }

			PackedByteArray palette;

			if (auto plteChunkIt = chunks.find("PLTE"); plteChunkIt == chunks.end())
			{
				errors.push_back("Can not find the PLTE chunk!");
			}
			else
			{
				auto plteData = plteChunkIt->second;
				if (plteData.size() % 3 != 0)
				{
					errors.push_back("PLTE chunk length is not a multiple of 3!");
				}
				else
				{
					PackedByteArray rgbBytes;
					rgbBytes.resize(3);
					while (plteData.Position() + 3 <= plteData.size())
					{
						palette.push_back(plteData.ReadInt8()); // read the red component
						palette.push_back(plteData.ReadInt8()); // read the green component
						palette.push_back(plteData.ReadInt8()); // read the blue component
						palette.push_back(255); // add an alpha component with full opacity for now, we will update it later if a tRNS chunk is present
					}
				}
			}
			// absent tRNS chunk is not an error, it means all palette colors are fully opaque
			if (auto trnsChunkIt = chunks.find("tRNS"); trnsChunkIt != chunks.end())
			{
				auto trnsData = trnsChunkIt->second;
				if (trnsData.size() != palette.size() / 4)
				{
					errors.push_back("tRNS chunk length does not match palette length!");
				}
				else
				{
					for (int i = 0; i < palette.size(); i += 4)
					{
						palette[i + 3] = trnsData.ReadInt8(); // update the alpha component
					}
				}
			}

			PackedByteArray filteredImageData;
			if (auto idatChunkIt = chunks.find("IDAT"); idatChunkIt != chunks.end())
			{
				auto idatData = idatChunkIt->second.copy();
				// each scanline starts with a filter byte, so we need to account
				// for that in the decompressed size
				filteredImageData = idatData.decompress((width + 1) * height, FileAccess::COMPRESSION_DEFLATE);
			}
			else
			{
				errors.push_back("Can not find image data chunk!");
			}

			PackedByteArray imageData;
			imageData.resize(width * height); // indexed color, so 1 byte per pixel
			auto filteredImageDataStream = ReadBytesStream{ filteredImageData };

			for (int lineIndex = 0; lineIndex < height; ++lineIndex)
			{
				auto scanLineFilter = filteredImageDataStream.ReadInt8();
				if (scanLineFilter != 0)
				{
					errors.push_back(vformat("Unhandled scanline filter value %d, line %d!", scanLineFilter, lineIndex));
				}
				for (int colIndex = 0; colIndex < width; ++colIndex)
				{
					uint8_t colorIndex = filteredImageDataStream.ReadInt8();
					imageData[lineIndex * width + colIndex] = colorIndex;
				}
			}
			if (!errors.is_empty())
			{
				print_error(vformat("Errors found while loading PNG file \"%s\":\n%s", filename, String{ "\n" }.join(errors)));
				return {};
			}

			auto index_img = Image::create_from_data(width, height, false, Image::FORMAT_L8, imageData);
			auto index_tex = ImageTexture::create_from_image(index_img);
			auto palette_img = Image::create_from_data(palette.size() / 4, 1, false, Image::FORMAT_RGBA8, palette);
			auto palette_tex = ImageTexture::create_from_image(palette_img);
			return { memnew(PalettizedImage(std::move(index_tex), std::move(palette_tex))) };
		}
	} //namespace PNGLoader
} //namespace experimental