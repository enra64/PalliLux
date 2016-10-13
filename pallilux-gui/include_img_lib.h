#ifndef INCLUDE_IMG_LIB_H
#define INCLUDE_IMG_LIB_H

// load 2D histogram plugin
#define cimg_plugin2 "twodimensionalhistogram.h"
// display support is disabled in CImg under unix to avoid including X11 here, which would break building with Qt. (line 266)
#include "cimg/CImg.h"
#include "geometry.h"
typedef cimg_library::CImg<uint8_t> Image;

#endif // INCLUDE_IMG_LIB_H
