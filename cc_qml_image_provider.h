#ifndef CCQMLIMAGEPROVIDER_H
#define CCQMLIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "src/cc_grabber.h"
#include <QtDebug>

class CCQmlImageProvider : public QQuickImageProvider
{

public:

    // Constructor
    CCQmlImageProvider();

    // Destructor
    ~CCQmlImageProvider();

    // In charge of returning the pixmap
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    // Set grabber pointer
    inline void set_grabber_pt(CCGrabber *grabber_pt) {Grabber_pt = grabber_pt;}

private:

    // A pointer to the grabber that provides the image
    CCGrabber *Grabber_pt;

};

#endif // CCQMLIMAGEPROVIDER_H
