#ifndef CCQMLGRABBER_H
#define CCQMLGRABBER_H

#include <QObject>
#include <QTimer>
#include <QtDebug>

#include "src/cc_grabber.h"
#define TIMER_TIMEOUT 30

class CCQmlGrabber : public QObject
{
    Q_OBJECT
public:
    explicit CCQmlGrabber(QObject *parent = 0);

    ~CCQmlGrabber();

    // Initialise the video capture
    const int initialise_video();

    // Read frame
    const int read_frame();

    // Initialise the timer for video capture
    Q_INVOKABLE const int initialise_timer();

    // Get a pointer to the grabber
    inline CCGrabber *grabber_pt() {return Grabber_pt;}

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


//#define THIS
#ifdef THIS
// Live code
int a = 3;
#endif

#endif // CCQMLGRABBER_H
