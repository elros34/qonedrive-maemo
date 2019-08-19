#include "downloadmodel.h"
#include <QDebug>

DownloadModel::DownloadModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    roles[StateRole] = "currentState";
    roles[ProgressRole] = "progress";
    roles[CreatedRole] = "created_time";
    roles[UpdatedRole] = "updated_time";
    roles[ClientUpdatedRole] = "client_updated_time";
    setRoleNames(roles);
}

DownloadModel::~DownloadModel()
{
    qDeleteAll(appList);
    appList.clear();
}

QVariant DownloadModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > appList.count())
        return QVariant();
    else if(role == IdRole)
    {
        return appList.at(index.row())->id();
    }
    else if(role == NameRole)
    {
        return appList.at(index.row())->name();
    }
    else if(role == TypeRole)
    {
        return appList.at(index.row())->type();
    }
    else if(role == SizeRole)
    {
        return appList.at(index.row())->size();
    }
    else if(role == StateRole)
    {
        return appList.at(index.row())->currentState();
    }
    else if(role == ProgressRole)
    {
        return appList.at(index.row())->progress();
    }
    else if(role == CreatedRole)
    {
        return appList.at(index.row())->created_time();
    }
    else if(role == UpdatedRole)
    {
        return appList.at(index.row())->updated_time();
    }
    else if(role == ClientUpdatedRole)
    {
        return appList.at(index.row())->client_updated_time();
    }
    else return QVariant();
}

int DownloadModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return appList.count();
}

void DownloadModel::append(AppItem *item)
{
    if(!appList.contains(item))
    {
        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
        appList.append(item);
        endInsertRows();
    }
}

void DownloadModel::enqueue(AppItem *item)
{
    if(!appList.contains(item))
    {
        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
        appList.enqueue(item);
        endInsertRows();
    }
}

AppItem *DownloadModel::last()
{
    return appList.last();
}

int DownloadModel::count()
{
    return appList.count();
}

AppItem* DownloadModel::head()
{
    return appList.head();
}

AppItem* DownloadModel::dequeue()
{
    return appList.dequeue();
}

AppItem *DownloadModel::at(int i)
{
    return appList.at(i);
}

void DownloadModel::clear()
{
    if(!appList.isEmpty())
    {
        beginResetModel();
        appList.clear();
        endResetModel();
    }
}

void DownloadModel::sortElements()
{
    beginResetModel();
    qSort(appList.begin(), appList.end(), compareAppItem);
    endResetModel();
}

void DownloadModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    delete appList.takeAt(i);
    endRemoveRows();
}

QString DownloadModel::getItemId(int index)
{
    if(index >= 0 && index < appList.count())
        return appList.at(index)->id();
    else return QString();
}

QString DownloadModel::getItemName(int index)
{
    if(index >= 0 && index < appList.count())
        return appList.at(index)->name();
    else return QString();
}

void DownloadModel::onDownloadProgressChanged(const QString &fileId, uchar percentage)
{
    for(int i = 0; i < appList.count(); ++i) {
          if(appList.at(i)->id() == fileId) {
              appList.at(i)->setProgress(percentage);
              emit dataChanged(index(i),index(i));
          }
    }

}

void DownloadModel::onStateChanged(const QString &fileId, const QString &state)
{

    for(int i = 0; i < appList.count(); ++i) {
          if(appList.at(i)->id() == fileId) {
              qDebug() << "onStateChanged: " << state;
              appList.at(i)->setCurrentState(state);
              emit dataChanged(index(i),index(i));
          }
    }
}

void DownloadModel::atChanged(int i)
{
    emit dataChanged(index(i), index(i));
}



