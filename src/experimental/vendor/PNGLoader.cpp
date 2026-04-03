#include "PNGLoader.hpp"

#include <godot_cpp/classes/file_access.hpp>

// https://www.w3.org/TR/png-3/

using namespace godot;

namespace experimental
{
	IndexedImageRawData PNGLoader::LoadRaw(const String& filename)
	{
		print_line(vformat("Loading PNG file \"%s\"...", filename));

		PackedByteArray bytes = FileAccess::get_file_as_bytes(filename);
		if (bytes.size() <= sizeof(PNGIdentifier) || memcmp(bytes.ptr(), PNGIdentifier, sizeof(PNGIdentifier)) != 0)
		{
			print_line_rich(vformat("[color=red]PNG identifier sequence not found in file \"%s\"![/color]", filename));
			return {};
		}
		print_line_rich(vformat("[color=green]PNG identifier sequence found in file \"%s\"![/color]", filename));

		readonly_bytes_stream stream{ bytes };

		auto chunks = IndexChunks(stream);

		int width, height, bitDepth;
		ColorType colorType;
		std::byte compressionMethod, filterMethod, interlaceMethod;

		auto ihdrChunkIt = chunks.find("IHDR");
		if (ihdrChunkIt == chunks.end())
		{
			print_line_rich(vformat("[color=red]Can not find the IHDR chunk in file \"%s\"![/color]", filename));
			return {};
		}
		auto ihdrData = ihdrChunkIt->second;
		{
			width = ihdrData.ReadInt32();
			height = ihdrData.ReadInt32();
			bitDepth = ihdrData.ReadInt8();
			// colorType = (ColorType)ihdrData.ReadInt8();
			// compressionMethod = (std::byte)ihdrData.ReadInt8(); // for now always 0
			// filterMethod = (std::byte)ihdrData.ReadInt8(); // for now always 0
			// interlaceMethod = (std::byte)ihdrData.ReadInt8();
			print_line(vformat("IHDR chunk: width=%d, height=%d, bitDepth=%d", width, height, bitDepth));
		}

		return {};
#if 0

	IndexChunks(bytes_view, out Dictionary<string, ReadOnlyMemory<byte>> chunks);

	if (chunks.TryGetValue("IHDR", out ReadOnlyMemory<byte> ihdrData))
	{
		using(ReadOnlyMemoryViewStream<byte> ihdrDataStream = new (ihdrData))
		{
			width = ReadInt32(ihdrDataStream);
			height = ReadInt32(ihdrDataStream);
			bitDepth = ReadInt8(ihdrDataStream);
			colorType = (ColorType)ReadInt8(ihdrDataStream);
			compressionMethod = ReadInt8(ihdrDataStream); // for now always 0
			filterMethod = ReadInt8(ihdrDataStream); // for now always 0
			interlaceMethod = ReadInt8(ihdrDataStream);
		}

		if (colorType != ColorType.Indexed)
			throw exceptionCreator($ "Not indexed color image! colorType is {colorType} in file \"{filename}\"!");

		if (compressionMethod != 0)
			throw exceptionCreator($ "Not supported compression method! compressionMethod is {compressionMethod} in file \"{filename}\"!");

		if (filterMethod != 0)
			throw exceptionCreator($ "Not supported filter method! filterMethod is {filterMethod} in file \"{filename}\"!");
	}
	else
	{
		throw exceptionCreator($ "Can not find the IHDR chunk in file \"{filename}\"!");
	}

	if (chunks.TryGetValue("sRGB", out ReadOnlyMemory<byte> srgbData))
	{
		byte renderingIntent = srgbData.Span[0];
		Logger.LogMessage($ "Rendering intent: {renderingIntent}");
	}

	Color[] palette;
	if (chunks.TryGetValue("PLTE", out ReadOnlyMemory<byte> plteData))
	{
		Debug.Assert(plteData.Length % 3 == 0);
		using ReadOnlyMemoryViewStream<byte> plteDataStream = new (plteData);
		List<Color> colors = new ();
		Span<byte> rgbBytes = stackalloc byte[3];
		while (plteDataStream.Position + 3 <= plteDataStream.Length)
		{
			plteDataStream.Read(rgbBytes);
			colors.Add(new Color(rgbBytes[0], rgbBytes[1], rgbBytes[2]));
		}
		palette = colors.ToArray();
	}
	else
	{
		throw exceptionCreator($ "Can not find the palette chunk in file \"{filename}\"!");
	}

	if (chunks.TryGetValue("tRNS", out ReadOnlyMemory<byte> trnsData))
	{
		if (trnsData.Length != palette.Length)
			throw exceptionCreator($ "Bad palette transparency length in file \"{filename}\"! Length is {trnsData.Length}. Expected {palette.Length}.");
		using ReadOnlyMemoryViewStream<byte> trnsDataStream = new (trnsData);
		for (int i = 0; i < palette.Length; ++i)
			palette[i].A = ReadInt8(trnsDataStream);
	}
	else
	{
		throw exceptionCreator($ "Can not find palette transpareny chunk in file \"{filename}\"!");
	}

	byte[] filteredImageData;
	if (chunks.TryGetValue("IDAT", out ReadOnlyMemory<byte> idatData))
	{
		using ReadOnlyMemoryViewStream<byte> idatStream = new (idatData);

		var compressedDataSize = idatStream.Length;
		using MemoryStream deflatedDataStream = new ();
		using(ZLibStream deflatingStream = new (idatStream, CompressionMode.Decompress))
				deflatingStream.CopyTo(deflatedDataStream);

		filteredImageData = deflatedDataStream.ToArray();
	}
	else
	{
		throw exceptionCreator($ "Can not find image data chunk in file \"{filename}\"!");
	}

	byte[] imageData;
	using(ReadOnlyMemoryViewStream<byte> filteredImageDataStream = new (filteredImageData))
	{
		using MemoryStream imageDataStream = new ();
		for (int lineIndex = 0; lineIndex < height; ++lineIndex)
		{
			int scanLineFilter = ReadInt8(filteredImageDataStream);
			if (scanLineFilter != 0)
				throw exceptionCreator($ "Unhandled scanline filter value {scanLineFilter}, line {lineIndex} in file \"{filename}\"!");

			for (int colIndex = 0; colIndex < width; ++colIndex)
			{
				byte colorIndex = (byte)filteredImageDataStream.ReadByte();
				imageDataStream.WriteByte(colorIndex);
			}
		}
		imageData = imageDataStream.ToArray();
	}

	return new (){
		Width = width,
		Height = height,
		ImageData = imageData,
		PaletteColors = palette
	};
#endif
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
			print_line(vformat("Found chunk %s with length %d (%d bytes remaining in file)", name, chunkLength, stream.size() - stream.Position()));
			// var chunkContent = data.Slice((int)stream.Position(), chunkLength);
			// chunks.Add(name, chunkContent);
			chunks[name] = stream.slice(chunkLength);

			// // skip the chunk data and checksum
			stream.advance(chunkLength + 4);
		}
		return chunks;
	}
} //namespace experimental