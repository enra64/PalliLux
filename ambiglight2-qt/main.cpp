#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#include "ledconnector.h"
#include "rgbconstructor.h"

using namespace std;

int main()
{
    std::shared_ptr<BorderProvider> provider = std::make_shared<BorderProvider>();
    LedConnector connector(provider, 60, 20);

    connector.connect("/dev/ttyUSB0");

    for(int i = 0; i < 100; i++){
        connector.update();
        //connector.draw();
    }

    cout << "avg fps:" << (1.f / connector.getAverageUpdateDuration()) << endl;

    return 0;
}
