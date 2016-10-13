#include "ambiconnectorbuilder.h"

#include <assert.h>

using namespace std;


AmbiConnectorBuilder& AmbiConnectorBuilder::setBorderProvider(std::shared_ptr<BorderProvider> provider) {
    mBorderProvider = provider;
    return *this;
}

AmbiConnectorBuilder & AmbiConnectorBuilder::setBorderProvider(BorderProvider * provider)
{
	mBorderProvider = std::shared_ptr<BorderProvider>(provider);
	return *this;
}

AmbiConnectorBuilder &AmbiConnectorBuilder::setScreenshotProvider(std::shared_ptr<ScreenshotProvider> provider) {
    mScreenshotProvider = provider;
    return *this;
}

AmbiConnectorBuilder & AmbiConnectorBuilder::setScreenshotProvider(ScreenshotProvider * provider)
{
	mScreenshotProvider = std::shared_ptr<ScreenshotProvider>(provider);
	return *this;
}

AmbiConnectorBuilder &AmbiConnectorBuilder::setAmbiColorDataProvider(std::shared_ptr<AmbiColorDataProvider> provider) {
    mAmbiColorDataProvider = provider;
    return *this;
}

AmbiConnectorBuilder & AmbiConnectorBuilder::setAmbiColorDataProvider(AmbiColorDataProvider * provider)
{
	mAmbiColorDataProvider = std::shared_ptr<AmbiColorDataProvider>(provider);
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
    if(!mAmbiColorDataProvider) throw new AmbiConnectorBuilderException("No ColorDataProvider set!");

    // set screenshot provider for our borderprovider
    mBorderProvider->setScreenshotProvider(mScreenshotProvider);

    // give our ColorDataProvider the borderprovider
    mAmbiColorDataProvider->setBorderProvider(mBorderProvider);

    // add all set filters
    for(auto it = mFilterMap.begin(); it != mFilterMap.end(); it++)
        mAmbiColorDataProvider->addFilter(it->first, move(it->second));

    // if the port value was changed, we apply it to the connector
    if(mPort != "CONNECTOR_BUILDER_DEFAULT_PORT")
        connector->setPort(mPort);

    // set the rgb line provider for the connector
    connector->setColorDataProvider(mAmbiColorDataProvider);

    return connector;
}
