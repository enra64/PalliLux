#ifndef DIMENSIONS_H
#define DIMENSIONS_H

/**
 * @brief The Dimensions struct contains position and size information
 */
struct Dimensions {
    Dimensions(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
    }

    int x, y, w, h;
};

#endif // DIMENSIONS_H
