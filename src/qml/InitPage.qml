import QtQuick 1.1

Item {
    anchors.fill: parent

    Item {
        id: previewItem

        width: isPortrait ? parent.width : parent.width / 3
        height: isPortrait ? 300 : parent.height
        anchors {
            left: parent.left
            top: parent.top
        }

        Rectangle {
            width: 1
            height: parent.height
            rotation: isPortrait ? -90 : 0
            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "transparent"
                }
                GradientStop {
                    position: 0.50
                    color: "white"
                }
                GradientStop {
                    position: 1.00
                    color: "transparent"
                }
            }
            x: isPortrait ? parent.width/2 : (previewItem.width-2)
            y: isPortrait ? (previewItem.height - previewItem.height/2) : 0
        }

        Flickable {
            anchors.fill: parent
            contentWidth: previewColumn.width
            contentHeight: previewColumn.height + 10
            clip: isPortrait ? true : false
            Column {
                id: previewColumn
                width: previewItem.width
                spacing: 10

                TextInfo {
                    id: nameInfo
                    info: "Name:  "
                    text: "me/skydrive"
                    oneLine: false
                    width: parent.width
                }
                TextInfo {
                    id: typeInfo
                    info: "Type:  "
                    text: "folder"
                    oneLine: false
                }
                TextInfo {
                    id: sizeInfo
                    info: "Size:  "
                    oneLine: false
                    visible: false
                }
                TextInfo {
                    id: modificationInfo
                    info: "Modification Time: "
                    oneLine: false
                    visible: false
                }
                TextInfo {
                    id: createdInfo
                    info: "Created Time: "
                    oneLine: false
                    visible: false
                }

                Button2 {
                    id: downloadButton
                    text: "Download"
                    width: 180
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: false

                    onClicked: {
                        console.log("current index: " + repeater.currentIndex)
                        console.log(repeater.model.getItemId(repeater.currentIndex))

                        var obj = mainPage.createPage("FileSelector.qml", "PAGEBOTTOM", {"mode" : 1})
                        obj.setFileName(nameInfo.text, sizeInfo.text)
                        obj.openDir("/home/")
                        obj.fileSelected.connect(onFileLocationReady)
                        topBar.hide()
                    }
                    function onFileLocationReady(path) {
                        console.log("path: " + path)
                        if(path) {
                            downloadManager.downloadFile(nameInfo.text,
                                                         skyModel.get(repeater.currentIndex).id,
                                                         typeInfo.text,
                                                         sizeInfo.text,
                                                         skyModel.get(repeater.currentIndex).created_time,
                                                         skyModel.get(repeater.currentIndex).updated_time,
                                                         skyModel.get(repeater.currentIndex).client_updated_time,
                                                         path)
                        }

                        topBar.show()
                        mainPage.destroyTopPage()
                    }
                }

                Button2 {
                    id: uploadButton
                    text: "Upload file"
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 170
                    height: 70

                    onClicked: {
                        var obj = mainPage.createPage("FileSelector.qml", "PAGEBOTTOM", {"mode" : 0})
                        obj.openDir("/home/")
                        obj.fileSelected.connect(onUploadFileLocationReady)
                        topBar.hide()
                    }

                    function onUploadFileLocationReady(path) {
                        console.log("path: " + path)
                        if(path) {
                            console.log("onUploadFileLocationReady: "+path)
                            console.log("currentDirectoryId:" + oneDrive.currentDirectoryId())
                            downloadManager.uploadFile(path, oneDrive.currentDirectoryId())
                        }
                        topBar.show()
                        mainPage.destroyTopPage()
                    }
                }
            }
        }
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


    Flickable {
        id: flickable
        clip: isPortrait ? true : false
        anchors {
            bottom: parent.bottom
            right: parent.right
        }
        width: isPortrait ? parent.width : parent.width * 2 / 3
        height: isPortrait ? parent.height - previewItem.height : parent.height
        contentWidth: width
        contentHeight: listView.height + 1

        Column {
            id: listView
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Repeater {
                id: repeater
                width: parent.width
                property int currentIndex: 0
                property Item currentItem

                model: skyModel
                delegate: DFileItem {
                    id: fileItem
                    width: listView.width
                    onClicked: {
                        repeater.currentItem = fileItem
                        downloadButton.visible = false

                        if (name == "..") {
                            repeater.currentIndex = -1
                            oneDrive.folderUp()
                            nameInfo.text = oneDrive.currentDirectoryName()
                            typeInfo.text = "folder"
                            uploadButton.visible = true
                            modificationInfo.visible = false
                            createdInfo.visible = false
                        } else {
                            repeater.currentIndex = index
                            if (type == "folder" || type == "album") {
                                oneDrive.readFolderInfo(id, name)
                                sizeInfo.visible = false
                                uploadButton.visible = true
                            } else {
                                downloadButton.visible = true
                                uploadButton.visible = false
                                sizeInfo.visible = true
                                sizeInfo.text =  size

                            }
                            nameInfo.text =  name
                            typeInfo.text =  type
                            modificationInfo.text = client_updated_time
                            modificationInfo.visible = true
                            createdInfo.text = created_time
                            createdInfo.visible = true
                        }
                    }
                }
            }
        }
    }
}
