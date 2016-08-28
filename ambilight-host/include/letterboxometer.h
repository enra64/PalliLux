#ifndef LETTERBOXOMETER_H
#define LETTERBOXOMETER_H

#include <memory>

#include <Magick++.h>

#include "screenshot.h"

class LetterboxOMeter
{
public:
	LetterboxOMeter(std::shared_ptr<Screenshot> screener, size_t w, size_t h, size_t xOff, size_t yOff, uint8_t blackThreshold);
	void measure();
	size_t getLetterboxHeight() const;
	size_t getLetterboxWidth() const;
private:// constant or input members
	const std::shared_ptr<Screenshot> mScreenshot;
	const size_t mScreenWidth;
	const size_t mScreenHeight;
	const size_t mScreenXOffset;
	const size_t mScreenYOffset;
	const uint8_t mBlackThreshold;

private:// result members
	size_t mLetterboxVerticalBarWidth = -1;
	size_t mLetterboxHorizontalBarHeight = -1;

private://functions
	bool isBlack(Magick::Image img, Magick::Geometry imgGeometry, uint8_t threshold);
	Magick::Geometry getHorizontalBar(size_t height, size_t topOffset) const;
	Magick::Geometry getVerticalBar(size_t width, size_t leftOffset) const;
};

#endif
