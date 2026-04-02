using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Buffers.Binary;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;

namespace Magnesium.Graphics;

public class PNGLoader
{
    private static ILogger Logger { get; set; }
#if DEBUG
    = new NetworkLogger("127.0.0.1", 12345);
#else
    = new NullLogger();
#endif
    private static readonly byte[] PNGIdentifier = { 137, 80, 78, 71, 13, 10, 26, 10 };

    // bit 0: 0 - no palette, 1 - palette
    // bit 1: 0 - grayscale, 1 - color
    // bit 2: 0 - no alpha, 1 - alpha
    private enum ColorType : byte
    {
        Grayscale = 0, // depth allowed : 1, 2, 4, 8, 16
        RGB = 2, // depth allowed : 8, 16
        Indexed = 3, // PLTE chunk required... depth allowed : 1, 2, 4, 8
        GrayscaleAlpha = 4, // depth allowed : 8, 16
        RGBA = 6 // depth allowed : 8, 16
    }

    private enum RenderingIntent : byte
    {
        Perceptual = 0, // Perceptual intent is for images preferring good adaptation to the output device gamut at the expense of colorimetric accuracy, like photographs.
        RelativeColorimetric = 1, // Relative colorimetric intent is for images requiring color appearance matching (relative to the output device white point), like logos.
        Saturation = 2, // Saturation intent is for images preferring preservation of saturation at the expense of hue and lightness, like charts and graphs.
        AbsoluteColorimetric = 3 // Absolute colorimetric intent is for images requiring preservation of absolute colorimetry, like proofs (previews of images destined for a different output device).
    }

    private enum FilterType : byte
    {
        None = 0,
        Sub = 1,
        Up = 2,
        Average = 3,
        Paeth = 4
    }

    private static Exception DefaultLoadException(string message) => new InvalidDataException(message);

    public static IndexedImageRawData LoadRaw(string filename) => LoadRaw(filename, DefaultLoadException);

    public static IndexedImageRawData LoadRaw(string filename, Func<string, Exception> exceptionCreator)
    {
        Logger.LogMessage($"Loading PNG file \"{filename}\"...");

        byte[] bytes = File.ReadAllBytes(filename);

        if (bytes.Length <= PNGIdentifier.Length || !PNGIdentifier.SequenceEqual(bytes.Take(PNGIdentifier.Length)))
        {
            throw exceptionCreator($"PNG identifier sequence not found in file \"{filename}\"!");
        }
        ReadOnlyMemory<byte> bytes_view = bytes;

        int width, height, bitDepth;
        ColorType colorType;
        byte compressionMethod, filterMethod, interlaceMethod;

        IndexChunks(bytes_view, out Dictionary<string, ReadOnlyMemory<byte>> chunks);

        if (chunks.TryGetValue("IHDR", out ReadOnlyMemory<byte> ihdrData))
        {
            using (ReadOnlyMemoryViewStream<byte> ihdrDataStream = new(ihdrData))
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
                throw exceptionCreator($"Not indexed color image! colorType is {colorType} in file \"{filename}\"!");

            if (compressionMethod != 0)
                throw exceptionCreator($"Not supported compression method! compressionMethod is {compressionMethod} in file \"{filename}\"!");

            if (filterMethod != 0)
                throw exceptionCreator($"Not supported filter method! filterMethod is {filterMethod} in file \"{filename}\"!");
        }
        else
        {
            throw exceptionCreator($"Can not find the IHDR chunk in file \"{filename}\"!");
        }

        if (chunks.TryGetValue("sRGB", out ReadOnlyMemory<byte> srgbData))
        {
            byte renderingIntent = srgbData.Span[0];
            Logger.LogMessage($"Rendering intent: {renderingIntent}");
        }

        Color[] palette;
        if (chunks.TryGetValue("PLTE", out ReadOnlyMemory<byte> plteData))
        {
            Debug.Assert(plteData.Length % 3 == 0);
            using ReadOnlyMemoryViewStream<byte> plteDataStream = new(plteData);
            List<Color> colors = new();
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
            throw exceptionCreator($"Can not find the palette chunk in file \"{filename}\"!");
        }

        if (chunks.TryGetValue("tRNS", out ReadOnlyMemory<byte> trnsData))
        {
            if (trnsData.Length != palette.Length)
                throw exceptionCreator($"Bad palette transparency length in file \"{filename}\"! Length is {trnsData.Length}. Expected {palette.Length}.");
            using ReadOnlyMemoryViewStream<byte> trnsDataStream = new(trnsData);
            for (int i = 0; i < palette.Length; ++i)
                palette[i].A = ReadInt8(trnsDataStream);
        }
        else
        {
            throw exceptionCreator($"Can not find palette transpareny chunk in file \"{filename}\"!");
        }

        byte[] filteredImageData;
        if (chunks.TryGetValue("IDAT", out ReadOnlyMemory<byte> idatData))
        {
            using ReadOnlyMemoryViewStream<byte> idatStream = new(idatData);

            var compressedDataSize = idatStream.Length;
            using MemoryStream deflatedDataStream = new();
            using (ZLibStream deflatingStream = new(idatStream, CompressionMode.Decompress))
                deflatingStream.CopyTo(deflatedDataStream);

            filteredImageData = deflatedDataStream.ToArray();
        }
        else
        {
            throw exceptionCreator($"Can not find image data chunk in file \"{filename}\"!");
        }

        byte[] imageData;
        using (ReadOnlyMemoryViewStream<byte> filteredImageDataStream = new(filteredImageData))
        {
            using MemoryStream imageDataStream = new();
            for (int lineIndex = 0; lineIndex < height; ++lineIndex)
            {
                int scanLineFilter = ReadInt8(filteredImageDataStream);
                if (scanLineFilter != 0)
                    throw exceptionCreator($"Unhandled scanline filter value {scanLineFilter}, line {lineIndex} in file \"{filename}\"!");

                for (int colIndex = 0; colIndex < width; ++colIndex)
                {
                    byte colorIndex = (byte)filteredImageDataStream.ReadByte();
                    imageDataStream.WriteByte(colorIndex);
                }
            }
            imageData = imageDataStream.ToArray();
        }

        return new()
        {
            Width = width,
            Height = height,
            ImageData = imageData,
            PaletteColors = palette
        };
    }

