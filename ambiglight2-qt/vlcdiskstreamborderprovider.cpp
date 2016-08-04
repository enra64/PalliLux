#include "vlcdiskstreamborderprovider.h"

using namespace Magick;
using namespace std;

#include <vector>

void VlcDiskstreamBorderProvider::retrieveBorders(Image &right, Image &top, Image &left, Image &bottom) const {
    Image source;
    // 120x58 | 60x29
    source.read("/home/arne/stream");

    right = source;
    top = source;
    left = source;

    // border width 4
    // heights: l39, m32, r58
    // widths:  l30, m58, r32

    right.crop("4x54+116+4");
    left.crop("4x35+0+4");
    top.crop("120x4");

    vector<Image> bottomBorderVector = vector<Image>(3);

    // assign source
    bottomBorderVector[0] = source;
    bottomBorderVector[1] = source;
    bottomBorderVector[2] = source;

    // crop
    bottomBorderVector[0].crop("30x4+0+35");
    bottomBorderVector[1].crop("58x4+30+28");
    bottomBorderVector[2].crop("32x4+88+54");

    appendImages(&bottom, bottomBorderVector.begin(), bottomBorderVector.end());
}
