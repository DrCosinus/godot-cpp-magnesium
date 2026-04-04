#pragma once

#include "indexed_image_rawdata.hpp"
#include "readonly_bytes_stream.hpp"
#include <godot_cpp/variant/string.hpp>
#include <map>

namespace experimental
{
	class PNGLoader
	{
	public:
		static IndexedImageRawData LoadRaw(const godot::String& filename);

	private:
		static std::map<godot::String, readonly_bytes_stream> IndexChunks(readonly_bytes_stream& data);

		static constexpr unsigned char PNGIdentifier[8] = { 137,
															80,
															78,
															71,
															13,
															10,
															26,
															10 };

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
		static godot::String GetColorTypeName(ColorType colorType)
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

		enum RenderingIntent : unsigned char
		{
			Perceptual = 0, // Perceptual intent is for images preferring good adaptation to the output device gamut at the expense of colorimetric accuracy, like photographs.
			RelativeColorimetric = 1, // Relative colorimetric intent is for images requiring color appearance matching (relative to the output device white point), like logos.
			Saturation = 2, // Saturation intent is for images preferring preservation of saturation at the expense of hue and lightness, like charts and graphs.
			AbsoluteColorimetric = 3 // Absolute colorimetric intent is for images requiring preservation of absolute colorimetry, like proofs (previews of images destined for a different output device).
		};

		enum FilterType : unsigned char
		{
			None = 0,
			Sub = 1,
			Up = 2,
			Average = 3,
			Paeth = 4
		};

	public:
	};
} //namespace experimental