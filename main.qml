import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.5

ApplicationWindow {
    id: appWindow

    visible: true
    width: 640 * 2
    height: 480 * 2
    title: qsTr("QMLVideoGrabber")

    color: "black"

    property string camera0_input_device: "/dev/video0"
    property string camera1_input_device: "/dev/video1"
    property string camera2_input_device: "/dev/video2"
    property string camera3_input_device: "/dev/video3"

    //property Camera camera

    Camera {
        id: camera0
        captureMode: Camera.CaptureStillImage
        //deviceId: camera0_input_device

        imageCapture {
            onImageCaptured: {
                stillControls.previewAvailable = true
            }
        }

    }

    Camera {
        id: camera1
        captureMode: Camera.CaptureStillImage
        //deviceId: camera1_input_device

        imageCapture {
            onImageCaptured: {
                stillControls.previewAvailable = true
            }
        }

    }

    Camera {
        id: camera2
        captureMode: Camera.CaptureStillImage
        //deviceId: camera2_input_device

        imageCapture {
            onImageCaptured: {
                stillControls.previewAvailable = true
            }
        }

    }

    Camera {
        id: camera3
        captureMode: Camera.CaptureStillImage
        //deviceId: camera3_input_device

        imageCapture {
            onImageCaptured: {
                stillControls.previewAvailable = true
            }
        }

    }

    VideoOutput {
        id: viewFinder0
        visible: true

        x:0
        y:0
        width: 640
        height: 480
        source: camera0
        autoOrientation: true

    }

    VideoOutput {
        id: viewFinder1
        visible: true

        x:640
        y:0
        width: 640
        height: 480
        source: camera1
        autoOrientation: true

    }

    VideoOutput {
        id: viewFinder2
        visible: true

        x:0
        y:480
        width: 640
        height: 480
        source: camera2
        autoOrientation: true

    }

    VideoOutput {
        id: viewFinder3
        visible: true

        x:640
        y:480
        width: 640
        height: 480
        source: camera3
        autoOrientation: true

    }

    Column {

        anchors {
            left: parent.left
            top: parent.top
            margins: 8
        }

        id: btns_column0
        spacing: 8

        Label {
            text: camera0_input_device
            color: "white"
        }

        Button {
            text: "/dev/video0"
            onClicked: {
                camera0_input_device = "/dev/video0"
                camera0.deviceId = camera0_input_device
            }
        }

        Button {
            text: "/dev/video1"
            onClicked: {
                camera0_input_device = "/dev/video1"
                camera0.deviceId = camera0_input_device
            }
        }

        Button {
            text: "/dev/video2"
            onClicked: {
                camera0_input_device = "/dev/video2"
                camera0.deviceId = camera0_input_device
            }
        }

        Button {
            text: "/dev/video3"
            onClicked: {
                camera0_input_device = "/dev/video3"
                camera0.deviceId = camera0_input_device
            }
        }

        Button {
            id: bnt_quit0
            text: "Quit"
            onClicked: {
                Qt.quit()
            }
        }
    }

    Column {

        anchors {
            right: parent.right
            top: parent.top
            margins: 8
        }

        id: btns_column1
        spacing: 8

        Label {
            text: camera1_input_device
            color: "white"
        }

        Button {
            text: "/dev/video0"
            onClicked: {
                camera1_input_device = "/dev/video0"
                camera1.deviceId = camera1_input_device
            }
        }

        Button {
            text: "/dev/video1"
            onClicked: {
                camera1_input_device = "/dev/video1"
                camera1.deviceId = camera1_input_device
            }
        }

        Button {
            text: "/dev/video2"
            onClicked: {
                camera1_input_device = "/dev/video2"
                camera1.deviceId = camera1_input_device
            }
        }

        Button {
            text: "/dev/video3"
            onClicked: {
                camera1_input_device = "/dev/video3"
                camera1.deviceId = camera1_input_device
            }
        }

        Button {
            id: bnt_quit1
            text: "Quit"
            onClicked: {
                Qt.quit()
            }
        }
    }

    Column {

        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: 8
        }

        id: btns_column2
        spacing: 8

        Label {
            text: camera2_input_device
            color: "white"
        }

        Button {
            text: "/dev/video0"
            onClicked: {
                camera2_input_device = "/dev/video0"
                camera2.deviceId = camera2_input_device
            }
        }

        Button {
            text: "/dev/video1"
            onClicked: {
                camera2_input_device = "/dev/video1"
                camera2.deviceId = camera2_input_device
            }
        }

        Button {
            text: "/dev/video2"
            onClicked: {
                camera2_input_device = "/dev/video2"
                camera2.deviceId = camera2_input_device
            }
        }

        Button {
            text: "/dev/video3"
            onClicked: {
                camera2_input_device = "/dev/video3"
                camera2.deviceId = camera2_input_device
            }
        }

        Button {
            id: bnt_quit2
            text: "Quit"
            onClicked: {
                Qt.quit()
            }
        }

    }

    Column {

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 8
        }

        id: btns_column3
        spacing: 8

        Label {
            text: camera3_input_device
            color: "white"
        }

        Button {
            text: "/dev/video0"
            onClicked: {
                camera3_input_device = "/dev/video0"
                camera3.deviceId = camera3_input_device
            }
        }

        Button {
            text: "/dev/video1"
            onClicked: {
                camera3_input_device = "/dev/video1"
                camera3.deviceId = camera3_input_device
            }
        }

        Button {
            text: "/dev/video2"
            onClicked: {
                camera3_input_device = "/dev/video2"
                camera3.deviceId = camera3_input_device
            }
        }

        Button {
            text: "/dev/video3"
            onClicked: {
                camera3_input_device = "/dev/video3"
                camera3.deviceId = camera3_input_device
            }
        }

        Button {
            id: bnt_quit3
            text: "Quit"
            onClicked: {
                Qt.quit()
            }
        }

    }


    /*
    CameraListButton {
        model: QtMultimedia.availableCameras
        onValueChanged: captureControls.camera.deviceId = value
    }
    */

    /*
    Row
    {
        Button {
            id: btn_open_video
            text: "Start C++ timer"
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
                image.source = "image://colors/yellow"
            }
        }

        Image {
            id: image
            source: "image://colors/yellow"
            cache : false
        }
        //Image { source: "image://colors/red" }

        Timer {
        interval: 30
        repeat: true
        running: true
        onTriggered: {
            //image.update()
            //image.height = 300
            image.source = "image://colors/yellow" + interval
            btn_update_video.text = "1000"
            }
        }

        Timer {
        interval: 30
        repeat: true
        running: true
        onTriggered: {
            //image.source = ""
            //image.height = 500
            image.source = "image://colors/yellow"
            btn_update_video.text = "700"
            }
        }

    }
    */

}