    public static (Texture2D texture, ColorPalette palette) LoadTexture2D(string filename) => LoadTexture2D(filename, DefaultLoadException);

    public static (Texture2D texture, ColorPalette palette) LoadTexture2D(string filename, Func<string, Exception> exceptionCreator)
    {
        /*
         *         var texture = new Texture2DContent();
         *         var bitmap = new PixelBitmapContent<byte>(width, height);
         *         bitmap.SetPixelData(imageData);
         *         texture.Mipmaps.Add(bitmap);
         *         texture.OpaqueData["palette"] = palette;
         */
        GraphicsDevice graphicsDevice = MagnesiumGame.Instance.GraphicsDevice;
        IndexedImageRawData raw = LoadRaw(filename, exceptionCreator);
        Texture2D texture = new Texture2D(graphicsDevice, raw.Width, raw.Height, false, SurfaceFormat.Alpha8);
        texture.SetData(raw.ImageData);
        ColorPalette palette = new(graphicsDevice);
        palette.Colors = raw.PaletteColors;

        return (texture, palette);
    }

    private static int ReadInt32(ReadOnlyMemoryViewStream<byte> stream)
    {
        Span<byte> buffer = stackalloc byte[4];
        stream.Read(buffer);
        int integer = BinaryPrimitives.ReadInt32BigEndian(buffer);
        return integer;
    }
    private static byte ReadInt8(ReadOnlyMemoryViewStream<byte> stream) => (byte)stream.ReadByte();

    private static void IndexChunks(ReadOnlyMemory<byte> data, out Dictionary<string, ReadOnlyMemory<byte>> chunks)
    {
        chunks = new();
        using ReadOnlyMemoryViewStream<byte> stream = new(data);

        // skip the PNG identifier
        stream.Seek(PNGIdentifier.Length, SeekOrigin.Begin);

        Span<byte> buffer = stackalloc byte[4];

        // chunk size + chunk header + chunk checksum = 12 bytes (minimum chunk size)
        while (stream.Position + 12 <= stream.Length)
        {
            // read the chunk length
            int chunkLength = ReadInt32(stream);

            // read the chunk name
            stream.Read(buffer);
            var chunkName = Encoding.ASCII.GetString(buffer);
            Logger.LogMessage($"Found chunk {chunkName} with length {chunkLength} ({stream.Position})");
            var chunkContent = data.Slice((int)stream.Position, chunkLength);
            chunks.Add(chunkName, chunkContent);

            // skip the chunk data and checksum
            stream.Seek(chunkLength + 4, SeekOrigin.Current);
        }
    }

    private static ReadOnlyMemoryViewStream<byte> ExtractChunkDataStream(ReadOnlyMemory<byte> data, string chunkName)
    {
        // Using UTF-8 as extra check to make sure the name does not contain > 127 values.
        Span<byte> chunkNamebytes = Encoding.ASCII.GetBytes(chunkName).AsSpan();

        if (chunkName.Length != 4 || chunkNamebytes.Length != 4)
            throw new ArgumentException("Chunk name must be 4 ASCII characters!", nameof(chunkName));

        using ReadOnlyMemoryViewStream<byte> stream = new(data);

        // skip the PNG identifier
        stream.Seek(PNGIdentifier.Length, SeekOrigin.Begin);

        Span<byte> buffer = stackalloc byte[4];

        // chunk size + chunk header + chunk checksum = 12 bytes (minimum chunk size)
        while (stream.Position + 12 < stream.Length)
        {
            // read the chunk length
            int chunkLength = ReadInt32(stream);

            // read the chunk name
            stream.Read(buffer);
            if (chunkNamebytes.SequenceEqual(buffer))
            {
                var chunkContent = data.Slice((int)stream.Position, chunkLength);
                return new(chunkContent);
            }

            // skip the chunk data and checksum
            stream.Seek(chunkLength + 4, SeekOrigin.Current);
        }

        // chunk not found
        return null;
    }
}