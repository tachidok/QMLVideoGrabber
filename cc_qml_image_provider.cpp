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
    int width = 100;
    int height = 50;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);
    pixmap.fill(QColor(id).rgba());

    return pixmap;
}