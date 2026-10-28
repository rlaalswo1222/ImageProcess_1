// ImageMask.h : shared data types between ObjectDetector (member A) and
//               CompositeProcessor (member B).
//
// These types do NOT depend on MFC or CxImage so that both sides can be
// developed and tested independently.
//
// Coordinate system: x = left -> right, y = top -> bottom (origin top-left).

#pragma once

#include <cstddef>
#include <vector>

using std::size_t;

// Plain 24-bit RGB image (3 bytes per pixel, order R,G,B, row-major).
struct RawImage
{
	int width;
	int height;
	std::vector<unsigned char> rgb;

	RawImage() : width(0), height(0) {}

	void Create(int w, int h, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0)
	{
		width = w;
		height = h;
		rgb.assign((size_t)w * h * 3, 0);
		for (int i = 0; i < w * h; i++) {
			rgb[i * 3 + 0] = r;
			rgb[i * 3 + 1] = g;
			rgb[i * 3 + 2] = b;
		}
	}

	bool IsValid() const { return width > 0 && height > 0 && rgb.size() == (size_t)width * height * 3; }
	bool InRange(int x, int y) const { return x >= 0 && y >= 0 && x < width && y < height; }

	void Get(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b) const
	{
		const size_t p = ((size_t)y * width + x) * 3;
		r = rgb[p]; g = rgb[p + 1]; b = rgb[p + 2];
	}
	void Set(int x, int y, unsigned char r, unsigned char g, unsigned char b)
	{
		const size_t p = ((size_t)y * width + x) * 3;
		rgb[p] = r; rgb[p + 1] = g; rgb[p + 2] = b;
	}
};

// Binary mask of one input image: 1 = shape pixel, 0 = background.
// Also carries the detected background color (needed to paint the result
// background).
struct ImageMask
{
	int width;
	int height;
	std::vector<unsigned char> data;   // width*height, 0 or 1

	unsigned char bgR, bgG, bgB;       // detected background color

	// Optional info filled by the detector (bounding box of the shape).
	// left > right means "no shape found".
	int left, top, right, bottom;

	ImageMask() : width(0), height(0), bgR(0), bgG(0), bgB(0),
	              left(0), top(0), right(-1), bottom(-1) {}

	void Create(int w, int h)
	{
		width = w;
		height = h;
		data.assign((size_t)w * h, 0);
	}

	bool IsValid() const { return width > 0 && height > 0 && data.size() == (size_t)width * height; }

	// Out-of-range pixels are treated as background.
	bool At(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= width || y >= height) return false;
		return data[(size_t)y * width + x] != 0;
	}
};
