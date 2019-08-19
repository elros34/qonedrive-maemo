#ifndef CORE_H
#define CORE_H

#include <QApplication>

#include "mainview.h"
#include "currentdirhelper.h"

#include <X11/extensions/Xdamage.h>

class Core : public QApplication
{
    Q_OBJECT
public:
    explicit Core(int &argc, char **argv);
    ~Core();

    static int myXErrorHandler( Display*, XErrorEvent*) {
        return 1;
    }

    bool x11EventFilter(XEvent *event);
    MainView *mainView;

    Damage _damage;
    int damage_event, damage_error;
    Window wId;

	CurrentDirHelper *currentDirHelper;
public slots:

};

#endif // CORE_H
