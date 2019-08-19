// This program or library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.

// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.

// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (c) 2010, Nokia Corporation and/or its subsidiary(-ies) <directui@nokia.com>
// Copyright (c) 2019, elros34

#include "mainview.h"
#include <QApplication>
#include <QFileDialog>
#include "downloadmanager.h"
#include "downloadmodel.h"


MainView::MainView(QWidget *parent) :
    QDeclarativeView(parent)
{
#ifdef Q_WS_MAEMO_5
    _HILDON_WM_WINDOW_TYPE_STATUS_AREA = XInternAtom(QX11Info::display(), "_HILDON_WM_WINDOW_TYPE_STATUS_AREA", false);
#else
    _HILDON_WM_WINDOW_TYPE_STATUS_AREA = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DOCK", false);
#endif
    _NET_WM_WINDOW_TYPE = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", false);

    findStatusWindows();

    onOrientationChanged();
    onKeyboardSlideChanged();

    qmlRegisterType<FileModel>("FileModel", 1, 0, "FileModel");

    imageProvider = new ImageProvider();
    connect(imageProvider, SIGNAL(imageReady()), this, SLOT(onImageReady()));

    Settings *settings = new Settings(this);

    rootContext()->setContextProperty("Settings", settings);

#ifdef Q_WS_MAEMO_5
    bool isHildon = true;
#else
    bool isHildon = false;
#endif
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewport()->setAttribute(Qt::WA_NoSystemBackground);

    qRegisterMetaType<QAbstractListModel*>("QAbstractListModel*");
    qRegisterMetaType<DownloadManager*>("DownloadManager*");

    engine()->addImageProvider("imageProvider", imageProvider);
    rootContext()->setContextProperty("mainView", this);

    rootContext()->setContextProperty("appPath", CurrentDirHelper::applicationDirPath());
    oneDrive = new OneDrive(this);
    rootContext()->setContextProperty("oneDrive", oneDrive);
    rootContext()->setContextProperty("skyModel", oneDrive->skyModel);
    rootContext()->setContextProperty("downloadManager", oneDrive->downloadManager);
    rootContext()->setContextProperty("downloadModel", &oneDrive->downloadManager->downloadQueue);
    rootContext()->setContextProperty("uploadModel", &oneDrive->downloadManager->uploadQueue);

    NetworkManagerFactory *networkManagerFactory = new NetworkManagerFactory(this);
    engine()->setNetworkAccessManagerFactory(networkManagerFactory);

    isPressed = false;

    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    setSource(QUrl::fromLocalFile(QString::fromLatin1("/%1/%2").arg(
                                      CurrentDirHelper::applicationDirPath(), "qml/main.qml")));

    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(onOrientationChanged()));

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#else
    setAttribute(Qt::WA_AutoOrientation);
#endif

    QDBusConnection::systemBus().connect("com.nokia.mce",
                                         "/com/nokia/mce/signal",
                                         "com.nokia.mce.signal",
                                         "sig_device_orientation_ind",
                                         this, SLOT(onDBusOrientationChanged(const QString &)));

    QDBusConnection::systemBus().connect("org.freedesktop.Hal",
                                         "/org/freedesktop/Hal/devices/platform_slide",
                                         "org.freedesktop.Hal.Device",
                                         "PropertyModified",
                                         this, SLOT(onKeyboardSlideChanged()));
}

MainView::~MainView()
{
    imageProvider->deleteLater();
}

void MainView::showExpanded()
{
#if defined(Q_WS_MAEMO_5)
    showFullScreen();
#else
    show();
#endif
}

void MainView::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange) {
        if(isActiveWindow()) {
            _isVisible = true;
#ifdef Q_WS_MAEMO_5
        if(isPortrait)
            setAttribute(Qt::WA_Maemo5PortraitOrientation);
        else
            setAttribute(Qt::WA_Maemo5LandscapeOrientation);
#endif
    updateStatusArea();
        }
        else {
            _isVisible = false;
        }
    }
}

QVariant MainView::windowId()
{
    return QVariant::fromValue(this->winId());
}

void MainView::updateStatusArea()
{
    if(!isPressed && _isVisible)
        imageProvider->registerImage(statusArea);
}

void MainView::minimize()
{
    QDBusConnection::sessionBus().send(
                QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view"));
}

void MainView::showStatusMenu()
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = statusArea;
    ev.xclient.message_type = XInternAtom(QX11Info::display(), "HILDON_STATUS_MENU_SHOW", false);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = 1;
    XSendEvent(QX11Info::display(), statusArea, False, NoEventMask, &ev);
}

void MainView::findStatusWindows()
{
    Window root_return, parent_return, *children_return;
    uint n;

    if(XQueryTree(QX11Info::display(), QX11Info::appRootWindow(0), &root_return,
                  &parent_return, &children_return, &n) == 0)
        return;

    for(int i = n - 1; i >= 0; --i)
    {
        if(getWindowType(children_return[i]) == _HILDON_WM_WINDOW_TYPE_STATUS_AREA)
        {
            statusArea = children_return[i];
            break;
        }
    }

    XFree(children_return);
}

Atom MainView::getWindowType(const Window &wId)
{
    Atom type;
    ulong n;
    ulong remain;
    uchar *data = NULL;
    int format;

    if(XGetWindowProperty(QX11Info::display(), wId, _NET_WM_WINDOW_TYPE,
                          0, 1, false, XA_ATOM, &type, &format, &n, &remain, &data) != Success)
        return Atom();

    if(data != NULL)
    {
        Atom wtype = ((Atom*)data)[0];
        XFree(data);
        return wtype;
    }
    else return Atom();
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    QDeclarativeView::mousePressEvent(event);
    isPressed = true;
}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    QDeclarativeView::mouseReleaseEvent(event);
    isPressed = false;
}

void MainView::onDBusOrientationChanged(const QString &orientation)
{
    if(!isKeyboardOpen)
    {
        if(isActiveWindow())
        {
            isPortrait = (orientation == "portrait") ? true : false;
            rootContext()->setContextProperty("isPortrait", (orientation == "portrait"));
        }
        pendingOrientation.clear();
    }
    else pendingOrientation = orientation;
}

void MainView::onKeyboardSlideChanged()
{
    QFile file("/sys/devices/platform/gpio-switch/slide/state");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream st(&file);
    if(st.readLine() == "open")
    {
        isKeyboardOpen = true;
        bool _false = false;
        rootContext()->setContextProperty("isPortrait", _false);
        pendingOrientation = isPortrait ? "portrait" : "landscape";
    }
    else {
        isKeyboardOpen = false;
        if(!pendingOrientation.isEmpty())
            onDBusOrientationChanged(pendingOrientation);
    }
}

void MainView::onOrientationChanged()
{
    isPortrait = QApplication::desktop()->height() > QApplication::desktop()->width();

    rootContext()->setContextProperty("isPortrait", isPortrait);
    rootContext()->setContextProperty("isRealPortrait", isPortrait);

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation);
#endif

}

void MainView::onImageReady()
{
    emit updateStatusAreaSignal(statusArea);
}
