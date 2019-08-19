import QtQuick 1.1

Item {
    property alias text: txt.text
    signal clicked
    width: 140
    height: 62


    Rectangle {
        anchors.fill: parent
        color: "white"
        opacity: 0.1
        border.color: "transparent"
        radius: 5
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "orange"
        opacity: 0.4
        radius: 5
    }

    Rectangle {
        color: "#9c845c"
        anchors.fill: parent
        visible: mouseArea.pressed
        opacity: 0.4
        radius: 5
    }

    Label {
        id: txt
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
        onPressed: txt.color = "#e4dbdb"
        onReleased: txt.color = "white"
    }
}
