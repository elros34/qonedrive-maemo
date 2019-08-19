import QtQuick 1.1
import FileModel 1.0

Item {
    id: fileSelector
    width: parent.width
    height: parent.height
    property int mode: 0

    signal fileSelected(string path)
    property string selectedPath
    property alias path: pathLabel.text

    Flickable {
        id: pathFlickable
        clip: true
        contentWidth: tx.width + pathLabel.width + 25
        anchors {
            top: parent.top
            left: parent.left
            right: isPortrait ? parent.right : selector.left
        }
        height: 50

        Label {
            id: tx
            text: "Path:"
            color: "#b39573"
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }
        }
        Label {
            id: pathLabel
            color: "#fff0d7"
            text: ""
            anchors {
                verticalCenter: parent.verticalCenter
                left: tx.right
                leftMargin: 10
            }
        }
    }

    InputField {
        id: saveFileNameLabel
        visible: mode
        anchors {
            left: parent.left
            //leftMargin: 10
            top: pathFlickable.bottom
        }
        text: "File name:"
        text2: "tasdt.tar.gz"
    }

        Label {
            id: saveFileSizeLabel
            text: "Size:"
            visible: saveFileNameLabel.visible
            color: "#b39573"
            anchors {
                verticalCenter: saveFileNameLabel.verticalCenter
                verticalCenterOffset: -5
                left: saveFileNameLabel.right
                leftMargin: 30
            }
        }
        Label {
            id: saveFileSize
            visible: saveFileNameLabel.visible
            color: "#fff0d7"
            text: "48MB"
            anchors {
                verticalCenter: saveFileSizeLabel.verticalCenter
                left: saveFileSizeLabel.right
                leftMargin: 10
            }
        }


    GridView {
        id: flickable
        clip: true
        anchors {
            top: mode ? saveFileNameLabel.bottom : pathFlickable.bottom
            left: parent.left
            right: isPortrait ? parent.right : selector.left
            bottom: isPortrait ? selector.top : (searchBar.visible ? searchBar.top : parent.bottom)
        }
        model: fileModel
        cellWidth: 91
        cellHeight: 120


        MouseArea {
            id: flickableArea
            anchors.fill: parent
            visible: saveFileNameLabel.textFocus
            onPressed: {
                saveFileNameLabel.textFocus = false
                fileSelector.focus = true
            }
            preventStealing: false
        }

        delegate: FileItem {
            onClicked: {
                if (fileName == "..") {
                    // up
                    nameInfo.text = filePath.split("/").pop()
                } else {
                    nameInfo.text = fileName
                }

                sizeInfo.text = fileSize
                if (isDir) {
                    fileModel.openDir(filePath)
                    pathLabel.text = filePath
                    if (pathFlickable.contentWidth > pathFlickable.width)
                        pathFlickable.contentX = pathFlickable.contentWidth - pathFlickable.width

                    searchBar.text = ""
                } else {
                    if(mode == 1) {
                        saveFileNameLabel.text2 = fileName
                    }
                }
                selectedPath = filePath

            }
        }
    }

    Rectangle {
        anchors.right: flickable.right
        width: 1
        y: flickable.y + flickable.visibleArea.yPosition * flickable.height
        height: flickable.visibleArea.heightRatio * flickable.height
        opacity: 0.4
    }

    Item {
        id: selector
        width: isPortrait ? parent.width : 150
        height: isPortrait ? (searchBar.visible ? 330 : 280) : parent.height
        anchors {
            right: parent.right
            bottom: parent.bottom
        }


        Rectangle {
            anchors.fill: parent
            opacity: 0.05
        }
        Flickable {
            id: selectorFlickable
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: column2.top
            }
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: column1.height
            contentWidth: parent.width
            clip: true
            Column {
                id: column1
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                TextInfo {
                    id: nameInfo
                    info: "Name:  "
                }
                TextInfo {
                    id: sizeInfo
                    info: "Size:  "
                }
            }
        }

        Column {
            id: column2
            spacing: 4
            anchors {
                bottom: parent.bottom
                bottomMargin: (isPortrait && searchBar.visible) ? 60 : 10
                left: parent.left
                right: parent.right
            }
            Button2 {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottomMargin: 40
                }
                text: "Cancel"
                onClicked: {
                    selectedPath = ""
                    fileSelector.fileSelected("")
                }
            }
            Button2 {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottomMargin: 40
                }
                text: mode ? "Save" : "Select"
                onClicked: { // TODO file overwrite support
                    if(mode == 1) {
                        fileSelector.fileSelected(pathLabel.text + "/" + saveFileNameLabel.text2)
                    } else {
                        fileSelector.fileSelected(selectedPath)
                    }
                }
            }
        }
    }

    Keys.onPressed: {
        if(event.key != Qt.Key_Backspace && event.text != "") {
            searchBar.visible = true
            searchBar.text = event.text
        }

    }

    focus: true

    FileModel {
        id: fileModel
    }

    SearchBar {
        id: searchBar
        visible: false
        onTextChanged: {
            if(text == "")
                fileSelector.focus = true
            fileModel.search(text)
        }
        anchors {
            bottom: parent.bottom
            bottomMargin: 2
            right: isPortrait ? parent.right : selector.left
            rightMargin: isPortrait ? 0 : 3
            left: parent.left
            leftMargin: isPortrait ? 0 : 2
        }
    }

    function openDir(path) {
        console.log(path)
        fileModel.openDir(path)
        pathLabel.text = path
        sizeInfo.text = fileModel.fileSize(path)
        selectedPath = path
        nameInfo.text = path.split("/").pop()
    }
    function setFileName(name, size) {
        saveFileNameLabel.text2 = name
        saveFileSize.text = size
    }
}
