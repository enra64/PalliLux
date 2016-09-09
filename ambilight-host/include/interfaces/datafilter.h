#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstdint>
#include <stddef.h>

/**
 * @brief An interface for functors implementing data filters for the rgb data, like brightness factoring or low-pass filters
 */
class DataFilter
{
public:
    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
	virtual ~DataFilter()
	{
	}

	/**
	 * @brief This operator will be applied if added to an RgbConverter
	 * @param rgbData the data modifiable data
	 */
	virtual void operator()(uint8_t* rgbData, size_t dataLength) = 0;
};

#endif // DATAFILTER_H
