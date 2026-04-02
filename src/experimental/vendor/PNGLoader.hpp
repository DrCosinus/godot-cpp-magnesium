#pragma once

#include "indexed_image_rawdata.hpp"
#include <string>

class PNGLoader
{
public:
	static IndexedImageRawData LoadRaw(const std::string& filename);

private:
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

	enum RenderingIntent : unsigned char
	{
		Perceptual = 0, // Perceptual intent is for images preferring good adaptation to the output device gamut at the expense of colorimetric accuracy, like photographs.
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