#include "PNGLoader.hpp"

#if 0
#include <godot_cpp/classes/file_access.hpp>

IndexedImageRawData PNGLoader::LoadRaw(const std::string& filename)
{
	// Logger.LogMessage($ "Loading PNG file \"{filename}\"...");

	byte[] bytes = File.ReadAllBytes(filename);

	if (bytes.Length <= PNGIdentifier.Length || !PNGIdentifier.SequenceEqual(bytes.Take(PNGIdentifier.Length)))
	{
		throw exceptionCreator($ "PNG identifier sequence not found in file \"{filename}\"!");
	}
	ReadOnlyMemory<byte> bytes_view = bytes;

	int width, height, bitDepth;
	ColorType colorType;
	byte compressionMethod, filterMethod, interlaceMethod;

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
}
#endif