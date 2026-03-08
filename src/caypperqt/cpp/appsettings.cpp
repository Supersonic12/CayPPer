#include "appsettings.h"

appsettings::appsettings(QObject *parent)
    : QObject{parent}
{

}
QVariant appsettings::getBoolValue(const QString &key, const QVariant &defaultvalue){
    if(!settings_.contains(key)){
        return defaultvalue;
    }
    QVariant stored=settings_.value(key,defaultvalue);

    if(defaultvalue.typeId()==QMetaType::Bool){
        return stored.toString().toLower()=="true";
    }
    return stored;
}
QVariant appsettings::getValue(const QString &key, const QVariant &defaultvalue){
    return settings_.value(key,defaultvalue);
}
void appsettings::setValue(const QString& key, const QVariant &value){
    settings_.setValue(key,value);
}
void appsettings::sync(){
    settings_.sync();
}
