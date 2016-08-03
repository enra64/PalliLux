#include <iostream>
#include <algorithm>
#include <string>

#include <assert.h>

using namespace std;

#include "ledconnector.h"
#include "rgbconstructor.h"
#include "vlcdiskstreamborderprovider.h"

using namespace std;

int main()
{
    std::shared_ptr<BorderProvider> provider = std::static_pointer_cast<BorderProvider>(std::make_shared<XlibBorderProvider>());
    LedConnector connector(provider, 60, 20);

    bool connection = connector.connect("/dev/ttyUSB0");

    assert(connection);

    cout << "conn ok" << endl;

    for(int i = 0; i < 100;){
        connector.update();
        connector.draw();
    }

    cout << "avg fps:" << (1.f / connector.getAverageUpdateDuration()) << endl;

    return 0;
}
