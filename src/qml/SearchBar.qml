import QtQuick 1.1

Item {
    property alias text: txinput.text
    property alias textInput: txinput
    signal textChanged(string text)
    signal accepted()
    width: 250
    height: 40

    TextInput {
        id: txinput
        color: "white"
        font.pixelSize: 24
        font.family: mainFont.name

        anchors {
            left: image.right
            leftMargin: 12
            right: parent.right
            rightMargin: 5
            verticalCenter: parent.verticalCenter
        }
//        focus: true // slowdown
        focus: parent.visible
        onAccepted: {
            if(txinput.text != "") {
                parent.accepted()
            }
        }
        onTextChanged: {
            if(text == "")
                parent.visible = false
            parent.textChanged(text)
        }
    }

    Image {
        id: image
        asynchronous: true
        source: "images/search.png"
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 6
        }
    }
    Rectangle {
        height: parent.height - 14
        color: "white"
        width: 1
        opacity: 0.8
        anchors {
            left: image.right
            leftMargin: 4
            verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        color: "transparent"
        border.color: "#b39573"
        anchors.fill: parent
        opacity: 0.7
    }
    Rectangle {
        anchors.fill: parent
        opacity: 0.1
    }
}

