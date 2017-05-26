#ifndef CCQMLGRABBER_H
#define CCQMLGRABBER_H

#include <QObject>
#include <QTimer>
#include <QtDebug>

#include "src/cc_grabber.h"
#define TIMER_TIMEOUT 1000

class CCQmlGrabber : public QObject
{
    Q_OBJECT
public:
    explicit CCQmlGrabber(QObject *parent = 0);

    ~CCQmlGrabber();

    // Initialise the video capture
    Q_INVOKABLE const int initialise_video();

signals:

public slots:

    // In charge of capturing a frame from video
    void trigger_frame_capture();

protected:

    // A pointer to the C++ grabber class
    CCGrabber *Grabber_pt;

    // Indicated whether the grabber object has been initialised
    bool Initialised_grabber_object;

    // In charge of capturing the image
    QTimer *Timer;

};

#endif // CCQMLGRABBER_H
