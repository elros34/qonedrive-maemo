#include "downloadmanager.h"
#include <QFileInfo>

DownloadManager::DownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent) :
    QObject(parent)
{
    this->networkAccessManager = networkAccessManager;
    m_emptyQueue = true;
    downloadHead = -1;
    m_fileDownloaded = true;
    m_fileUploaded = true;
    uploadHead = -1;
}

DownloadManager::~DownloadManager()
{

}

void DownloadManager::downloadFile(const QString &name, const QString &id, const QString &type, const QString &size, const QString &created_time, const QString &updated_time, const QString &client_updated_time, const QString &path)
{
    downloadQueue.enqueue(new AppItem(
                      name,
                      id,
                      type,
                      size,
                      "Wait",
                      created_time,
                      updated_time,
                      client_updated_time,
                      0));

    pathQueue.enqueue(path);


    setEmptyQueue(false);

    qDebug() << "currentItem: " << downloadHead;
    if(m_fileDownloaded)
        doDownload();
}

void DownloadManager::uploadFile(const QString &filePath, const QString &uploadLocationId)
{
    qDebug() << "uploadFile: " << filePath << "\n" << uploadLocationId;

    QFileInfo fileInfo(filePath);

    if(fileInfo.size() >= 1024*1024*100) {
        qDebug() << "uploading files above 100MB size is not supported";
        showBanner("uploading files above 100MB size is not supported");
        return;
    }
    setEmptyQueue(false);

    uploadQueue.enqueue(new AppItem(
                            fileInfo.fileName(),
                            "",
                            "file",
                            sizeToString(fileInfo.size()),
                            "Wait",
                            "",
                            "",
                            "",
                            0));
    uploadPathsQueue.enqueue(qMakePair(filePath, uploadLocationId));

    if(m_fileUploaded)
        doUpload();
}

QAbstractListModel *DownloadManager::model()
{
    return &downloadQueue;
}

bool DownloadManager::emptyQueue() const
{
    return m_emptyQueue;
}

void DownloadManager::setEmptyQueue(bool emptyQueue)
{
    if(emptyQueue != m_emptyQueue) {
        m_emptyQueue = emptyQueue;
        emit emptyQueueChanged();
    }
}


void DownloadManager::showBanner(const QString &info)
{
    QDBusInterface interface("org.freedesktop.Notifications",
                             "/org/freedesktop/Notifications",
                             "org.freedesktop.Notifications",
                             QDBusConnection::sessionBus(),this);
    interface.asyncCall("SystemNoteInfoprint", info);
}

void DownloadManager::doUpload()
{
    m_fileUploaded = false;

    if(uploadHead == uploadQueue.count()-1) {
        m_fileUploaded = true;
        showBanner("All files uploaded");
        qDebug() << "all files uploaded";
        return;
    }

    uploadHead++;
    QString filePath = uploadPathsQueue.at(uploadHead).first;
    QString uploadLocationId = uploadPathsQueue.at(uploadHead).second;
    uploadQueue.at(uploadHead)->setCurrentState("Upload");

    currentUploadFile.setFileName(filePath);
    if(currentUploadFile.open(QIODevice::ReadOnly)) {
        QNetworkRequest request(QUrl("https://apis.live.net/v5.0/"+uploadLocationId+"/files/"+QFileInfo(filePath).fileName()+"?access_token="+accessToken));
        QNetworkReply *reply = networkAccessManager->put(request, &currentUploadFile);
        connect(reply, SIGNAL(finished()), this, SLOT(onFileUploaded()));
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(updateUploadProgess(qint64,qint64)));
    }
}


void DownloadManager::doDownload()
{
    m_fileDownloaded = false;

    if(downloadHead == downloadQueue.count()-1) {
        m_fileDownloaded = true;
        showBanner("All files downloaded");
        qDebug() << "all files downloaded";
        return;
    }

    downloadHead++;

    QNetworkRequest networkRequest(QUrl("https://apis.live.net/v5.0/"+downloadQueue.at(downloadHead)->id()+"/content?suppress_redirects=true&access_token="+accessToken));
    QNetworkReply *reply = networkAccessManager->get(networkRequest);
    connect(reply, SIGNAL(finished()), this, SLOT(onFileLocationReady()));
}

