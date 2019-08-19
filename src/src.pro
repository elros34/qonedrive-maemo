QT       += core gui dbus declarative network webkit
LIBS += -lX11 -lXcomposite -lXdamage -lqjson

TARGET = qonedrive

TEMPLATE = app

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

SOURCES += \
    main.cpp \
    core.cpp \
    mainview.cpp \
    imageprovider.cpp \
    skymodel.cpp \
    appitem.cpp \
    networkmanagerfactory.cpp \
    filemodel.cpp \
    fileitem.cpp \
    downloadmanager.cpp \
    onedrive.cpp \
    currentdirhelper.cpp \
    pixmapconv.cpp \
    downloadmodel.cpp

HEADERS += \
    core.h \
    mainview.h \
    imageprovider.h \
    skymodel.h \
    appitem.h \
    networkmanagerfactory.h \
    filemodel.h \
    fileitem.h \
    settings.h \
    downloadmanager.h \
    onedrive.h \
    currentdirhelper.h \
    pixmapconv.h \
    downloadmodel.h



OTHER_FILES += \
    qml/main.qml \
    qml/HildonBar.qml \
    qml/Button.qml \
    qml/LoginView.qml \
    qml/mainHelper.js \
    qml/MainPage.qml \
    qml/ParentPage.qml \
    qml/InitPage.qml \
    qml/DownloadView.qml \
    qml/TextInfo.qml \
    qml/FileSelector.qml \
    qml/SearchBar.qml \
    qml/Button2.qml \
    qml/Label.qml \
    qml/TextEditInfo.qml \
    qml/FileItem.qml \
    qml/DFileItem.qml \
    qml/InputField.qml

maemo5 {
    target.path = /opt/qonedrive/

    qml.path = /opt/qonedrive/qml/
    qml.files += \
    qml/main.qml \

    INSTALLS += target  qml
}


OBJECTS_DIR = .obj
MOC_DIR = .moc
