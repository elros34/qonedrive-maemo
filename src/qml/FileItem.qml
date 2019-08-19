import QtQuick 1.1

Item {
    width: 91
    signal clicked
    height: image.height + 60

    Image {
        id: image
        source: isDir ? "images/folder.png" : "images/file.png"
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        anchors {
            top: image.bottom
            horizontalCenter: parent.horizontalCenter
        }
        width: 80
        height: 44
        color: "white"
        font.family: mainFont.name
        font.weight: Font.Normal
        text: fileName
        clip: true
        wrapMode: Text.Wrap
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
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
    }
}
