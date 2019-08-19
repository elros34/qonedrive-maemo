/***************************************************************************
**
** Copyright (C) 2014 Joscor Technical Research
** Copyright (C) 2019 elros34
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
**
****************************************************************************/


#include "onedrive.h"
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <qjson/parser.h>


OneDrive::OneDrive(QObject *parent) :
    QObject(parent)
{
    networkAccessManager = new QNetworkAccessManager(this);
    skyModel = new SkyModel(this);
    downloadManager = NULL;
    m_tokenTimerRequest = false;
    m_validateInitCompleted = false;
    QTimer::singleShot(0, this, SLOT(initOneDrive()));
    downloadManager = new DownloadManager(networkAccessManager, this);
    connect(downloadManager, SIGNAL(downloadProgressChanged(QString,uchar)), skyModel, SLOT(onDownloadProgressChanged(QString,uchar)));
    connect(downloadManager, SIGNAL(stateChanged(QString,QString)), skyModel, SLOT(onStateChanged(QString,QString)));
}

void OneDrive::initOneDrive()
{
    refreshToken = readRefreshToken();
    if(refreshToken.isEmpty()) {
        qDebug() << "invalidRefreshToken";
        emit showLoginViewSignal();
    } else {
        reqRefreshToken();
    }
}

QNetworkReply* OneDrive::oauthRequest(const QByteArray &data)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setUrl(QUrl("https://login.live.com/oauth20_token.srf"));
    return networkAccessManager->post(request, data);
}

void OneDrive::parseInput(QString code)
{
    code.remove("https://login.live.com/oauth20_desktop.srf?code=");
    QByteArray data;
    data.append("client_id=" + clientId);
    data.append("&redirect_uri=" + redirectUrl);
    data.append("&client_secret=" + clientSecret);
    data.append("&code=" + code.toUtf8());
    data.append("&grant_type=authorization_code");

    QNetworkReply *reply = oauthRequest(data);
    connect(reply, SIGNAL(finished()), this, SLOT(onValidateInput()));
}


QString OneDrive::getLoginUrl()
{
    return ("https://login.live.com/oauth20_authorize.srf?client_id="+clientId+"&scope=wl.signin%20wl.skydrive_update%20wl.photos%20wl.basic%20wl.offline_access&display=touch&response_type=code&redirect_uri=" + redirectUrl);
}

//token_type":"bearer","expires_in":3600,"scope":"wl.signin wl.skydrive wl.basic wl.offline_access","access_token, user_id
void OneDrive::onValidateInput()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool ok;
    QJson::Parser parser;
    QVariantMap map = parser.parse(reply->readAll(), &ok).toMap();

    if(ok) {
        accessToken = map.value("access_token").toString().toUtf8();
        refreshToken = map.value("refresh_token").toString().toUtf8();
        int expires_in = map.value("expires_in").toInt();
        QTimer *t = new QTimer(this);
        t->setInterval((expires_in-100)*1000);
        t->setSingleShot(true);
        t->start();
        m_tokenTimerRequest = true;
        connect(t, SIGNAL(timeout()), this, SLOT(reqRefreshToken()));

        emit validateInputCompleted();

        if(!m_validateInitCompleted) {
            downloadManager->setAccessToken(accessToken);
            readFolderInfo("me/skydrive", "skydrive");
        }

    }
    reply->deleteLater();
    saveRefreshToken();
}


void OneDrive::reqRefreshToken()
{
    qDebug() << "reqRefreshToken";
    QByteArray data;
    data.append("client_id=" + clientId);
    data.append("&redirect_uri=" + redirectUrl);
    data.append("&client_secret=" + clientSecret);
    data.append("&refresh_token=" + refreshToken);
    data.append("&grant_type=refresh_token");

    QNetworkReply *reply = oauthRequest(data);

    connect(reply, SIGNAL(finished()), this, SLOT(onValidateInput()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onValidateInputError(QNetworkReply::NetworkError)));

    if(m_tokenTimerRequest)
        QObject::sender()->deleteLater();
}

void OneDrive::onValidateInputError(const QNetworkReply::NetworkError &err)
{
    qDebug() << err;
    if(err == QNetworkReply::AuthenticationRequiredError) { // 401 http error
        qDebug() << "err: " << err;
        emit refreshTokenErrorSignal();
    }
    emit refreshTokenErrorSignal();
    QObject::sender()->deleteLater();
}

void OneDrive::saveRefreshToken()
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "qonedrive",
                       "qonedrive");
    settings.setValue("rtk", encryptDecrypt(refreshToken));
    settings.sync();
}

QByteArray OneDrive::readRefreshToken()
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "qonedrive",
                       "qonedrive");
    return encryptDecrypt(settings.value("rtk", "").toByteArray());
}


