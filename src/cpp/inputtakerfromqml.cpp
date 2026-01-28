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
QString dirChanged(QString input)
{
    return input;
}
