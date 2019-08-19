import QtQuick 1.1

Item {
    anchors.fill: parent

    Label {
        text: "Empty queue"
        pixelSize: 40
        anchors.centerIn: parent
        visible: downloadManager.emptyQueue
    }

    Rectangle {
        anchors{
            right: flickable.right
            rightMargin: 1
        }
        width: 1
        y: flickable.y + flickable.visibleArea.yPosition * flickable.height
        height: flickable.visibleArea.heightRatio * flickable.height
        opacity: 0.4
    }

    Button2 {
        id: clearButton
        text: "Clear finished"
        width: 180
        visible: !downloadManager.emptyQueue
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 2
        }
        onClicked: {
            downloadManager.clearFinished()
        }
    }

    Flickable {
        id: flickable
        clip: true
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: clearButton.top
        }

        contentWidth: width
        contentHeight: listView.height + 1

        Column {
            id: listView
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Label {
                text: "Download queue:"
                visible: !downloadManager.emptyQueue
            }

            Repeater {
                id: repeater
                width: parent.width
                property int currentIndex: 0
                property Item currentItem

                model: downloadModel

                Component.onCompleted: {
                    console.log("download maanger model count:" + downloadModel.count())
                }

                delegate: DFileItem {
                    id: fileItem
                    width: listView.width
                }
            }


            Label {
                text: "Upload queue:"
                visible: !downloadManager.emptyQueue
            }

            Repeater {
                id: uploadRepeater
                width: parent.width
                property int currentIndex: 0
                property Item currentItem

                model: uploadModel

                Component.onCompleted: {
                    console.log("download maanger model count:" + uploadModel.count())
                }
                delegate: DFileItem {
                    width: listView.width
                }
            }

        }
    }

}
