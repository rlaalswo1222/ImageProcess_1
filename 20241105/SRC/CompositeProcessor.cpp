// CompositeProcessor.cpp : 3-step composition (member B's module)

#include "stdafx.h"
#include "CompositeProcessor.h"

static inline unsigned char ClampByte(int v)
{
	return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

static inline int AbsDiff(int a, int b) { return a > b ? a - b : b - a; }

// true if the two colors differ by at least 'gap' in some channel
static inline bool FarEnough(int r1, int g1, int b1, int r2, int g2, int b2, int gap)
{
	return AbsDiff(r1, r2) >= gap || AbsDiff(g1, g2) >= gap || AbsDiff(b1, b2) >= gap;
}

bool CompositeProcessor::AddHeadBody(const RawImage& head, const ImageMask& headMask,
                                     const RawImage& body, const ImageMask& bodyMask,
                                     const CompositeOptions& opt, RawImage& out)
{
	if (!head.IsValid() || !body.IsValid() || !headMask.IsValid() || !bodyMask.IsValid())
		return false;

	const int w = head.width;
	const int h = head.height;
	out.Create(w, h, headMask.bgR, headMask.bgG, headMask.bgB);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			// masks / images may have different sizes: outside = background
			const bool inHead = headMask.At(x, y);
			const bool inBody = bodyMask.At(x, y) && body.InRange(x, y);

			if (!inHead && !inBody)
				continue;                                   // background already painted

			unsigned char hr = 0, hg = 0, hb = 0, br = 0, bg = 0, bb = 0;
			if (inHead) head.Get(x, y, hr, hg, hb);
			if (inBody) body.Get(x, y, br, bg, bb);

			if (inHead && inBody) {
				// overlap -> color different from both figures
				int r = opt.overlapR, g = opt.overlapG, b = opt.overlapB;
				if (opt.overlapMode == OVERLAP_ADDITIVE) {
					const int sr = hr + br, sg = hg + bg, sb = hb + bb;
					const int cr = ClampByte(sr), cg = ClampByte(sg), cb = ClampByte(sb);
					if (FarEnough(cr, cg, cb, hr, hg, hb, opt.minColorDistance) &&
					    FarEnough(cr, cg, cb, br, bg, bb, opt.minColorDistance)) {
						r = cr; g = cg; b = cb;
					}
				}
				out.Set(x, y, ClampByte(r), ClampByte(g), ClampByte(b));
			}
			else if (inHead) {
				out.Set(x, y, hr, hg, hb);
			}
			else {
				out.Set(x, y, br, bg, bb);
			}
		}
	}
	return true;
}

bool CompositeProcessor::SubtractPoints(const RawImage& current,
                                        const RawImage& points, const ImageMask& pointsMask,
                                        const ImageMask& headMask,
                                        const CompositeOptions& opt, RawImage& out)
{
	if (!current.IsValid() || !points.IsValid() || !pointsMask.IsValid())
		return false;

	out = current;   // copy, then modify only the points region

	for (int y = 0; y < current.height; y++) {
		for (int x = 0; x < current.width; x++) {
			if (!pointsMask.At(x, y) || !points.InRange(x, y))
				continue;

			if (opt.subtractMode == SUBTRACT_CUTOUT) {
				out.Set(x, y, headMask.bgR, headMask.bgG, headMask.bgB);
			}
			else {
				unsigned char cr, cg, cb, pr, pg, pb;
				current.Get(x, y, cr, cg, cb);
				points.Get(x, y, pr, pg, pb);
				out.Set(x, y, ClampByte((int)cr - (int)pr),
				              ClampByte((int)cg - (int)pg),
				              ClampByte((int)cb - (int)pb));
			}
		}
	}
	return true;
}

bool CompositeProcessor::Compose(const RawImage& head,   const ImageMask& headMask,
                                 const RawImage& body,   const ImageMask& bodyMask,
                                 const RawImage& points, const ImageMask& pointsMask,
                                 const CompositeOptions& opt,
                                 RawImage& intermediate, RawImage& result)
{
	if (!AddHeadBody(head, headMask, body, bodyMask, opt, intermediate))
		return false;
	return SubtractPoints(intermediate, points, pointsMask, headMask, opt, result);
}
