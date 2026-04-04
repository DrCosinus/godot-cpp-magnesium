#include "PNGLoader.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>
#include <godot_cpp/classes/image.hpp>
// #include <godot_cpp/classes/image_texture.hpp>

// https://www.w3.org/TR/png-3/

using namespace godot;

namespace experimental
{
	IndexedImageRawData PNGLoader::LoadRaw(const String& filename)
	{
		// print_line(vformat("Loading PNG file \"%s\"...", filename));

		PackedByteArray bytes = FileAccess::get_file_as_bytes(filename);
		if (bytes.size() <= sizeof(PNGIdentifier) || memcmp(bytes.ptr(), PNGIdentifier, sizeof(PNGIdentifier)) != 0)
		{
			print_error(vformat("PNG identifier sequence not found in file \"%s\"!", filename));
			return {};
		}
		// print_line(vformat("PNG identifier sequence found in file \"%s\"!", filename));

		readonly_bytes_stream stream{ bytes };

		auto chunks = IndexChunks(stream);

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
			// print_line(vformat("IHDR chunk: width=%d, height=%d, bitDepth=%d, colorType=%s, compressionMethod=%d, filterMethod=%d, interlaceMethod=%d", width, height, bitDepth, GetColorTypeName(colorType), compressionMethod, filterMethod, interlaceMethod));

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
					// for (int i = 0; i < 3; ++i)
					// 	rgbBytes[i] = plteData.ReadInt8();
					// palette.push_back(Color::from_rgba8(rgbBytes[0], rgbBytes[1], rgbBytes[2]));
					// print_line(vformat("Read palette color: #%02X%02X%02X", rgbBytes[0], rgbBytes[1], rgbBytes[2]));
				}
			}
		}
		// absent tRNS chunk means all palette colors are fully opaque
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
					// print_line(vformat("Read palette color with alpha: #%02X%02X%02X%02X", palette[i], palette[i + 1], palette[i + 2], palette[i + 3]));
				}
			}
		}

		PackedByteArray filteredImageData;
		if (auto idatChunkIt = chunks.find("IDAT"); idatChunkIt != chunks.end())
		{
			auto idatData = idatChunkIt->second.copy();
			filteredImageData = idatData.decompress(width * height + height, FileAccess::COMPRESSION_DEFLATE); // each scanline starts with a filter byte, so we need to account for that in the decompressed size
		}
		else
		{
			errors.push_back("Can not find image data chunk!");
		}

		PackedByteArray imageData;
		imageData.resize(width * height); // indexed color, so 1 byte per pixel
		auto filteredImageDataStream = readonly_bytes_stream{ filteredImageData };

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
		// print_line(vformat("Image data size: %d bytes, filtered image data size: %d bytes", imageData.size(), filteredImageData.size()));
		// print_line(vformat("Palette size: %d colors", palette.size()));
		// print_line(vformat("Finished processing image data, total pixels: %d", width * height));
		auto index_img = Image::create_from_data(width, height, false, Image::FORMAT_L8, imageData);
		// print_line(vformat("Created index image: %dx%d, format=%d", index_img->get_width(), index_img->get_height(), index_img->get_format()));
		auto palette_img = Image::create_from_data(palette.size() / 4, 1, false, Image::FORMAT_RGBA8, palette);
		// print_line(vformat("Created palette image: %dx%d, format=%d", palette_img->get_width(), palette_img->get_height(), palette_img->get_format()));
		if (!errors.is_empty())
		{
			print_error(vformat("Errors found while loading PNG file \"%s\":\n%s", filename, String{ "\n" }.join(errors)));
			return {};
		}

		return { ImageTexture::create_from_image(index_img), ImageTexture::create_from_image(palette_img)};
	}

	std::map<godot::String, readonly_bytes_stream> PNGLoader::IndexChunks(readonly_bytes_stream& stream /*data*/)
	{
		std::map<godot::String, readonly_bytes_stream> chunks;
		// using ReadOnlyMemoryViewStream<byte> stream = new (data);

		// skip the PNG identifier
		stream.advance(sizeof(PNGIdentifier));

		// Span<byte> buffer = stackalloc byte[4];

		// chunk size + chunk header + chunk checksum = 12 bytes (minimum chunk size)
		while (stream.Position() + 12 <= stream.size())
		{
			// read the chunk length
			int chunkLength = stream.ReadInt32();

			// read the chunk name
			auto name = stream.ReadString(4);
			//print_line(vformat("Found chunk %s with length %d (%d bytes remaining in file)", name, chunkLength, stream.size() - stream.Position()));
			// var chunkContent = data.Slice((int)stream.Position(), chunkLength);
			// chunks.Add(name, chunkContent);
			chunks[name] = stream.slice(chunkLength);

			// // skip the chunk data and checksum
			stream.advance(chunkLength + 4);
		}
		return chunks;
	}
} //namespace experimental