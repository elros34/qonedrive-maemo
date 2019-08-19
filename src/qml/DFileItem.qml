import QtQuick 1.1

Item {
    signal clicked
    height: 60

    Image {
        id: image
        source: "file://" + appPath + "/qml/images/" + type + ".png"
        z: 10
    }
    Label {
        z: 10
        anchors {
            verticalCenter: parent.verticalCenter
            left: image.right
            leftMargin: 10
            verticalCenterOffset: -9
        }
        color: "white"
        text: name
    }
    Label {
        id: datetimeLabel
        anchors {
            left: image.right
            leftMargin: 10
            bottom: parent.bottom
            bottomMargin: 2
        }
        color: "white"
        text: client_updated_time
        pixelSize: 14
        weight: Font.Light
    }
    Label {
        anchors {
            left: datetimeLabel.right
            leftMargin: 30
            bottom: parent.bottom
            bottomMargin: 2
        }
        color: "white"
        text: size
        pixelSize: 14
        weight: Font.Light
    }

    MouseArea {
        anchors.fill: parent
        onPressed: pressRec.visible = true
        onClicked: parent.clicked()
        onExited: pressRec.visible = false
        onCanceled: pressRec.visible = false
        onReleased: pressRec.visible = false
    }
    Rectangle {
        id: pressRec
        anchors.fill: parent
        opacity: 0.4
        visible: false
        color: "#a67642"
    }

    Rectangle {
        id: progressRectangle
        color: "#1d6864"
        opacity: 0.5
        z: 5
        height: parent.height
        width: (progress * parent.width)/100
        visible: (currentState == "Download")
        Behavior on width {
            enabled: progressRectangle.visible && (progress != 100)
            NumberAnimation{}}
    }
    Rectangle {
        id: progressRectangle2
        color: "#8f9735"
        opacity: 0.5
        z: 5
        height: parent.height
        width: (progress * parent.width)/100
        visible: (currentState == "Upload")
        anchors.right: parent.right
        Behavior on width {
            enabled: progressRectangle2.visible && (progress != 100)
            NumberAnimation{}}
    }
}
