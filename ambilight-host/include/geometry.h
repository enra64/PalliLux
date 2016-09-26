#ifndef GEOMETRY_H
#define GEOMETRY

template <typename T>
struct Dimensions {
	Dimensions(T w, T h, T xOff = 0, T yOff = 0) : width(w), height(h), xOffset(xOff), yOffset(yOff) {
	}

	Dimensions(){}

	T width = 0;///< object width
	T height = 0;///< object height
	T xOffset = 0;///< object horizontal offset
	T yOffset = 0;///< object vertical offset

	T top() const {
		return yOffset;
	}

	T left() const {
		return xOffset;
	}

	T right() const {
		return xOffset + width;
	}

	T bottom() const {
		return yOffset + height;
	}
};

typedef Dimensions<size_t> Geometry;

#endif // !GEOMETRY_H
