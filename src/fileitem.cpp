#include "fileitem.h"

FileItem::FileItem(QObject *parent) :
    QObject(parent)
{
}

FileItem::FileItem(const QString &fileName, const QString &filePath, QObject *parent) : QObject(parent)
{
    _fileName = fileName;

    QFileInfo finfo(filePath);

    _filePath = finfo.absoluteFilePath();
    _isDir = finfo.isDir();
    int fileSize = finfo.size();
    _fileSize = fileSizeToString(fileSize);
}
QString FileItem::fileSizeToString(float fileSize)
{
    QString sizeString;
    if(fileSize/1024/1024 >= 1) {
        sizeString = "MB";
        fileSize = fileSize/1024/1024;
        fileSize = float(int(fileSize*100))/100;
    } else if(fileSize/1024 >= 1) {
        sizeString = "KB";
        fileSize = fileSize/1024;
        fileSize = float(int(fileSize*100))/100;
    } else {
        sizeString = "B";
    }
    return sizeString.prepend(QString::number(fileSize));
}

QString FileItem::fileName() const
{
    return _fileName;
}

QString FileItem::filePath() const
{
    return _filePath;
}

QString FileItem::fileSize() const
{
    return _fileSize;
}

bool FileItem::isDir() const
{
    return _isDir;
}
