#pragma once
#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
class appsettings : public QObject
{
    Q_OBJECT
public:
    explicit appsettings(QObject *parent = nullptr);
    QVariant getValue (const QString &key,const QVariant &defaultvalue =QVariant());
    QVariant getBoolValue(const QString &key, const QVariant &defaultvalue=QVariant());
    void setValue (const QString &key,const QVariant &value);
    void sync();
signals:

private:
    QSettings settings_;
};

#endif // APPSETTINGS_H
