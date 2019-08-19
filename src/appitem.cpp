#include "appitem.h"

AppItem::AppItem(QObject *parent) :
    QObject(parent)
{
}

AppItem::AppItem(const QString &name, const QString &id, const QString &type, const QString &size, const QString &currentState, const int &progress, /*, const QString &icon, const QString &section,*/ QObject *parent)
    : QObject(parent), _name(name), _id(id), _type(type), _size(size), _currentState(currentState), _progress(progress)
{
}

AppItem::AppItem(const QString &name, const QString &id, const QString &type, const QString &size, const QString &currentState, const QString &created_time, const QString &updated_time, const QString &client_updated_time, const int &progress, /*, const QString &icon, const QString &section,*/ QObject *parent)
    : QObject(parent), _name(name), _id(id), _type(type), _size(size), _currentState(currentState), _progress(progress), m_created_time(created_time), m_updated_time(updated_time), m_client_updated_time(client_updated_time)
{
}

QString AppItem::name() const
{
    return _name;
}

void AppItem::setName(const QString &name)
{
    if(name != _name)
    {
        _name = name;
        emit nameChanged();
    }
}

QString AppItem::id() const
{
    return _id;
}

void AppItem::setId(const QString &id)
{
    if(id != _id)
    {
        _id = id;
        emit idChanged();
    }
}

QString AppItem::type() const
{
    return _type;
}

void AppItem::setType(const QString &type)
{
    if(type != _type)
    {
        _type = type;
        emit typeChanged();
    }
}

QString AppItem::size() const
{
    return _size;
}

void AppItem::setSize(const QString &size)
{
    if(size != _size)
    {
        _size = size;
        emit sizeChanged();
    }
}

QString AppItem::currentState() const
{
    return _currentState;
}


void AppItem::setCurrentState(const QString &currentState)
{
    if(currentState != _currentState)
    {
        _currentState = currentState;
        emit currentStateChanged();
    }
}

int AppItem::progress() const
{
    return _progress;
}

void AppItem::setProgress(const int &progress)
{
    if(progress != _progress) {
        _progress = progress;
        emit progressChanged();
    }
}
QString AppItem::created_time() const
{
    return m_created_time;
}

void AppItem::setCreated_time(const QString &created_time)
{
    m_created_time = created_time;
}
QString AppItem::updated_time() const
{
    return m_updated_time;
}

void AppItem::setUpdated_time(const QString &updated_time)
{
    m_updated_time = updated_time;
}
QString AppItem::client_updated_time() const
{
    return m_client_updated_time;
}

void AppItem::setClient_updated_time(const QString &client_updated_time)
{
    m_client_updated_time = client_updated_time;
}



