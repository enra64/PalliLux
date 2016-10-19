#ifndef COLORDATAPROVIDER_H
#define COLORDATAPROVIDER_H

#include <unordered_map>
#include <memory>
#include <geometry.h>

#include "datafilter.h"

struct LedConfig {
    LedConfig() : bottom(INVALID), right(INVALID), top(INVALID), left(INVALID), counterClockWise(true) {}
    LedConfig(int b, int r, int t, int l, bool ccw = true) : bottom(b), right(r), top(t), left(l), counterClockWise(ccw) {}
    LedConfig(int horizontal, int vertical, bool counterClockWise = true) : LedConfig(horizontal, vertical, horizontal, vertical, counterClockWise) {}
    int bottom, right, top, left;
    bool counterClockWise;

    bool valid() {
        return bottom > 0 && right > 0 && top > 0 && left > 0;
    }

    static const int INVALID = -1;
};

/**
 * @brief Classes that implement this interface provide color data for the Leds as RGB data.
 *
 * This class also facilitates the use of DataFilter instances, with the implemented functions applyFilters() (must be called by derived class in getData()),
 * addFilter(), removeFilter() and getFilter().
 */
class ColorDataProvider {
public:
    /**
    * @brief Create a ColorDataProvider parametrized with the given led count
    * @param horizontalLedCount
    * @param verticalLedCount
    */
    ColorDataProvider(LedConfig d) :
        BOTTOM_LED_COUNT(d.bottom), RIGHT_LED_COUNT(d.right), TOP_LED_COUNT(d.top), LEFT_LED_COUNT(d.left) {
    }

    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    virtual ~ColorDataProvider() {}

    /**
     * @brief Retrieve a line of rgb data
     * @param[out] data character buffer for output. Consists of sequential RGB-triplets
     * @return the time in seconds the action required
     */
    virtual float getData(uint8_t* data) = 0;

    /**
     * @brief Access the required result buffer length
     * @return the number of bytes required to buffer the resulting led data
     */
    size_t getRequiredBufferLength() const {
        return 3 * LED_COUNT;
    }

    /**
     * @brief Add a DataFilter
     * @param id how to reference to the DataFilter
     * @param filter the DataFilter itself; the rgb converter will have sole ownership.
     */
    void addFilter(std::string id, std::unique_ptr<DataFilter> filter) {
        // insert (key, value) pair into filter list
        mDataFilters.emplace(std::make_pair(id, std::move(filter)));
    }

    /**
     * @brief Remove a DataFilter
     * @param id DataFilter name
     * @return the DataFilter
     */
    std::unique_ptr<DataFilter> removeFilter(std::string id) {
        // find the filter
        const auto& result = mDataFilters.find(id);

        // return "null" when id not found
        if (result == mDataFilters.end())
            return std::unique_ptr<DataFilter>(nullptr);

        // move out of map before erasing the value
        std::unique_ptr<DataFilter> tmp = std::move(result->second);

        // erase
        mDataFilters.erase(id);

        // return
        return move(tmp);
    }

    /**
     * @brief Get a DataFilter for editing
     * @param id DataFilter name
     * @return reference to the DataFilter
     */
    std::unique_ptr<DataFilter>& getFilter(std::string id) {
        // find the filter
        const auto& result = mDataFilters.find(id);

        // cant find that filter
        if (result == mDataFilters.end())
            throw new std::invalid_argument("could not find datafilter id" + id);

        // return
        return result->second;
    }

    /**
     * @brief Retrieve a LedCount object detailing how many leds are where
     */
    LedConfig getLedCounts(){
        return LedConfig(BOTTOM_LED_COUNT, RIGHT_LED_COUNT, TOP_LED_COUNT, LEFT_LED_COUNT);
    }

protected:
    /**
     * @brief Apply all filters that were set
     * @param resultBuffer the rgb data to be modified
     */
    void applyFilters(uint8_t* resultBuffer) {
        // iterate over the filter pairs: pair<string, unique_ptr<DataFilter>>&
        for (auto& filter : mDataFilters) {
            // call the functor
            (*filter.second)(resultBuffer, getRequiredBufferLength());
        }
    }

    const int BOTTOM_LED_COUNT;///< amount of leds on bottom border
    const int RIGHT_LED_COUNT;///< amount of leds on right border
    const int TOP_LED_COUNT;///< amount of leds on top border
    const int LEFT_LED_COUNT;///< amount of leds on left border

    const int LED_COUNT = BOTTOM_LED_COUNT + RIGHT_LED_COUNT + TOP_LED_COUNT + LEFT_LED_COUNT;///< aggregated led count

    std::unordered_map<std::string, std::unique_ptr<DataFilter>> mDataFilters;///< List of DataFilter objects that will be applied to the rgb data
};

#endif // COLORDATAPROVIDER_H
