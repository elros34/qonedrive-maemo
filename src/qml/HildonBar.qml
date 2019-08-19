/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 * Copyright (C) 2019 elros34
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1

Item {
    width: parent.width
    height: 56
    Image {
        anchors.fill: parent
            fillMode: Image.Stretch
            smooth: true
            source: "file://etc/hildon/theme/images/wmTitleBar.png"
            opacity: 0.7
    }

    signal pressed
    MouseArea {
        anchors.fill: parent
        onClicked: topBar.pressed()
    }

    Item {
        x: 0
        id: task
        height: parent.height
        width: isPortrait  ? 80 : 112

        Image {
            id: taskIcon
            source: "file://etc/hildon/theme/images/wmTaskSwitcherIcon.png"
            anchors.centerIn: parent

            MouseArea {
                id: taskArea
                anchors.fill: parent
                onClicked: mainView.minimize();
            }

            states: State {
                name: "pressed"
                when: taskArea.pressed
                PropertyChanges {
                    target: taskIconPressed
                    visible: true
                }
            }
        }
        Image {
            id: taskIconPressed
            source: "file://etc/hildon/theme/images/wmTaskSwitcherIconPressed.png"
            visible: false
            anchors.centerIn: parent
        }
        Image {
            id: separator
            anchors{
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }
            source: "file://etc/hildon/theme/images/wmSeparator.png"
        }
    }

    Image {
        id: statusAreaImage
        anchors.left: task.right
        cache: false
        property int wid:  0

        MouseArea {
            anchors.fill: parent
            onClicked: mainView.showStatusMenu();
        }

        Connections {
            target: mainView
            onUpdateStatusAreaSignal: {
                statusAreaImage.source = ""
                statusAreaImage.source = "image://imageProvider/"+ wId
            }
        }
    }


    Image {
        id: separator1
        anchors{
            top: parent.top; bottom: parent.bottom; left: statusAreaImage.right
        }
        source: "file://etc/hildon/theme/images/wmSeparator.png"
    }


    Label {
        id: titleLabel
        text: "QOneDrive"
        anchors {
            verticalCenter: parent.verticalCenter
            left: separator1.right
            leftMargin: 20
        }
        color: "white"

    }

    Image {
        anchors {
            left: titleLabel.right
            verticalCenter: titleLabel.verticalCenter
        }
        source: "file://etc/hildon/theme/images/wmMenuIndicator.png"
    }

    Item {
        id: close
        height: parent.height
        anchors.right: parent.right

        width: isPortrait  ? 80 : 112
        Image {
            id: closeButton
            z: 11
            source: "file://etc/hildon/theme/images/wmCloseIcon.png"
            anchors.centerIn: parent
            MouseArea {
                id: closeArea
                anchors.fill: parent
                onClicked: {
                    mainView.close()
                }

            }
        }

        Image {
            id: backButton
            z: 11
            visible: false
            source: "file://etc/hildon/theme/images/wmBackIcon.png"
            anchors.centerIn: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainPage.destroyTopPage();
                    topBar.backButtonPressed()
                }
            }
        }

        Image {
            id: separator2
            anchors{
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }
            source: "file://etc/hildon/theme/images/wmSeparator.png"
        }
        Image {
            z: 10
            id: closeButtonPressed
            source: "file://etc/hildon/theme/images/wmRightButtonPressed.png"
            visible: closeArea.pressed
            anchors.centerIn: parent
            width: isPortrait  ? 80 : 112
        }
        Image {
            z: 11
            source: "file://etc/hildon/theme/images/wmCloseIconPressed.png"
            visible: closeArea.pressed
            anchors.centerIn: parent
            width: isPortrait  ? 80 : 112
        }
    }


    function enableBackButton(enable) {
        if(enable) {
            closeButton.visible = false
            backButton.visible = true
        } else {
            backButton.visible = false
            closeButton.visible = true
        }

    }
    function hide() {
        y = -height
        isBarVisible = false
    }
    function show() {
        y = 0
        isBarVisible = true
    }
    property bool isBarVisible: true
    signal backButtonPressed()
}
