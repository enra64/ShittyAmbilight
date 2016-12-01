#include "rotatingcolordataprovider.h"

#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

rgb RotatingColorDataProvider::hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    if(hh < 0) hh = 360.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

/*
 *
 *
 *
 * Just the rotating color data provider
 *
 *
 *
 *
 */

double RotatingColorDataProvider::overflow(double in){
    if(in > mMinimumHue && in <= mMaximumHue)
        return in;

    // use integer division to reduce hue if it went beyond the maximum
    if(in >= mMaximumHue)
        in -= (int(in) / mMaximumHue) * mMaximumHue;
    // use integer division to raise the hue if it went below the maximum
    if(in < mMinimumHue)
        in += (abs(int(in)) / mMaximumHue + 1) * mMaximumHue;
    return in;
}

float RotatingColorDataProvider::getData(uint8_t *data) {
    // benchmarking
    clock_t start = clock();

    // rotate around the hue wheel
    mCurrentHsv.h = overflow((mCurrentHsv.h + mRotationSpeed));

    cout << "hsv: " << mCurrentHsv.h << endl;

    // how distant should the leds be positioned in the hue wheel
    const double hueDistancePerLed = (mMaximumHue - mMinimumHue) / static_cast<double>(LED_COUNT);

    for(int i = 0; i < LED_COUNT; i++){
        // create a working copy of the current hsv base (eg the start hue)
        hsv currentHsvCopy = mCurrentHsv;

        // increase that hue according to the hue position
        currentHsvCopy.h = overflow(currentHsvCopy.h + hueDistancePerLed * i);

        // convert the modified hsv to rgb
        rgb currentRgb = hsv2rgb(currentHsvCopy);

        // assign the led data
        data[(i * 3) + 0] = currentRgb.r;
        data[(i * 3) + 1] = currentRgb.g;
        data[(i * 3) + 2] = currentRgb.b;
    }

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

RotatingColorDataProvider::RotatingColorDataProvider(LedConfig d, double rotationSpeed) : ColorDataProvider(d), mRotationSpeed(rotationSpeed)
{
}