void OneDrive::readFolderInfo(const QString &id, const QString &name)
{
    qDebug() << "folder: " << id;

    if(!folders.isEmpty())
       if(id == folders.at(folders.count()-1).first)
           return;

    QNetworkReply *reply = networkAccessManager->get(QNetworkRequest(QUrl("https://apis.live.net/v5.0/"+id+"/files"+"?access_token="+accessToken)));
    connect(reply, SIGNAL(finished()), this, SLOT(onReadFolderInfo()));

    folders.append(qMakePair(id, name));
}

// client_updated_time - last file modification
// updated_time - last upload
// created_time - first upload

void OneDrive::onReadFolderInfo()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    qDebug() << "onReadFolderInfo";

    QByteArray data = reply->readAll();

    bool ok;
    QJson::Parser parser;
    QVariantMap map = parser.parse(data, &ok).toMap();
    if(ok) {
        QVariantList varList = map.value("data").toList();

        skyModel->clear();
        skyModel->append(new AppItem("..", QString(), "folder", QString(), QString(), 0));

        for(int i = 0; i < varList.count(); ++i)
        {
            QVariantMap item = varList.at(i).toMap();
            QString created_time = item.value("created_time").toString();
            QString updated_time = item.value("updated_time").toString();
            QString client_updated_time = item.value("client_updated_time").toString();

            QDateTime dt = QDateTime::fromString(created_time, Qt::ISODate);
            dt.setTimeSpec(Qt::UTC);
            created_time = dt.toLocalTime().toString("HH:mm:ss dd/MM/yyyy");
            dt = QDateTime::fromString(updated_time, Qt::ISODate);
            dt.setTimeSpec(Qt::UTC);
            updated_time = dt.toLocalTime().toString("HH:mm:ss dd/MM/yyyy");
            dt = QDateTime::fromString(client_updated_time, Qt::ISODate);
            dt.setTimeSpec(Qt::UTC);
            client_updated_time = dt.toLocalTime().toString("HH:mm:ss dd/MM/yyyy");

            float fileSize = item.value("size").toInt();

            QPair<QString, uchar> pair = downloadManager->itemProperties(item.value("id").toString());
            skyModel->append(new AppItem(item.value("name").toString(),
                                         item.value("id").toString(),
                                         item.value("type").toString(),
                                         sizeToString(fileSize),
                                         pair.first,
                                         created_time,
                                         updated_time,
                                         client_updated_time,
                                         pair.second));
        }
    }

    reply->deleteLater();
}


void OneDrive::folderUp()
{
    if(folders.count() > 1)
    {
        folders.removeLast();
        qDebug() << "folderUp: " << folders;
        QNetworkReply *reply = networkAccessManager->get(QNetworkRequest(QUrl("https://apis.live.net/v5.0/"+folders.last().first+"/files"+"?access_token="+accessToken)));
        connect(reply, SIGNAL(finished()), this, SLOT(onReadFolderInfo()));
    }
}

void OneDrive::reqStorageQuota()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://apis.live.net/v5.0/me/skydrive/quota?access_token="+accessToken));
    QNetworkReply *reply = networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onStrageQuota()));
}

void OneDrive::onStrageQuota()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if(reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        bool ok;
        QJson::Parser parser;
        QVariantMap map = parser.parse(data, &ok).toMap();
        if(ok) {
            qint64 availableStorage = map.value("available").toLongLong();
            qint64 storageQuota = map.value("quota").toLongLong();

            QString totalStorage = sizeToString(storageQuota);
            QString unusedStorage = sizeToString(availableStorage);
            qDebug() << "total storage: " << totalStorage << "unused: " << unusedStorage;
        }
    }

    reply->deleteLater();
}

QString OneDrive::sizeToString(float size)
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

void OneDrive::downloadFile(const QString &name, const QString &id, const QString &type, const QString &size, const QString &created_time, const QString &updated_time, const QString &client_updated_time, const QString &path)
{
    downloadManager->downloadFile(name, id, type, size, created_time, updated_time, client_updated_time, path);
}

void OneDrive::uploadFile(const QString &fileName, const QString &uploadDir)
{
    downloadManager->uploadFile(fileName, uploadDir);
}

QString OneDrive::currentDirectoryId()
{
    return folders.last().first;
}

QString OneDrive::currentDirectoryName()
{
    return folders.last().second;
}

QAbstractListModel *OneDrive::model()
{
    return downloadManager->model();
}

QByteArray OneDrive::encryptDecrypt(const QByteArray &data)
{
    QByteArray result = data;
    int edKeyLength = edKey.length();
    for (int i = 0; i < data.count(); ++i) {
        result[i] =  data.at(i) ^ edKey.at(i%edKeyLength);
    }
    return result;
}
