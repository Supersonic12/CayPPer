#include "inputtakerfromqml.h"
#include <QDebug>
inputTakerFromQML::inputTakerFromQML(QObject *parent)
    : QObject{parent}
{}
void inputTakerFromQML::takeInput(const QString &text){

    qDebug()<< "TextFromQml:"<<text;
    p_dir=text;
    emit dirChanged(p_dir);
}
void inputTakerFromQML::chooseWallpaper(const QString &text){
    qDebug() << "Chosen Wallpaper" << text;
    p_fileIndex=text;
    emit wallpaperSelected(p_fileIndex);
}
QString dirChanged(QString input)
{
    return input;
}
QString wallpaperSelected(QString input){
    return input;
}
