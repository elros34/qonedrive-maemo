#ifndef PIXMAPCONV_H
#define PIXMAPCONV_H

#include <QObject>
#include <QPixmap>

#include "qtextstream.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/XShm.h>
#include <QX11Info>

class PixmapConv : public QObject
{
public:
    PixmapConv(QObject *parent = 0);
    ~PixmapConv();
private:
    Pixmap xpixmap;


public slots:
    QPixmap getWindowPixmap(const uint wId);
};

#endif // PIXMAPCONV_H
