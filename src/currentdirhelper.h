#ifndef CURRENTDIRHELPER
#define CURRENTDIRHELPER

#include <QObject>
#include <QCoreApplication>
#include <QDebug>

class CurrentDirHelper : public QObject
{
    Q_OBJECT

public:
    explicit CurrentDirHelper(const QString &argv0, QObject *parent = 0);

    static const CurrentDirHelper *instance();
    static QString applicationDirPath();
    static CurrentDirHelper *m_currentDirHelper;

private:
    QString m_applicationDirPath;

};

#endif // CURRENTDIRHELPER

