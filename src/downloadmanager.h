#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <qjson/parser.h>
#include <QFile>
#include <QQueue>
#include <QPair>
#include <QtDBus>
#include <QDBusInterface>
#include <QDBusMessage>
#include "appitem.h"
#include "downloadmodel.h"

class DownloadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool emptyQueue READ emptyQueue WRITE setEmptyQueue NOTIFY emptyQueueChanged)
public:
    explicit DownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent = 0);
    ~DownloadManager();

    Q_INVOKABLE void downloadFile(const QString &name, const QString &id, const QString &type, const QString &size, const QString &created_time, const QString &updated_time, const QString &client_updated_time, const QString &path);
    Q_INVOKABLE void uploadFile(const QString &filePath, const QString &uploadLocationId);

    DownloadModel downloadQueue;
    DownloadModel uploadQueue;
    Q_INVOKABLE QAbstractListModel* model();
    bool emptyQueue() const;
    void setEmptyQueue(bool emptyQueue);

private:
    QNetworkAccessManager *networkAccessManager;
    QByteArray accessToken;
    QQueue<QString> pathQueue;
    QFile currentDownloadFile;
    QFile currentUploadFile;
    bool m_emptyQueue;
    int downloadHead;
    bool m_fileDownloaded;
    QQueue<QPair<QString, QString> > uploadPathsQueue;
    bool m_fileUploaded;
    int uploadHead;

signals:
    void downloadProgressChanged(const QString &fileId, uchar precentage);
    void stateChanged(const QString &fileId, const QString &state);
    void emptyQueueChanged();
public slots:

    QPair<QString, uchar> itemProperties(const QString &fileId);
    void setAccessToken(const QByteArray &accessToken);
    void clearFinished();
private slots:
    void onFileDownloaded();
    void onFileLocationReady();
    void updateDownloadProgess(qint64 bytesRead, qint64 totalBytes);
    void updateUploadProgess(qint64 bytesRead, qint64 totalBytes);
    void onReadyRead();
    void doDownload();
    void onFileUploaded();
    void showBanner(const QString &info);
    void doUpload();
    QString sizeToString(float size);
};

#endif // DOWNLOADMANAGER_H
