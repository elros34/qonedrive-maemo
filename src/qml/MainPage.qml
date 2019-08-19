import QtQuick 1.1
import "mainHelper.js" as MainHelper

Item {
    id: mainPage
    width: parent.width
    height: parent.height
    property string initState: "PAGERIGHT"

    function createPage(fileName, initState, arguments) {
        return MainHelper.createPage(fileName, initState, arguments)
    }

    function destroyTopPage() {
        MainHelper.destroyTopPage()
    }

    function getTopPageObjName() {
        return MainHelper.getTopPageObjName();
    }

    Component.onCompleted: {
        MainHelper.initPage("InitPage.qml")
        topBar.pressed.connect(showAbout)
    }

    function showAbout() {
        mainPage.createPage("DownloadView.qml", "PAGEBOTTOM", {})
    }
    Connections {
        target: oneDrive
        onValidateInputCompleted: {
            if(mainPage.getTopPageObjName() == "LoginView.qml")
                mainPage.destroyTopPage()
        }
        onShowLoginViewSignal: {
            mainPage.createPage("LoginView.qml", "PAGERIGHT", {})
        }
    }
}

