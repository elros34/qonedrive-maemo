#ifndef SKYMODEL_H
#define SKYMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "appitem.h"

class SkyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SkyModel(QObject *parent = 0);

    ~SkyModel();

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

    void clear();
    void sortElements();


signals:

public slots:
    QString getItemId(const int &index);
    QString getItemName(const int &index);
    void onDownloadProgressChanged(const QString &fileId, uchar percentage);
    void onStateChanged(const QString &fileId, const QString &state);
    QVariantMap get(int i) const;

private:
    QList<AppItem*> appList;
    static bool compareAppItem(const AppItem *a, const AppItem *b)
    {
        return a->name().toLower() < b->name().toLower();
    }
};

#endif // SKYMODEL_H
