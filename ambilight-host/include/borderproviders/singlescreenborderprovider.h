#ifndef SINGLESCREENBORDERPROVIDER_H
#define SINGLESCREENBORDERPROVIDER_H

#include "borderprovider.h"

class SingleScreenBorderProvider : public BorderProvider
{
public:
    SingleScreenBorderProvider(size_t width, size_t height, std::shared_ptr<Screenshot> screener, size_t xOffset = 0, size_t yOffset = 0, size_t blackbarOffsetX = 0, size_t blackBarOffsetY = 0);

    void retrieveBorders(Magick::Image &right, Magick::Image &top, Magick::Image &left, Magick::Image &bottom);

    void setBorderWidth(size_t width){
        mBorderWidth = width;
        updateGeometry();
    }

    size_t getBorderWidth() const {
        return mBorderWidth;
    }

private:
    Magick::Geometry mRightGeometry;///!< right border geometry
    Magick::Geometry mTopGeometry;///!< top border geometry
    Magick::Geometry mLeftGeometry;///!< left border geometry
    Magick::Geometry mBottomGeometry;///!< bottom border geometry

    void updateGeometry();

    size_t mWidth, mHeight;
    size_t mXOffset, mYOffset;
    size_t mLetterBoxX, mLetterBoxY;

    size_t mBorderWidth = 200;///!< how wide should the borders be
};

#endif // SINGLESCREENBORDERPROVIDER_H
