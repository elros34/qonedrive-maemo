#ifndef APPITEM_H
#define APPITEM_H

#include <QObject>

class AppItem : public QObject
{
    Q_OBJECT

public:
    explicit AppItem(QObject *parent = 0);
    explicit AppItem(const QString &name, const QString &id, const QString &type, const QString &size, const QString &currentState, const int &progress, QObject *parent=0);
    explicit AppItem(const QString &name,
                     const QString &id,
                     const QString &type,
                     const QString &size,
                     const QString &currentState,
                     const QString &created_time,
                     const QString &updated_time,
                     const QString &client_updated_time,
                     const int &progress,
                     QObject *parent=0);
    QString name() const;
    void setName(const QString &name);

    QString id() const;
    void setId(const QString &exec);

    QString type() const;
    void setType(const QString &type);

    QString size() const;
    void setSize(const QString &size);

    QString currentState() const;
    void setCurrentState(const QString &currentState);

    int progress() const;
    void setProgress(const int &progress);

    QString created_time() const;
    void setCreated_time(const QString &created_time);

    QString updated_time() const;
    void setUpdated_time(const QString &updated_time);

    QString client_updated_time() const;
    void setClient_updated_time(const QString &client_updated_time);

signals:
    void nameChanged();
    void idChanged();
    void sizeChanged();
    void typeChanged();
    void currentStateChanged();
    void progressChanged();

private:
    QString _name;
    QString _id;
    QString _type;
    QString _size;
    QString _currentState;
    int _progress;
    QString m_created_time;
    QString m_updated_time;
    QString m_client_updated_time;

public slots:

};

#endif // APPITEM_H
