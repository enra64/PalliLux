#include "ambiconnectorbuilder.h"

#include <assert.h>

using namespace std;


AmbiConnectorBuilder& AmbiConnectorBuilder::setBorderProvider(std::shared_ptr<BorderProvider> provider) {
    mBorderProvider = provider;
    return *this;
}

AmbiConnectorBuilder &AmbiConnectorBuilder::setScreenshotProvider(std::shared_ptr<Screenshot> provider) {
    mScreenshotProvider = provider;
    return *this;
}

AmbiConnectorBuilder &AmbiConnectorBuilder::setAmbiRgbLineProvider(std::shared_ptr<AmbiRgbLineProvider> provider) {
    mAmbiRgbLineProvider = provider;
    return *this;
}

AmbiConnectorBuilder& AmbiConnectorBuilder::setPort(const string &port) {
    mPort = port;
    return *this;
}

AmbiConnectorBuilder& AmbiConnectorBuilder::addDataFilter(const std::string name, std::unique_ptr<DataFilter> filter) {
    mFilterMap.emplace(make_pair(name, move(filter)));
    return *this;
}

std::shared_ptr<ArduinoConnector> AmbiConnectorBuilder::build() {
    // create an ArduinoConnector
    std::shared_ptr<ArduinoConnector> connector = make_shared<ArduinoConnector>();

    // check whether all necessary components have been set
    if(!mScreenshotProvider) throw new AmbiConnectorBuilderException("No ScreenshotProvider set!");
    if(!mBorderProvider) throw new AmbiConnectorBuilderException("No BorderProvider set!");
    if(!mAmbiRgbLineProvider) throw new AmbiConnectorBuilderException("No RgbLineProvider set!");

    // set screenshot provider for our borderprovider
    mBorderProvider->setScreenshotProvider(mScreenshotProvider);

    // give our rgblineProvider the borderprovider
    mAmbiRgbLineProvider->setBorderProvider(mBorderProvider);

    // add all set filters
    for(auto it = mFilterMap.begin(); it != mFilterMap.end(); it++)
        mAmbiRgbLineProvider->addFilter(it->first, move(it->second));

    // if the port value was changed, we apply it to the connector
    if(mPort != "CONNECTOR_BUILDER_DEFAULT_PORT")
        connector->setPort(mPort);

    // set the rgb line provider for the connector
    connector->setRgbLineProvider(mAmbiRgbLineProvider);

    return connector;
}
