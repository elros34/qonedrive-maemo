#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include "fileitem.h"

class FileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileModel(QObject *parent = 0);

    ~FileModel();

    enum ListRoles {
        NameRole = Qt::UserRole +1,
        PathRole,
        IsDirRole,
        SizeRole
    };

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void append(FileItem *item);
    Q_INVOKABLE bool isDir(const QString &filePath);
    Q_INVOKABLE bool isDir(const int &index);
    Q_INVOKABLE void openDir(const QString &path);
    Q_INVOKABLE int count();
    Q_INVOKABLE void setCurrent(const int &index);
    Q_INVOKABLE int getCurrent();
    Q_INVOKABLE QString filePathAt(const int &index);
    Q_INVOKABLE void search(const QString &text);
    Q_INVOKABLE QString fileSize(const QString &path);



private:
    bool isHildon;
    QDir fileDir;
    QList<FileItem*> _fileList;
    int _current;
    QStringList filter;

public slots:

};

#endif // FILEMODEL_H
