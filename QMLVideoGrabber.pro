QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    src/cc_grabber.cpp \
    src/cc_image.cpp \
    cc_qml_grabber.cpp \
    cc_qml_image_provider.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/cc_grabber.h \
    src/cc_image.h \
    cc_qml_grabber.h \
    cc_qml_image_provider.h
