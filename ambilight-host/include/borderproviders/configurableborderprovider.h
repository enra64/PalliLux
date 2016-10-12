#ifndef CONFIGURABLEBORDERPROVIDER_H
#define CONFIGURABLEBORDERPROVIDER_H

#include "borderprovider.h"

#include <vector>

/**
 * @brief A BorderProvider that considers a single screen.
 *
 * The position may be offset to select a screen out of multiple ones, and offsets to consider letterboxing are supported.
 */
class ConfigurableBorderProvider : public BorderProvider
{
public:
	ConfigurableBorderProvider(
		std::vector<Geometry> bottomBorderElements,
		std::vector<Geometry> rightBorderElements,
		std::vector<Geometry> topBorderElements,
		std::vector<Geometry> leftBorderElements);

	void retrieveBorders(Image& right, Image& top, Image& left, Image& bottom) override;

protected:
	void updateGeometry() override;
	
private:
	/// \brief encapsulate data for each border element
	struct BorderElement {
		BorderElement(Geometry g) : geometry(g) {}
		Image fallback;
		char fallbackAge = -1;
		Geometry geometry;
	};

	/// \brief 
	Image getImage(BorderElement& e);

	std::vector<BorderElement> mBottomBorderElements;///< list of bottom border elements
	std::vector<BorderElement> mRightBorderElements;///< list of right border elements
	std::vector<BorderElement> mTopBorderElements;///< list of top border elements
	std::vector<BorderElement> mLeftBorderElements;///< list of left border elements
};

#endif // CONFIGURABLEBORDERPROVIDER_H
