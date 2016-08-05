#include <iostream>
#include <algorithm>
#include <string>

#include <assert.h>

using namespace std;

#include "ambiconnector.h"
#include "rgbconstructor.h"

using namespace std;

int main() {
    // create a shared pointe to the desired borderProvider (must be static_pointer_casted to the base class)
    std::shared_ptr<BorderProvider> provider = std::static_pointer_cast<BorderProvider>(std::make_shared<TripleScreenBorderProvider>());

    // supply our ledconnector with the desired borderProvider and the count of horizontal/vertical leds on each border
    AmbiConnector connector(provider, 60, 20);

    // connect to the arduino; abort if connection failed
    if(!connector.connect("/dev/ttyUSB0"))
        exit(1);

    // loop: update the screen images and push the data to the arduino
    //for(int i = 0; i < 100; i++) {
    while(1) {
        connector.update();
        connector.draw();

        cout << "avg draw fps:" << connector.getCurrentFps() << endl;
    }

    return 0;
}