import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 960
    title: qsTr("QMLVideoGrabber")

    Row
    {
        Button {
            id: btn_open_video
            text: "Open video"
            onClicked:
            {
                qml_grabber.initialise_timer()
            }
        }

        Button {
            id: btn_update_video
            text: "Update video"
            onClicked:
            {

            }
        }

        Image { source: "image://colors/yellow"}
        //Image { source: "image://colors/red" }

    }

}

