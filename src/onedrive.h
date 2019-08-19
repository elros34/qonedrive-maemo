#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QStringList>
#include "skymodel.h"
#include "appitem.h"
#include "downloadmanager.h"
#include "settings.h"


const QString clientId = "0000000000000000";
const QString clientSecret = "000000000000000000000000000000";
const QString redirectUrl = "https://login.live.com/oauth20_desktop.srf";
const QByteArray edKey = "0000000000000";

class OneDrive : public QObject
{
    Q_OBJECT
public:
    explicit OneDrive(QObject *parent = 0);

    SkyModel *skyModel;
    QList<QPair<QString, QString> > folders;

    QNetworkAccessManager *networkAccessManager;
    DownloadManager *downloadManager;


    Q_INVOKABLE void downloadFile(const QString &name, const QString &id, const QString &type, const QString &size, const QString &created_time, const QString &updated_time, const QString &client_updated_time, const QString &path);
    Q_INVOKABLE void uploadFile(const QString &fileName, const QString &uploadDir);
    Q_INVOKABLE QString currentDirectoryId();
    Q_INVOKABLE QString currentDirectoryName();
    Q_INVOKABLE QAbstractListModel* model();
private:

    QByteArray accessToken;
    QByteArray refreshToken;
    QString _activeFilePath;
    QString _activeFileId;
    bool m_tokenTimerRequest;
    bool m_validateInitCompleted;

signals:
    void validateInputCompleted();
    void refreshTokenErrorSignal();
    void invalidRefreshToken();
    void showLoginViewSignal();

public slots:
    void parseInput(QString code);
    QString getLoginUrl();
    void readFolderInfo(const QString &id, const QString &name);
    void folderUp();  
    void reqStorageQuota();
    QByteArray encryptDecrypt(const QByteArray &data);

private slots:
    void onValidateInput();
    void onReadFolderInfo();

    void reqRefreshToken();
    void onValidateInputError(const QNetworkReply::NetworkError &err);
    void saveRefreshToken();
    QByteArray readRefreshToken();
    void initOneDrive();
    QNetworkReply *oauthRequest(const QByteArray &data);

    QString sizeToString(float size);
    void onStrageQuota();
};

#endif // ONEDRIVE_H
