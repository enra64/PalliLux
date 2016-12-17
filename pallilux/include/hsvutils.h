#ifndef HSVUTILS_H
#define HSVUTILS_H

// from http://stackoverflow.com/a/6930407
#include <cstdint>

typedef struct {
    double r;       // percent 0-1
    double g;       // percent 0-1
    double b;       // percent 0-1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent 0-1
    double v;       // percent 0-1
} hsv;


rgb hsv2rgb(hsv in);
hsv rgb2hsv(uint8_t r, uint8_t g, uint8_t b);
hsv rgb2hsv(rgb in);

#endif // !HSVUTILS_H
