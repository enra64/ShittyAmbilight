#ifndef SCREENSHOT_H
#define SCREENSHOT_H

// channel order load plugin needs these
#include <stdexcept>
#include <stdint.h>
#include <assert.h>

// define our custom plugin to allow easier loading of interleaved BGR-or-BGRA-in-whatever-order images
#define cimg_plugin1 "load_custom_channel_order.h"

// display support is disabled in CImg under unix to avoid including X11 here, which would break building with Qt. (line 266)
#ifndef cimg_version
    #include "CImg.h"
#endif

#include "geometry.h"
#include "stdint.h"
typedef cimg_library::CImg<uint8_t> Image;

#include <string>

namespace cimg_library {
	// avoid magic numbers
	const uint8_t CIMG_RED_CHANNEL = 0, CIMG_GREEN_CHANNEL = 1, CIMG_BLUE_CHANNEL = 2;
	const uint8_t CIMG_2D_Z_LEVEL = 0, CIMG_2D_Z_LEVEL_COUNT = 1;
    const uint8_t PALLILUX_CHANNEL_COUNT = 3;
    const uint8_t PALLILUX_INTERPOLATION_TYPE = 1;// 1 - slow, 2 - slower, 3 - very slow, 4 - ~20fps, 5 - 10-15, 6 - 10-15
	const bool CIMG_SHARED_MEMORY = true, CIMG_NO_SHARED_MEMORY = false;
}

/**
 * @brief Exceptions occuring in the Screenshot code
 */
class ScreenshotException : public std::runtime_error
{
public:
    /**
     * @brief std::string constructor
     * @param message the error message
     */
    ScreenshotException(const std::string& message) : runtime_error(message)
    {
    }
};

/**
 * @brief Interface for capturing screen areas
 */
class ScreenshotProvider {
public:
    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    virtual ~ScreenshotProvider() {
    }

    /**
     * @brief Provide a crop of the requested geometry
     * @param[out] result the resulting image
     * @param d the geometry saved in the result, relative to the complete screen
     * @return the time in seconds required
     */
    virtual float getScreenCrop(Image& result, const Geometry& d) = 0;

    /**
     * @brief Take a screenshot.
     *
     * This should be called once per frame by any BorderProvider, to avoid taking multiple screenshots for
     * the different borders if the screenshot implementation is faster capturing only once.
     */
    virtual void takeScreenshot() {
    }
};

#endif // SCREENSHOT_H
