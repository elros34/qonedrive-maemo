var pageStack = new Array
var parentPageComponent = Qt.createComponent("ParentPage.qml")

function initPage(fileName) {

    var parentPage = parentPageComponent.createObject(mainPage, {"pageState" : "PAGECENTER"})
    var object = Qt.createComponent(fileName).createObject(parentPage.childArea)
    object.objectName = fileName
    pageStack.push(parentPage)
    parentPage.showPage()
    topBar.enableBackButton(false)
}

function createPage(fileName, initState, arguments) {
    var activePage = pageStack[pageStack.length - 1]
    var parentPage = parentPageComponent.createObject(mainPage, {"pageInitState" : initState,
                                                      "pageState" : initState})
    var object = Qt.createComponent(fileName).createObject(parentPage.childArea, arguments)
    pageStack.push(parentPage)
    activePage.hidePage(initState == "PAGERIGHT" ? "PAGELEFT" : (initState == "PAGEBOTTOM" ? "PAGETOP" : "PAGEBOTTOM"))
    parentPage.showPage()
    object.objectName = fileName
    topBar.enableBackButton(true)
    return object
}

function destroyTopPage() {

    if (pageStack.length != 0) {
        var parentObject = pageStack.pop()
        parentObject.destroyPage()
        pageStack[pageStack.length - 1].showPage()
        if (pageStack.length == 1)
            topBar.enableBackButton(false)
    }
}

function getTopPageObjName() {
    return pageStack[pageStack.length - 1].childArea.children[0].objectName
}

function qmlObjType(obj) {
    console.log(obj.toString().split("_QML")[0])
}
