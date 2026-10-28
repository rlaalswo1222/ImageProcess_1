#pragma once

#include <windows.h>
#include <vector>

class CxImage;

struct ObjectMask
{
	DWORD width;
	DWORD height;
	RGBQUAD backgroundColor;
	RECT bounds;
	std::vector<unsigned char> pixels;

	ObjectMask();
	bool IsObjectPixel(DWORD x, DWORD y) const;
};

bool IsSimilarColor(const RGBQUAD& first, const RGBQUAD& second, int tolerance);
RGBQUAD EstimateBackgroundColor(CxImage* image);
bool FindObjectBounds(CxImage* image, const RGBQUAD& backgroundColor, int tolerance, RECT& bounds);
bool BuildObjectMask(CxImage* image, const RGBQUAD& backgroundColor, int tolerance, ObjectMask& mask);
bool AnalyzeImage(CxImage* image, int tolerance, ObjectMask& mask);
