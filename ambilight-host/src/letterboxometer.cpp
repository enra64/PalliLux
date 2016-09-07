#include "letterboxometer.h"

using namespace Magick;
using namespace std;

size_t LetterboxOMeter::getLetterboxWidth() const
{
	return mLetterboxVerticalBarWidth;
}

bool LetterboxOMeter::isBlack(Magick::Image img, Magick::Geometry imgGeometry, uint8_t threshold)
{
	for (size_t x = 0; x < imgGeometry.width(); x++)
	{
		for (size_t y = 0; y < imgGeometry.height(); y++)
		{
			ColorRGB data = img.pixelColor(x, y);
			if (data.red() * 255.0 > threshold) return false;
			if (data.green() * 255.0 > threshold) return false;
			if (data.blue() * 255.0 > threshold) return false;
		}
	}
	return true;
}

size_t LetterboxOMeter::getLetterboxHeight() const
{
	return mLetterboxHorizontalBarHeight;
}

Geometry LetterboxOMeter::getHorizontalBar(size_t height, size_t topOffset) const
{
	return Geometry(mScreenWidth, height, mScreenXOffset, mScreenYOffset + topOffset);
}

Geometry LetterboxOMeter::getVerticalBar(size_t width, size_t leftOffset) const
{
	return Geometry(width, mScreenHeight, mScreenXOffset + leftOffset, mScreenYOffset);
}

void LetterboxOMeter::measure()
{
	Geometry horizontalBarGeometry, verticalBarGeometry;
	Image horizontalBar, verticalBar;

	// move the bar as far down as is black
	do
	{
		horizontalBarGeometry = getHorizontalBar(1, ++mLetterboxHorizontalBarHeight);
		mScreenshot->takeScreenshot();
		mScreenshot->getScreenCrop(horizontalBar, horizontalBarGeometry);
	}
	while (isBlack(horizontalBar, horizontalBarGeometry, mBlackThreshold));

	// move the bar as far right as is black
	do
	{
		verticalBarGeometry = getVerticalBar(1, ++mLetterboxVerticalBarWidth);
		mScreenshot->takeScreenshot();
		mScreenshot->getScreenCrop(verticalBar, verticalBarGeometry);
	}
	while (isBlack(verticalBar, verticalBarGeometry, mBlackThreshold));
}

LetterboxOMeter::LetterboxOMeter(
	std::shared_ptr<ScreenshotProvider> screener, size_t w, size_t h, size_t xOff, size_t yOff, uint8_t blackThreshold) :
	mScreenshot(screener), mScreenWidth(w), mScreenHeight(h),
	mScreenXOffset(xOff), mScreenYOffset(yOff), mBlackThreshold(blackThreshold)
{
}
