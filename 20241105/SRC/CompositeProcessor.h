// CompositeProcessor.h : 3-step composition (member B's module)
//
//   step 1 : intermediate = head + body   (mask based, overlap gets a new color)
//   step 2 : result       = intermediate - points
//
// The module only works on RawImage / ImageMask (see ImageMask.h), so it does
// not depend on MFC or CxImage. Shape masks come from ObjectDetector.

#pragma once

#include "ImageMask.h"

// How the pixels where head and body overlap are colored.
enum OverlapMode
{
	OVERLAP_ADDITIVE = 0,   // headColor + bodyColor (clamped to 255). Falls back to
	                        // the fixed color if the sum is too close to a source color.
	OVERLAP_FIXED   = 1     // always use CompositeOptions::overlap*
};

// How the points image is subtracted from the intermediate result.
enum SubtractMode
{
	SUBTRACT_RGB    = 0,    // result = clamp(current - pointsColor) per channel
	SUBTRACT_CUTOUT = 1     // points pixels are replaced with the background color
};

struct CompositeOptions
{
	OverlapMode  overlapMode;
	unsigned char overlapR, overlapG, overlapB;   // fixed / fallback overlap color
	int          minColorDistance;                // min per-channel gap to a source color (additive mode)
	SubtractMode subtractMode;

	CompositeOptions()
		: overlapMode(OVERLAP_ADDITIVE),
		  overlapR(255), overlapG(255), overlapB(0),   // yellow
		  minColorDistance(40),
		  subtractMode(SUBTRACT_RGB) {}
};

class CompositeProcessor
{
public:
	// step 1: head + body. Canvas size = head size. Background = head background.
	//  - head only      -> head pixel
	//  - body only      -> body pixel
	//  - head and body  -> overlap color
	//  - none           -> background
	static bool AddHeadBody(const RawImage& head, const ImageMask& headMask,
	                        const RawImage& body, const ImageMask& bodyMask,
	                        const CompositeOptions& opt, RawImage& out);

	// step 2: current - points (only where pointsMask is set).
	static bool SubtractPoints(const RawImage& current,
	                           const RawImage& points, const ImageMask& pointsMask,
	                           const ImageMask& headMask,   // supplies the background color
	                           const CompositeOptions& opt, RawImage& out);

	// step 1 + step 2. 'intermediate' = result of step 1, 'result' = final image.
	static bool Compose(const RawImage& head,   const ImageMask& headMask,
	                    const RawImage& body,   const ImageMask& bodyMask,
	                    const RawImage& points, const ImageMask& pointsMask,
	                    const CompositeOptions& opt,
	                    RawImage& intermediate, RawImage& result);
};
