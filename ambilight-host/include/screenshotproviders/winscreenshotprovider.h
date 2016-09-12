#ifndef WINSCREENSHOT_H
#define WINSCREENSHOT_H

#include "screenshotprovider.h"


/**
 * @brief An implementation of the Screenshot interface for windows
 */
class WinScreenshotProvider : public ScreenshotProvider
{
public:
    WinScreenshotProvider();
    ~WinScreenshotProvider();

	/**
	 * @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
	 * @param result the resulting image
	 * @param d the dimensions the image will be; these are the dimensions requested from X
	 * @return the time in seconds required
	 */
	float getScreenCrop(Magick::Image& result, const Magick::Geometry& d) override;
};

#endif // WINSCREENSHOT_H
