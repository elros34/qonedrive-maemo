import QtQuick 1.1

Item {
    property alias text: txt.text
    signal clicked
    width: 140
    height: 62


    Rectangle {
        anchors {
            bottom: parent.bottom
        }
        width: parent.width
        height: 1
        smooth: true
        opacity: 0.5

    }
    Rectangle {
        color: "#9c845c"
        anchors.fill: parent
        visible: mouseArea.pressed
        opacity: 0.4
    }
    Text {
        id: txt
        anchors.centerIn: parent
        color: "white"
        font.family: mainFont.name
        font.weight: Font.Light
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
        onPressed: txt.color = "#e4dbdb"
        onReleased: txt.color = "white"
    }
}
