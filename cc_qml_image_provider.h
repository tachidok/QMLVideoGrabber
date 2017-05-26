#ifndef CCQMLIMAGEPROVIDER_H
#define CCQMLIMAGEPROVIDER_H

#include <QQuickImageProvider>

class CCQmlImageProvider : public QQuickImageProvider
{

public:

    // Constructor
    CCQmlImageProvider();

    // Destructor
    ~CCQmlImageProvider();

    // In charge of returning the pixmap
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

};

#endif // CCQMLIMAGEPROVIDER_H
