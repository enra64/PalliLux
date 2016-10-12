#include "configurableborderprovider.h"

ConfigurableBorderProvider::ConfigurableBorderProvider(
	std::vector<Geometry> bottomBorderElements, 
	std::vector<Geometry> rightBorderElements, 
	std::vector<Geometry> topBorderElements, 
	std::vector<Geometry> leftBorderElements)
{
	for (Geometry& g : bottomBorderElements)
		mBottomBorderElements.push_back(BorderElement(g));

	for (Geometry& g : rightBorderElements)
		mRightBorderElements.push_back(BorderElement(g));

	for (Geometry& g : topBorderElements)
		mTopBorderElements.push_back(BorderElement(g));

	for (Geometry& g : leftBorderElements)
		mLeftBorderElements.push_back(BorderElement(g));
}

void ConfigurableBorderProvider::retrieveBorders(Image & right, Image & top, Image & left, Image & bottom)
{
	for (BorderElement& e : mBottomBorderElements)
		bottom.append(getImage(e));

	for (BorderElement& e : mRightBorderElements)
		right.append(getImage(e));

	for (BorderElement& e : mTopBorderElements)
		top.append(getImage(e));

	for (BorderElement& e : mLeftBorderElements)
		left.append(getImage(e));
}

void ConfigurableBorderProvider::updateGeometry()
{
}

Image ConfigurableBorderProvider::getImage(BorderElement & e){
	Image latestImage;

	mScreenshotProvider->getScreenCrop(latestImage, e.geometry);

	// screenshot success!
	if (!latestImage.is_empty()) {
		// if the fallback image is not too old or invalid
		if (e.fallbackAge >= 0 && e.fallbackAge < 100) {
			// fallback image is now older
			e.fallbackAge++;
		}
		// the fallback image must be refreshed
		else {
			// save fallback image
			e.fallback = latestImage;
			// update age to zero
			e.fallbackAge = 0;
		}
	}
	// the screenshot was not successfully taken - use the fallback image
	else {
		latestImage = e.fallback;
	}

	return latestImage;
}
