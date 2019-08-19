import QtQuick 1.1

Item {
    property alias text: tx.text
    property alias text2: tx2.text
    property alias color: tx.color
    height:  30
    width:  tx.width + tx2.width
    property alias textFocus: tx2.focus
    signal textFocusChanged()
    Label {
        id: tx
        color: "#b39573"
        anchors {
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: -5
            left: parent.left
            leftMargin: 10
        }
    }
    TextInput {
        id: tx2
        font.pixelSize: 22
        color: "white"
        font.family: mainFont.name
        font.weight: Font.Light
        selectByMouse: true
        width: isPortrait ? 220 : 280
        clip: true
        anchors {
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: -5
            left: tx.right
            leftMargin: 10
        }
        onActiveFocusChanged: parent.textFocusChanged(focus)
    }


    Rectangle {
        rotation: -90
        height: parent.width
        width: 1
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#ffffff"
            }
            GradientStop {
                position: 0.30;
                color: "#ffffff"
            }
            GradientStop {
                position: 1.00;
                color: "transparent";
            }
        }
        opacity: 0.6
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            horizontalCenterOffset: 5
            verticalCenterOffset: 13

        }
    }
}

