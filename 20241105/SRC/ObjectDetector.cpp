#include "stdafx.h"
#include "ObjectDetector.h"

#include <map>

namespace
{
	struct ColorStatistics
	{
		int count;
		long red;
		long green;
		long blue;

		ColorStatistics()
			: count(0), red(0), green(0), blue(0)
		{
		}
	};

	int MakeColorBucketKey(const RGBQUAD& color)
	{
		const int red = color.rgbRed / 8;
		const int green = color.rgbGreen / 8;
		const int blue = color.rgbBlue / 8;
		return (red << 10) | (green << 5) | blue;
	}

	void AddColorSample(std::map<int, ColorStatistics>& buckets, const RGBQUAD& color)
	{
		ColorStatistics& statistics = buckets[MakeColorBucketKey(color)];
		statistics.count++;
		statistics.red += color.rgbRed;
		statistics.green += color.rgbGreen;
		statistics.blue += color.rgbBlue;
	}

	bool IsBorderPixel(DWORD x, DWORD y, DWORD width, DWORD height)
	{
		return x == 0 || y == 0 || x + 1 == width || y + 1 == height;
	}
}

ObjectMask::ObjectMask()
	: width(0), height(0), backgroundColor(), bounds()
{
	SetRectEmpty(&bounds);
}

bool ObjectMask::IsObjectPixel(DWORD x, DWORD y) const
{
	if (x >= width || y >= height)
		return false;

	return pixels[static_cast<size_t>(y) * width + x] != 0;
}

bool IsSimilarColor(const RGBQUAD& first, const RGBQUAD& second, int tolerance)
{
	if (tolerance < 0)
		tolerance = 0;

	return abs(static_cast<int>(first.rgbRed) - static_cast<int>(second.rgbRed)) <= tolerance
		&& abs(static_cast<int>(first.rgbGreen) - static_cast<int>(second.rgbGreen)) <= tolerance
		&& abs(static_cast<int>(first.rgbBlue) - static_cast<int>(second.rgbBlue)) <= tolerance;
}

RGBQUAD EstimateBackgroundColor(CxImage* image)
{
	RGBQUAD background = {};
	if (image == NULL || image->GetWidth() == 0 || image->GetHeight() == 0)
		return background;

	const DWORD width = image->GetWidth();
	const DWORD height = image->GetHeight();
	std::map<int, ColorStatistics> buckets;

	for (DWORD y = 0; y < height; ++y)
	{
		for (DWORD x = 0; x < width; ++x)
		{
			if (IsBorderPixel(x, y, width, height))
				AddColorSample(buckets, image->GetPixelColor(x, y));
		}
	}

	int bestCount = 0;
	ColorStatistics bestStatistics;
	for (std::map<int, ColorStatistics>::const_iterator it = buckets.begin(); it != buckets.end(); ++it)
	{
		if (it->second.count > bestCount)
		{
			bestCount = it->second.count;
			bestStatistics = it->second;
		}
	}

	if (bestCount > 0)
	{
		background.rgbRed = static_cast<BYTE>(bestStatistics.red / bestStatistics.count);
		background.rgbGreen = static_cast<BYTE>(bestStatistics.green / bestStatistics.count);
		background.rgbBlue = static_cast<BYTE>(bestStatistics.blue / bestStatistics.count);
	}

	return background;
}

bool FindObjectBounds(CxImage* image, const RGBQUAD& backgroundColor, int tolerance, RECT& bounds)
{
	SetRectEmpty(&bounds);
	if (image == NULL || image->GetWidth() == 0 || image->GetHeight() == 0)
		return false;

	const DWORD width = image->GetWidth();
	const DWORD height = image->GetHeight();
	DWORD left = width;
	DWORD top = height;
	DWORD right = 0;
	DWORD bottom = 0;
	bool found = false;

	for (DWORD y = 0; y < height; ++y)
	{
		for (DWORD x = 0; x < width; ++x)
		{
			if (!IsSimilarColor(image->GetPixelColor(x, y), backgroundColor, tolerance))
			{
				if (!found || x < left) left = x;
				if (!found || y < top) top = y;
				if (!found || x > right) right = x;
				if (!found || y > bottom) bottom = y;
				found = true;
			}
		}
	}

	if (!found)
		return false;

	bounds.left = static_cast<LONG>(left);
	bounds.top = static_cast<LONG>(top);
	bounds.right = static_cast<LONG>(right + 1);
	bounds.bottom = static_cast<LONG>(bottom + 1);
	return true;
}

bool BuildObjectMask(CxImage* image, const RGBQUAD& backgroundColor, int tolerance, ObjectMask& mask)
{
	mask = ObjectMask();
	if (image == NULL || image->GetWidth() == 0 || image->GetHeight() == 0)
		return false;

	mask.width = image->GetWidth();
	mask.height = image->GetHeight();
	mask.backgroundColor = backgroundColor;
	mask.pixels.assign(static_cast<size_t>(mask.width) * mask.height, 0);

	bool found = false;
	DWORD left = mask.width;
	DWORD top = mask.height;
	DWORD right = 0;
	DWORD bottom = 0;

	for (DWORD y = 0; y < mask.height; ++y)
	{
		for (DWORD x = 0; x < mask.width; ++x)
		{
			if (!IsSimilarColor(image->GetPixelColor(x, y), backgroundColor, tolerance))
			{
				mask.pixels[static_cast<size_t>(y) * mask.width + x] = 1;
				if (!found || x < left) left = x;
				if (!found || y < top) top = y;
				if (!found || x > right) right = x;
				if (!found || y > bottom) bottom = y;
				found = true;
			}
		}
	}

	if (found)
	{
		mask.bounds.left = static_cast<LONG>(left);
		mask.bounds.top = static_cast<LONG>(top);
		mask.bounds.right = static_cast<LONG>(right + 1);
		mask.bounds.bottom = static_cast<LONG>(bottom + 1);
	}

	return found;
}

bool AnalyzeImage(CxImage* image, int tolerance, ObjectMask& mask)
{
	if (image == NULL || image->GetWidth() == 0 || image->GetHeight() == 0)
	{
		mask = ObjectMask();
		return false;
	}

	const RGBQUAD backgroundColor = EstimateBackgroundColor(image);
	return BuildObjectMask(image, backgroundColor, tolerance, mask);
}
