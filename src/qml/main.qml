import QtQuick 1.1

Item {
    width: isRealPortrait ? 480 : 800
    height: isRealPortrait ? 800 : 480

    Item {
        id: rotatorItem
        width: isPortrait ? 480 : 800
        height: isPortrait ? 800 : 480
        anchors.centerIn: parent

        rotation: {
            if (isRealPortrait == isPortrait)
                return 0
            else if (!isRealPortrait && isPortrait)
                return -90
            else if (isRealPortrait && !isPortrait)
                return 90
        }
        HildonBar {
            z: 20
            id: topBar
        }
        MainPage {
            id: mainPage
            z: 10
        }
        Image {
            z: 5
            rotation: isPortrait ? 0 : -90
            source:  "images/wallpaper.png"
            anchors.centerIn: parent
        }
    }

    FontLoader {
        id: mainFont
        name: "Open Sans"
    }
}

