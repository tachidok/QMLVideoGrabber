#include "cc_qml_image_provider.h"

// =============================================================
// Constructor
// =============================================================
CCQmlImageProvider::CCQmlImageProvider()
    : QQuickImageProvider (QQuickImageProvider::Pixmap)
{

}

// =============================================================
// Destructor
// =============================================================
CCQmlImageProvider::~CCQmlImageProvider()
{

}

// =============================================================
// In charge of returning the pixmap
// =============================================================
QPixmap CCQmlImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage qimage(Grabber_pt->image_pt(), Grabber_pt->width(), Grabber_pt->height(), Grabber_pt->bpl(), QImage::Format_RGB888);
    qDebug() << "New pixmap";
    //ui->lbl_image->setPixmap(QPixmap::fromImage(qimage));

    //return pixmap;
    // TODO HERE
    return QPixmap::fromImage(qimage);
}
