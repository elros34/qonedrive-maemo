#include "networkmanagerfactory.h"
#include <QSslConfiguration>

NetworkManagerFactory::NetworkManagerFactory(QObject *parent) :
    QObject(parent)
{
#ifdef Q_WS_MAEMO_5
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1);
    QSslConfiguration::setDefaultConfiguration(config);
#endif
}

 /**
this is a virtual method we need to implement this method , most important step
we will create our custom QNetworkAccessManager here and return that
the second important thing we need to do here is to connect the sslErrors signal from QNetworkAccessManager to our own slot
which will ignore the sslErrors
*/
QNetworkAccessManager* NetworkManagerFactory::create(QObject *parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));

    return m_networkManager;
}

/**
Our own slot which is connected to the sslErrors signal of QNetworkAccessManager
When this slot is called using the QNetworkReply object in the parameter we need to call ignoreSslErrors method of QNetworkReply
*/
void NetworkManagerFactory::onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error)
{
    qDebug() << "ssl errors: " << error;
    qDebug() << "reply->errorString(): " << reply->errorString();
    reply->ignoreSslErrors(error);
}
