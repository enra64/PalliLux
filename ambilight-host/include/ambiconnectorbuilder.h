#ifndef AMBICONNECTORBUILDER_H
#define AMBICONNECTORBUILDER_H

#include "arduinoconnector.h"

#include <memory>
#include <unordered_map>
#include <stdexcept>

/**
 * @brief Exceptions thrown when the AmbiConnectorBuilder finds an error during construction of the ArduinoConnector
 */
class AmbiConnectorBuilderException : std::runtime_error {
public:
    /**
     * @brief std::string constructor
     * @param message the error message
     */
    AmbiConnectorBuilderException(const std::string& message) : runtime_error(message)
    {
    }
};

/**
 * @brief A Builder class to simplify the creation of an ArduinoConnector with Ambilight-like functionality.
 *
 * The following three components must be set:\n
 * 1) ScreenshotProvider \n
 * 2) BorderProvider \n
 * 3) AmbiColorDataProvider \n
 * The ArduinoConnector may then be created using the build() function. Additionally, the Arduino port can be set, and
 * DataFilter instances can be added to the AmbiColorDataProvider.
 */
class AmbiConnectorBuilder {
public:// setters for providers
    /**
     * @brief setBorderProvider set the BorderProvider that should be used by the ArduinoConnector.
     *
     * Example:
     * `builder.setBorderProvider(new MyBorderProvider());`
     */
    AmbiConnectorBuilder& setBorderProvider(std::shared_ptr<BorderProvider> provider);

    /**
     * @brief Set the ScreenshotProvider that should be used by the ArduinoConnector.
     *
     * Example:
     * `builder.setScreenshotProvider(new XlibScreenshot());`
     */
    AmbiConnectorBuilder& setScreenshotProvider(std::shared_ptr<ScreenshotProvider> provider);

    /**
     * @brief Set the AmbiColorDataProvider the ArduinoConnector shall use.
     *
     * Example:
     * `builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(horizontalBorderLedCount, verticalBorderLedCount)));`
     */
    AmbiConnectorBuilder& setAmbiColorDataProvider(std::shared_ptr<AmbiColorDataProvider> provider);

    /**
     * @brief Set the port the ArduinoConnector shall use
     */
    AmbiConnectorBuilder &setPort(const std::string& port);

    /**
     * @brief Add a DataFilter to the ArduinoConnector
     * @param name the std::string id of the filter for later modifications
     * @param filter the actual filter object
     */
    AmbiConnectorBuilder &addDataFilter(const std::string name, std::unique_ptr<DataFilter> filter);

public:// result function
    /**
     * @brief Builds a valid ArduinoConnector with an AmbiColorDataProvider.
     *
     * Check whether all requirements for a valid ArduinoConnector are filled, and if so, return one.
     * If a requirement is not fulfilled, an AmbiConnectorBuilderException will be thrown with information as to what is missing.
     * @return std::shared_ptr to an ArduinoConnector corresponding to the given parameters
     */
    std::shared_ptr<ArduinoConnector> build();

private:
    std::string mPort = "CONNECTOR_BUILDER_DEFAULT_PORT";///< the tty port to use. the default is invalid, and will not be set.
    std::shared_ptr<BorderProvider> mBorderProvider;///< the BorderProvider set
    std::shared_ptr<ScreenshotProvider> mScreenshotProvider;///< the ScreenshotProvider set
    std::shared_ptr<AmbiColorDataProvider> mAmbiColorDataProvider;///< the AmbiColorDataProvider set
    std::unordered_map<std::string, std::unique_ptr<DataFilter>>  mFilterMap;///< A map of given DataFilter instances
};

#endif // AMBICONNECTORBUILDER_H
