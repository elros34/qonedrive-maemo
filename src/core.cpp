// This program or library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (c) 2010, Nokia Corporation and/or its subsidiary(-ies) <directui@nokia.com>
// Copyright (c) 2019, elros34


#include "core.h"

Core::Core(int &argc, char **argv) :
    QApplication(argc, argv)
{
    currentDirHelper = new CurrentDirHelper(argv[0], this);
    XSetErrorHandler(myXErrorHandler);

    mainView = new MainView();

    installEventFilter(this);
    mainView->showExpanded();

    XDamageQueryExtension(QX11Info::display(), &damage_event, &damage_error);
    _damage = XDamageCreate(QX11Info::display(), mainView->statusArea, XDamageReportNonEmpty);
}

Core::~Core()
{
    delete mainView;
    XDamageDestroy(QX11Info::display(), _damage);
}

bool Core::x11EventFilter(XEvent *event)
{
    if(event->type == damage_event + XDamageNotify)
    {
        const XDamageNotifyEvent *e = reinterpret_cast<const XDamageNotifyEvent*>(event);
        if(e->damage == _damage)
        {
            XDamageSubtract(QX11Info::display(), e->damage, None, None);
            mainView->updateStatusArea();
        }
    }

    return false;
}
