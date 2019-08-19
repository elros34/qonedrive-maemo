#include <QApplication>
#include "core.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    Core a(argc, argv);

    return a.exec();
}
