#include "datafilter.h"

class LowPassFilter : public DataFilter
{
	// DataFilter interface
public:
	/**
	 * @brief Create somewhat of a gliding filter
	 * @param bufferLength how large the acquired buffer space must be
	 * @param newFactor the factor by which the new value should be used; old will use (1-newFactor)
	 */
	LowPassFilter(size_t bufferLength, float newDataFactor = .9f);

	/**
	  * Release buffer
	  */
	~LowPassFilter();

	/**
	 * @brief apply the filter
	 * @param rgbData data
	 * @param dataLength data length
	 */
	void operator ()(uint8_t* rgbData, size_t dataLength) override;

	/**
	 * @brief change new data factor
	 * @param factor new factor
	 */
	void setNewDataFactor(float factor);

private:
    uint8_t* mBuffer;///< rgb data buffer
    float mNewDataFactor;///< amount that the new data will take in filter application
};
