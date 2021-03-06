#include "ddapiscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

DdApiScreenshotProvider::DdApiScreenshotProvider() {
    initializeDx();
}

DdApiScreenshotProvider::~DdApiScreenshotProvider() {
    deInitializeDx();

    for (DdApiScreen* s : mScreenVector)
        delete s;
}

void DdApiScreenshotProvider::addScreenHandler(int screenNumber, Rotation rotation) {
    mScreenVector.push_back(new DdApiScreen(mDxDevice, rotation, screenNumber));
}

void DdApiScreenshotProvider::initializeDx() {
    // initialize direct x device and connection

    // resulting feature level nobody cares about
    D3D_FEATURE_LEVEL tmpFeatureLvl;

    // hresult for device creation
    HRESULT deviceCreation;

    // iterate over all sufficient driver types
    for (int i = 0; i < D3D_DRIVER_TYPE_COUNT; i++) {
        // we
        // need
        // MORE
        // PAARAAAAMETERS
        deviceCreation = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPES[i],
            nullptr,
            0,
            D3D_VERSIONS,
            D3D_VERSION_COUNT,
            D3D11_SDK_VERSION,
            &mDxDevice,
            &tmpFeatureLvl,
            &mDxDeviceContext);

        // we have successfully gotten some device, abort mission
        if (SUCCEEDED(deviceCreation))
            break;
    }

    if (FAILED(deviceCreation))
        throw std::runtime_error("could not create dx device");
}

void DdApiScreenshotProvider::deInitializeDx()
{
    if (mDxDevice) {
        mDxDevice->Release();
        mDxDevice = nullptr;
    }

    if (mDxDeviceContext) {
        mDxDeviceContext->Release();
        mDxDeviceContext = nullptr;
    }
}

void DdApiScreenshotProvider::reinitializeDx(){
    // release old pointers
    deInitializeDx();

    // re-create our dx device
    initializeDx();

    // force each DdApiScreen to reinitialize
    for (DdApiScreen* s : mScreenVector)
        s->reinitialize(mDxDevice);
}

void DdApiScreenshotProvider::takeScreenshot() {
	// panic: if we dont have a single ScreenHandler, this house of cards will come crashing down
	if (mScreenVector.size() == 0)
		// "safe default" since 0 is main screen
		addScreenHandler(0, Rotation::CounterClockWise0);

	// reset main image
	mImage.clear();
	
    int failureCount = 0;

    for (unsigned int i = 0; i < mScreenVector.size(); i++) {
        // windows might throw some weird-ass errors
        shared_ptr<Image> img;
        try {
             img = mScreenVector.at(i)->getScreenshot(mDxDeviceContext);
        } catch (exception e){
            cout << "ddapiscreen exception " << e.what() << endl;
        }

		// if the fallback system of DdApiScreen failed, we must force retaking the screenshot
		if (!img) {
			i--;
            // if we hit this too often, we will have to re-initialitze the whole system,
            // as dx probably did some weird shit
            failureCount++;
            if(failureCount > 100){
                cout << "begin reinit" << endl;
                // We wait 10s to let dx finish whatever fuckery it was doing
                Sleep(10000);

                // now we reinitialize the whole dx system
                reinitializeDx();

                // start taking this screenshot batch again
                takeScreenshot();

                // reset fail counter
                failureCount = 0;

				cout << "reinit complete" << endl;

                return;
            }

            continue;
		}

        // check whether cimg is ok
        assert(!img->is_empty());

		mImage.append(*img.get());
	}
}

float DdApiScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();

	assert(!mImage.is_empty());

	// crop out the relevant part
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

bool DdApiScreenshotProvider::getDxDevice()
{
	

	return false;
}
