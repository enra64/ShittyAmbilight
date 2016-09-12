#ifndef PIXELLINEWIDGET_H
#define PIXELLINEWIDGET_H

#include <QLabel>
#include <QWidget>

#include <Magick++.h>

namespace Ui {
class PixelLineWidget;
}

class PixelLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PixelLineWidget(QWidget *parent = 0);
    ~PixelLineWidget();

public:// interaction methods
    void update(Magick::Image *pixelLine);

private slots:
    void on_fpsMeterCheckbox_clicked(bool checked);

private:
    QLabel* mPixelLine;///< label used to display line of pixels
    Magick::Blob mBlob;///< data blob used to temporarily store the image data
    QPixmap mLinePixmap;///< pixmap used to load the data into a Qt format
    bool mEnable;///< whether or not the image should be displayed

private:// ui
    Ui::PixelLineWidget *ui;
};

#endif // PIXELLINEWIDGET_H