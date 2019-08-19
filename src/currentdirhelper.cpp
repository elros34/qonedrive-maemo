#include "currentdirhelper.h"

CurrentDirHelper *CurrentDirHelper::m_currentDirHelper = NULL;


CurrentDirHelper::CurrentDirHelper(const QString &argv0, QObject *parent) :
    QObject(parent)
{
    m_currentDirHelper = this;
    m_applicationDirPath = QCoreApplication::applicationDirPath();
    qDebug() << "appPath: " << m_applicationDirPath;

    if(m_applicationDirPath.startsWith("./") || m_applicationDirPath == "/usr/bin") {
        m_applicationDirPath = argv0;
        m_applicationDirPath.chop(m_applicationDirPath.size() - m_applicationDirPath.lastIndexOf("/"));
    }
}

const CurrentDirHelper *CurrentDirHelper::instance()
{
    return m_currentDirHelper;
}

QString CurrentDirHelper::applicationDirPath()
{
    return instance()->m_applicationDirPath;
}
