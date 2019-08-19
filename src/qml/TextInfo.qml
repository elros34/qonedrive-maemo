import QtQuick 1.1

Item {
    property alias info: text1.text
    property alias text: text2.text
    property bool oneLine: false
    width: parent.width
    height: oneLine ? text1.height : (text1.height + text2.height)
    Label {
        id: text1
        anchors {
            left: parent.left
        }
        text: "Name: "
        color: "#b39573"
    }

    Text {
        id: text2
        font.pixelSize: 22
        wrapMode: Text.Wrap
        anchors {
            top: oneLine ? undefined : text1.bottom
            left: oneLine ? text1.right : undefined
        }
        color: "#fff0d7"
        width: parent.width
        font.family: mainFont.name
    }
}

