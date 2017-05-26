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

    QScopedPointer<CCQmlGrabber> qml_grabber(new CCQmlGrabber());

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("qml_grabber", qml_grabber.data());
    engine.addImageProvider(QLatin1String("colors"), new CCQmlImageProvider);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
