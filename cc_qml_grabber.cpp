#include "cc_qml_grabber.h"

CCQmlGrabber::CCQmlGrabber(QObject *parent) :
    QObject(parent),
    Initialised_grabber_object(false)
{
    // Initialise grabber object
    string device = "/dev/video0";
    string format = "NTSC";

    Grabber_pt = new CCGrabber(device, format);
    Initialised_grabber_object = true;

    // Initiliase qtimer
    Timer = new QTimer(this);

    // Connect timer signals with this class slot
    connect(Timer, SIGNAL(timeout()), this, SLOT(trigger_frame_capture()));

}

CCQmlGrabber::~CCQmlGrabber()
{
    if (Initialised_grabber_object)
    {
        delete Grabber_pt;
        Grabber_pt = NULL;
    }

}

const int CCQmlGrabber::initialise_video()
{
    if (!Initialised_grabber_object)
    {
        return 0;
    }

    // Try to initialise video
    bool initialised = Grabber_pt->initialise_video();
    // Only start timer if video was correctly initialised
    if (initialised)
    {
        Timer->start(TIMER_TIMEOUT);
    }

    return initialised;

}

void CCQmlGrabber::trigger_frame_capture()
{
    qDebug() << "Before Grabber_pt->read_frame()";

    // Read a frame from the grabber
    Grabber_pt->read_frame();

    qDebug() << "After Grabber_pt->read_frame()";

    //QImage::Format_RGB32
    // Use QImage::Format_RGB888 for a 3 channels image
    //QImage qimage(Grabber_pt->image_pt(), Grabber_pt->width(), Grabber_pt->height(), Grabber_pt->bpl(), QImage::Format_RGB888);
    //ui->lbl_image->setPixmap(QPixmap::fromImage(qimage));

}
