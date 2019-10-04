#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

class Settings: public QSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0): QSettings(QSettings::IniFormat,
                                                      QSettings::UserScope,
                                                      "qonedrive",
                                                      "qonedrive",
                                                      parent) {}


    Q_INVOKABLE inline void setValue(const QString &key, const QVariant &value) { QSettings::setValue(key, value); }
    Q_INVOKABLE inline QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const { return QSettings::value(key, defaultValue); }
    Q_INVOKABLE inline void beginGroup(const QString &prefix) {QSettings::beginGroup(prefix);}
    Q_INVOKABLE inline void endGroup() {QSettings::endGroup();}
    Q_INVOKABLE inline void remove(const QString &key) {QSettings::remove(key);}

};

#endif // SETTINGS_H
