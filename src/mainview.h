#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "qtextstream.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDesktopWidget>
#include <QtDeclarative/QDeclarativeComponent>
#include <QMouseEvent>
#include <QtDBus>

#include "imageprovider.h"
#include "skymodel.h"
#include "onedrive.h"
#include "networkmanagerfactory.h"
#include "filemodel.h"
#include "settings.h"
#include "downloadmanager.h"
#include "currentdirhelper.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>

#include <QX11Info>

class MainView : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

    void showExpanded();
    void changeEvent(QEvent *event);
    bool _isVisible;

    Q_INVOKABLE QVariant windowId();

    Window statusArea;
    ImageProvider *imageProvider;
    bool isKeyboardOpen;
    QString pendingOrientation;
    bool isPortrait;
    OneDrive *oneDrive;


signals:
    void updateStatusAreaSignal(int wId);

public slots:
    void updateStatusArea();
    void minimize();
    void showStatusMenu();
    void onImageReady();

private slots:
    void findStatusWindows();
    Atom getWindowType(const Window &wId);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void onDBusOrientationChanged(const QString &orientation);
    void onKeyboardSlideChanged();
    void onOrientationChanged();

private:

    Atom _HILDON_WM_WINDOW_TYPE_STATUS_AREA;
    Atom _NET_WM_WINDOW_TYPE;
    bool isPressed;
};

#endif // MAINVIEW_H
