#ifndef DOWNLOADMODEL_H
#define DOWNLOADMODEL_H

#include <QAbstractListModel>
#include <QQueue>
#include <QList>
#include "appitem.h"

class DownloadModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DownloadModel(QObject *parent = 0);

    ~DownloadModel();

    enum ListRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TypeRole,
        SizeRole,
        StateRole,
        ProgressRole,
        CreatedRole,
        UpdatedRole,
        ClientUpdatedRole
    };

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void append(AppItem *item);
    void enqueue(AppItem *item);
    AppItem* last();
    Q_INVOKABLE int count();
    AppItem *head();
    AppItem *dequeue();
    AppItem* at(int i);

    void clear();
    void sortElements();
    void removeItem(int i);


signals:

public slots:
    QString getItemId(int index);
    QString getItemName(int index);
    void onDownloadProgressChanged(const QString &fileId, uchar percentage);
    void onStateChanged(const QString &fileId, const QString &state);
    void atChanged(int i);

private:
    QQueue<AppItem*> appList;
    static bool compareAppItem(const AppItem *a, const AppItem *b)
    {
        return a->name().toLower() < b->name().toLower();
    }
};

#endif // DOWNLOADMODEL_H
