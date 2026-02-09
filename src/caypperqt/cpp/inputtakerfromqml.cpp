#include "inputtakerfromqml.h"
#include <QPair>
//#include <QDebug>
inputTakerFromQML::inputTakerFromQML(QObject *parent)
    : QObject{parent}
{}
void inputTakerFromQML::takeInput(const QString &text){
    //qDebug()<< "Grid showing directory: "<<text;
    p_dir=text;
    emit dirChanged(p_dir);
}
void inputTakerFromQML::chooseWallpaper(const QString &text){
    p_fileIndex=text;
    //qDebug() << "Chosen Wallpaper Index: " << text;
    //qDebug() << "Chosen Wallpaper Dir: " << p_dir;
    emit wallpaperSelected(p_fileIndex,p_modeFill);
}

void inputTakerFromQML::selectedMode(const QString &mode){
    p_modeFill=mode;
    emit modeSelected(mode);
}

