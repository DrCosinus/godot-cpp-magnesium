#!/usr/bin/env python3
"""
Import an indexed (paletted) image and export:
 - an index image (grayscale PNG where each pixel value is the palette index)
 - a palette PNG (1 x N RGBA) and a JSON metadata file

If the input is not paletted, pass `--quantize` to convert it to an indexed image.
"""
import argparse
import json
import os
from PIL import Image


def extract_palette(img):
    pal = img.getpalette()
    if pal is None:
        return []
    # pal is a flat list [r,g,b,r,g,b,...]
    colors = []
    for i in range(0, len(pal), 3):
        r = pal[i]
        g = pal[i + 1]
        b = pal[i + 2]
        colors.append([r, g, b, 255])
    # clamp length to 256
    return colors[:256]


def main():
    ap = argparse.ArgumentParser(description="Import paletted/indexed images and export palette + index map")
    ap.add_argument("infile", help="Input image file (png, gif, etc.)")
    ap.add_argument("--out-dir", default=".", help="Output directory")
    ap.add_argument("--quantize", action="store_true", help="Quantize non-paletted images to an indexed palette")
    ap.add_argument("--colors", type=int, default=256, help="Max palette colors when quantizing (default 256)")
    args = ap.parse_args()

    infile = args.infile
    out_dir = args.out_dir
    os.makedirs(out_dir, exist_ok=True)

    img = Image.open(infile)

    if img.mode != 'P':
        if not args.quantize:
            print(f"Input image mode is '{img.mode}' (not paletted). Use --quantize to convert to indexed.")
            return 1
        # convert to RGBA then quantize
        img = img.convert('RGBA').quantize(colors=args.colors, method=Image.MEDIANCUT)

    palette_colors = extract_palette(img)
    if not palette_colors:
        print("No palette found in image.")
        return 1

    width, height = img.size
    indices = list(img.getdata())

    base = os.path.splitext(os.path.basename(infile))[0]
    index_fname = os.path.join(out_dir, f"{base}_index.png")
    palette_png = os.path.join(out_dir, f"{base}_palette.png")
    meta_json = os.path.join(out_dir, f"{base}_indexed.json")

    # Save index image as grayscale where pixel value == palette index
    index_img = Image.new('L', (width, height))
    index_img.putdata(indices)
    index_img.save(index_fname)

    # Save palette strip as 1xN RGBA image (width = palette length)
    pal_len = len(palette_colors)
    pal_img = Image.new('RGBA', (pal_len, 1))
    pal_img.putdata([tuple(c) for c in palette_colors])
    pal_img.save(palette_png)

    # Handle palette transparency if present
    transparency = None
    info = img.info
    if 'transparency' in info:
        transparency = info['transparency']
        # PIL may store a single index or a list; normalize to int or list

    meta = {
        'source': infile,
        'index_image': os.path.relpath(index_fname, start=os.getcwd()),
        'palette_image': os.path.relpath(palette_png, start=os.getcwd()),
        'width': width,
        'height': height,
        'palette_size': pal_len,
        'mode': img.mode,
        'transparency': transparency,
    }

    with open(meta_json, 'w', encoding='utf-8') as f:
        json.dump(meta, f, indent=2)

    print(f"Wrote index -> {index_fname}")
    print(f"Wrote palette -> {palette_png} ({pal_len} colors)")
    print(f"Wrote metadata -> {meta_json}")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
