#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>

#include "cc_qml_grabber.h"
#include "cc_qml_image_provider.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    CCQmlGrabber *qml_grabber_pt = new CCQmlGrabber();
    qml_grabber_pt->initialise_video();
    qml_grabber_pt->read_frame();

    QScopedPointer<CCQmlGrabber> qml_grabber(qml_grabber_pt);

    CCQmlImageProvider *qml_image_provider_pt = new CCQmlImageProvider();

    // Pass the pointer of the grabber to the image provider
    qml_image_provider_pt->set_grabber_pt(qml_grabber->grabber_pt());

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("qml_grabber", qml_grabber.data());

    engine.addImageProvider(QLatin1String("colors"), qml_image_provider_pt);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
