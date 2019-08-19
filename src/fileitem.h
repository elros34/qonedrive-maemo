#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QDebug>

class FileItem : public QObject
{
    Q_OBJECT
public:
    explicit FileItem(QObject *parent = 0);
    explicit FileItem(const QString &fileName, const QString &filePath, QObject *parent = 0);

    QString fileName() const;
    QString filePath() const;
    QString fileSize() const;
    bool isDir() const;

private:
    QString _fileName;
    QString _filePath;
    QString _fileSize;
    bool _isDir;
signals:

public slots:

private slots:
    QString fileSizeToString(float fileSize);
};

#endif // FILEITEM_H
