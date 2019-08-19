import QtQuick 1.1

Item {
    property alias info: text1.text
    property alias text: text2.text
    property alias textInput: text2
    width: parent.width
    height: text1.height + text2.height
    Label {
        id: text1
        anchors {
            left: parent.left
        }
        text: "Name: "
        color: "#b39573"
    }

    TextEdit {
        id: text2
        font.pixelSize: 22
        wrapMode: Text.Wrap
        anchors {
            top: text1.bottom
        }
        color: "#fff0d7"
        width: parent.width
        selectByMouse: true
    }
}

