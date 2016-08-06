#ifndef SINGELESCREENBORDERPROVIDER_H
#define SINGELESCREENBORDERPROVIDER_H

#include "borderprovider.h"

class SingleScreenBorderProvider : public BorderProvider
{
public:
    SingleScreenBorderProvider(size_t width, size_t height, std::shared_ptr<Screenshot> screener);

    void retrieveBorders(Magick::Image &right, Magick::Image &top, Magick::Image &left, Magick::Image &bottom);

private:
    Magick::Geometry mRightGeometry;///!< right border geometry
    Magick::Geometry mTopGeometry;///!< top border geometry
    Magick::Geometry mLeftGeometry;///!< left border geometry
    Magick::Geometry mBottomGeometry;///!< bottom border geometry

    const size_t BORDER_WIDTH = 100;///!< how wide should the borders be
};

#endif // SINGELESCREENBORDERPROVIDER_H