void DownloadManager::onFileLocationReady()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    bool ok;
    QJson::Parser parser;
    QVariantMap map = parser.parse(reply->readAll(), &ok).toMap();
    if(ok) {
        QString downloadLink = map.value("location").toString();
        downloadLink.remove(4,1); // https doesn't work on maemo's qt?
        qDebug() << "downloadLink: " << downloadLink;
        downloadQueue.at(downloadHead)->setCurrentState("Download");

        QFile file(pathQueue.at(downloadHead));
        if(file.open(QIODevice::WriteOnly)) {
            file.close();
        }
        currentDownloadFile.setFileName(pathQueue.at(downloadHead) + ".part");
        if(currentDownloadFile.open(QIODevice::WriteOnly)) {
        }

        QNetworkRequest networkRequest(downloadLink);
        QNetworkReply *downloadReply = networkAccessManager->get(networkRequest);
        connect(downloadReply, SIGNAL(finished()), this, SLOT(onFileDownloaded()));
        connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgess(qint64,qint64)));
        connect(downloadReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    reply->deleteLater();
}

void DownloadManager::onFileDownloaded()
{
    qDebug() << "onFileDownloaded: " << pathQueue.at(downloadHead);
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());


    QFile file(pathQueue.at(downloadHead));
    file.remove();
    currentDownloadFile.close();
    currentDownloadFile.rename(pathQueue.at(downloadHead));


    reply->deleteLater();
    doDownload();
}


void DownloadManager::onFileUploaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if(reply->error() == QNetworkReply::NoError) {
        bool ok = false;
        QJson::Parser parser;
        QVariantMap map = parser.parse(reply->readAll(), &ok).toMap();// id, name, source
        qDebug() << "fileuploaded map: " << map;
        if(ok) {
            qDebug() << "fileUploaded: " << map.value("name").toString();
            showBanner(map.value("name").toString() + " uploaded");
        }
    } else {
        qDebug() << "error string: " << reply->errorString() << " " << reply->error();
        showBanner("error, cannot upload " + uploadPathsQueue.at(uploadHead).first);
        qDebug() << "error, cannot upload " << uploadPathsQueue.at(uploadHead).first;
    }

    currentUploadFile.close();
    reply->deleteLater();
    doUpload();
}

void DownloadManager::updateDownloadProgess(qint64 bytesRead, qint64 totalBytes)
{
    downloadQueue.at(downloadHead)->setProgress(bytesRead*100/totalBytes);
    downloadQueue.atChanged(downloadHead);
}

void DownloadManager::updateUploadProgess(qint64 bytesRead, qint64 totalBytes)
{
    qDebug() << "upload progress: " << bytesRead*100/totalBytes;
    uploadQueue.at(uploadHead)->setProgress(bytesRead*100/totalBytes);
    uploadQueue.atChanged(uploadHead);
}

void DownloadManager::onReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    currentDownloadFile.write(reply->readAll());
}

QPair<QString, uchar> DownloadManager::itemProperties(const QString &fileId)
{
    for(int i = 0; i < downloadQueue.count(); ++i) {
        if(fileId == downloadQueue.at(i)->id()) {
            return qMakePair(downloadQueue.at(i)->currentState(), (uchar)downloadQueue.at(i)->progress());
        }
    }
    return QPair<QString, uchar>();
}

void DownloadManager::setAccessToken(const QByteArray &accessToken)
{
    this->accessToken = accessToken;
}

void DownloadManager::clearFinished()
{
    qDebug() << "clearFinished";
    if(m_fileDownloaded) {
        downloadQueue.clear();
        pathQueue.clear();
        downloadHead = -1;
    } else {
        for (int i = 0; i < downloadHead; ++i) {
            qDebug() << "i: " << i;
            qDebug() << downloadQueue.at(0)->name();
            downloadQueue.removeItem(0);
            pathQueue.removeAt(0);
        }
        downloadHead = 0;
    }

    if(m_fileUploaded) {
        uploadQueue.clear();
        uploadPathsQueue.clear();
        uploadHead = -1;
    } else {
        for (int i = 0; i < uploadHead; ++i) {
            qDebug() << "i: " << i;
            qDebug() << uploadQueue.at(0)->name();
            uploadQueue.removeItem(0);
            uploadPathsQueue.removeAt(0);
        }
        uploadHead = 0;
    }

    if(uploadHead == -1 && downloadHead == -1) {
        setEmptyQueue(true);
    }
}

QString DownloadManager::sizeToString(float size)
{
    QString sizeString;
    if(size/1024/1024/1024 >= 1) {
        sizeString = "GB";
        size = size/1024/1024/1024;
        size = double(int(size*100))/100;
    } else if(size/1024/1024 >= 1) {
        sizeString = "MB";
        size = size/1024/1024;
        size = double(int(size*100))/100;
    } else if(size/1024 >= 1) {
        sizeString = "KB";
        size = size/1024;
        size = double(int(size*100))/100;
    } else {
        sizeString = "B";
    }
    return sizeString.prepend(QString::number(size));
}
