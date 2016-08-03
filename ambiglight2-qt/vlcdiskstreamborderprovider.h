#ifndef VLCDISKSTREAMBORDERPROVIDER_H
#define VLCDISKSTREAMBORDERPROVIDER_H

#include "borderprovider.h"



class VlcDiskstreamBorderProvider : public BorderProvider
{
public:
    /**
     * @brief takeBorderShot This function creates a shot of each border; the rgb constructor can then use it to create the LED data
     */
    void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) const override;
};

#endif // VLCDISKSTREAMBORDERPROVIDER_H
