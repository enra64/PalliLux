#ifndef RGBLINEPROVIDER_H
#define RGBLINEPROVIDER_H

#include <unordered_map>
#include <memory>

#include "datafilter.h"

/**
 * @brief An interface to provide an array of rgb data.
 * It also provides "::"<applyFilters>, which applies all DataFilter objects set with "::"<addFilter> to the provided buffer.
 */
class RgbLineProvider {
public:
    /**
     * @brief Create a RgbLineProvider parametrized with the given led count
     * @param horizontalLedCount
     * @param verticalLedCount
     */
    RgbLineProvider(size_t horizontalLedCount, size_t verticalLedCount) :
        HORIZONTAL_LED_COUNT(horizontalLedCount),
        VERTICAL_LED_COUNT(verticalLedCount){}

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
    void addFilter(std::string id, std::unique_ptr<DataFilter> filter){
        // insert (key, value) pair into filter list
        mDataFilters.emplace(std::make_pair(id, std::move(filter)));
    }

    /**
     * @brief Remove a DataFilter
     * @param id DataFilter name
     * @return the DataFilter
     */
    std::unique_ptr<DataFilter> removeFilter(std::string id)
    {
        // find the filter
        const auto& result = mDataFilters.find(id);

        // return "null" when id not found
        if(result == mDataFilters.end())
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
    std::unique_ptr<DataFilter>& getFilter(std::string id)
    {
        // find the filter
        const auto& result = mDataFilters.find(id);

        // cant find that filter
        if(result == mDataFilters.end())
            throw new std::invalid_argument("could not find datafilter id" + id);

        // return
        return result->second;
    }

protected:
    /**
     * @brief Apply all filters that were set
     * @param resultBuffer the rgb data to be modified
     */
    void applyFilters(uint8_t *resultBuffer)
    {
        // iterate over the filter pairs: pair<string, unique_ptr<DataFilter>>&
        for(auto& filter : mDataFilters){
            // call the functor
            (*filter.second)(resultBuffer, getRequiredBufferLength());
        }
    }

    const unsigned int HORIZONTAL_LED_COUNT;///!< amount of leds on each horizontal border

    const unsigned int VERTICAL_LED_COUNT;///!< amount of leds on each vertical border

    const unsigned int LED_COUNT = VERTICAL_LED_COUNT * 2 + HORIZONTAL_LED_COUNT * 2; ///!< aggregated count

    std::unordered_map<std::string, std::unique_ptr<DataFilter>> mDataFilters;///!< List of DataFilter objects that will be applied to the rgb data
};

#endif // RGBLINEPROVIDER_H
