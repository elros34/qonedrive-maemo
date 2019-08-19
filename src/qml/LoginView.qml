import QtQuick 1.1
import QtWebKit 1.0

Rectangle {
    z: 24
    id: loginView
    anchors.fill: parent

    color: "black"

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: webView.height
        contentWidth: webView.width
        WebView {
            id: webView
            url: oneDrive.getLoginUrl() //"https://login.live.com"
            preferredWidth: loginView.width
            preferredHeight: loginView.height
            onLoadFinished: {
                if(webView.url.toString().indexOf("https://login.live.com/oauth20_authorize.srf") == 0 ){
                    webView.evaluateJavaScript("document.getElementById('i0116').focus()")
                    webView.evaluateJavaScript("document.getElementById('i0116').value = '" + Settings.value("login","login") + "'")
                    webView.focus = true
                } else if(webView.url.toString().indexOf("https://account.live.com/Consent/Update?") == 0 ){
                    flickable.contentY = flickable.contentHeight-flickable.height
                } else if(webView.url.toString().indexOf("https://login.live.com/oauth20_desktop.srf") == 0 ){
                    oneDrive.parseInput(webView.url.toString())
                }
            }
        }
    }
}
