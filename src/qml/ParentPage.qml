import QtQuick 1.1

Item {
    width: parent.width
    height: parent.height
    clip: false
    signal pageVisible
    signal pageHidden
    property string pageInitState: "PAGERIGHT"
    property string pageHideState: "PAGELEFT"
    property string pageState: "PAGERIGHT" // PAGELEFT, PAGECENTER, PAGETOP, PAGEBOTTOM
    x: {
        if(pageState == "PAGERIGHT")
           return width
        else if(pageState == "PAGELEFT")
            return -width
        else if(pageState == "PAGECENTER")
            return 0
        else return 0
    }
    y: {
        if(pageState == "PAGETOP")
           return -height
        else if(pageState == "PAGEBOTTOM")
            return height
        else if(pageState == "PAGECENTER") {
            return 0
        }
        else return 0
    }

    Behavior on y {
        enabled: visible // hide rotation glitch
        NumberAnimation {
            easing.type: Easing.OutCubic
            duration: 300
            onRunningChanged: {
                if(running) {
                } else {
                    if(y == 0) {
                        if(x == 0) {
                            pageVisible()
                            clip = false
                        }
                    } else if(y == height) {
                        visible = false
                        pageHidden()
                        if(pageHideState == "PAGETOP") {
                            for(var i = 0; i < children.length; ++i)
                                children[i].destroy() // destroy created page
                            destroy() // destroy parentPage
                        } else if(pageHideState == "PAGEBOTTOM") {

                        }
                    } else if(y == -height) {
                        if(x == 0) {
                            clip = true
                        }
                        pageHidden()
                        visible = false
                        if(pageHideState == "PAGETOP") {
                        } else if(pageHideState == "PAGEBOTTOM") {
                            for(var i = 0; i < children.length; ++i)
                                children[i].destroy() // destroy created page
                            destroy() // destroy parentPage
                        }
                    }
                }
            }
        }
    }

    Behavior on x {
        enabled: visible // hide rotation glitch
        NumberAnimation {
            easing.type: Easing.OutCubic
            duration: 300
            onRunningChanged: {
                if(running) {
                } else {
                    if(x == 0) {
                        if(y == 0)
                            pageVisible()
                    } else if(x == width) {
                        visible = false
                        pageHidden()
                        for(var i = 0; i < children.length; ++i)
                            children[i].destroy() // destroy created page
                        destroy() // destroy parentPage
                    } else if(x == -width) {
                        pageHidden()
                        visible = false
                    }


                }

            }
        }
    }

    property variant childArea: childArea

    Item {
        id: childArea
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: topBar.isBarVisible ? 56 : 0
            bottom: parent.bottom
        }
    }


    function destroyPage() {
        pageState = pageInitState
    }
    function hidePage(pageState) {
        clip = true
        pageHideState = pageState
        this.pageState = pageState

    }
    function showPage() {
        console.log(childArea.children[0].objectName + ", showPage")
        visible = true
        pageState = "PAGECENTER"
    }
}

