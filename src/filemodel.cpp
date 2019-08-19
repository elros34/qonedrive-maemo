#include "filemodel.h"
#include <QTime>

FileModel::FileModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "fileName";
    roles[PathRole] = "filePath";
    roles[IsDirRole] = "isDir";
    roles[SizeRole] = "fileSize";
    setRoleNames(roles);

    _current = 1;
    isHildon = false;

    filter.append("*.txt");
    filter.append("*.srt");

    QString dirPath;

#ifdef Q_WS_MAEMO_5
    dirPath = "/home/user/MyDocs/";
    isHildon = true;
#else
    dirPath = "/home/";
#endif

    fileDir.setSorting(QDir::DirsFirst);
    fileDir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDot);
}

FileModel::~FileModel()
{
    qDeleteAll(_fileList);
    _fileList.clear();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if(role == NameRole)
    {
        return _fileList.at(index.row())->fileName();
    }
    else if(role == PathRole)
    {
        return _fileList.at(index.row())->filePath();
    }
    else if(role == IsDirRole)
    {
        return _fileList.at(index.row())->isDir();
    }
    else if(role == SizeRole)
    {
        return _fileList.at(index.row())->fileSize();
    }
    else return QVariant();
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _fileList.count();
}

void FileModel::append(FileItem *item)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    _fileList.append(item);
    endInsertRows();
}

bool FileModel::isDir(const QString &filePath)
{
    QFileInfo info(filePath);
    return info.isDir();
}

bool FileModel::isDir(const int &index)
{
    return _fileList.at(index)->isDir();
}

void FileModel::openDir(const QString &path)
{
    if(path == "/..")
        return;


    fileDir.setPath(path);
    if(!fileDir.exists())
        fileDir.setPath("/");

    beginResetModel();
    qDeleteAll(_fileList);
    _fileList.clear();

    QStringList itemList = fileDir.entryList();

    for (int i = 0; i < itemList.count(); ++i) {
        _fileList.append(new FileItem(itemList.at(i), (fileDir.path() + "/" + itemList.at(i))));
    }
    endResetModel();
}

int FileModel::count()
{
    return _fileList.count();
}

void FileModel::setCurrent(const int &index)
{
    _current = index;
}

int FileModel::getCurrent()
{
    return _current;
}

QString FileModel::filePathAt(const int &index)
{
    if(index >= 0 && index < count())
        return _fileList.at(index)->filePath();
    else return QString();
}

void FileModel::search(const QString &text)
{
    beginResetModel();
    qDeleteAll(_fileList);
    _fileList.clear();
    endResetModel();

    QStringList itemList = fileDir.entryList();

    for (int i = 0; i < itemList.count(); ++i) {
        if(text.count() == 1) {
            if(itemList.at(i).startsWith(text, Qt::CaseInsensitive)) {
                append(new FileItem(itemList.at(i), (fileDir.path() + "/" + itemList.at(i))));
            }
        } else {
            if(itemList.at(i).contains(text, Qt::CaseInsensitive)) {
                append(new FileItem(itemList.at(i), (fileDir.path() + "/" + itemList.at(i))));
            }
        }
    }
}

QString FileModel::fileSize(const QString &path)
{
    QFileInfo finfo(path);
    int fileSize = finfo.size();
    if(fileSize/1000/1000 >= 1)
        return QString::number(fileSize/1000/1000) + "MB";
    else if(fileSize/1000 >= 1) {
        return QString::number(fileSize/1000) + "KB";
    }
    else {
        return QString::number(fileSize) + "B";
    }
}
