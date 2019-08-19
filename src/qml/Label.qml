import QtQuick 1.1

Item {
    property alias text: tx.text
    property alias color: tx.color
    property alias pixelSize: tx.font.pixelSize
    property alias weight: tx.font.weight
    height:  30
    width:  tx.width
    Text {
        id: tx
        font.pixelSize: 22
        color: "white"
        font.family: mainFont.name
        font.weight: Font.Normal
        anchors {
            verticalCenter: parent.verticalCenter
        }
    }
}

