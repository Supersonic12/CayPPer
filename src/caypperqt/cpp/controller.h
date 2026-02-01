#ifndef CONTROLLER_H
#define CONTROLLER_H
#include<QStringList>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:

    explicit Controller(QObject *parent = nullptr);
    //looks what is inside given path
    QStringList whatInsideDirectory(QString path);
    //saves as return value of whatInsideDirectory
    //i save it to use in setWallPaper i think


    // i need to use listString i believe.
    //since i just call core libraries idk if i will need saving return value of listString

    //takes index and path of directory from inputtakefromqml::wallpaperSelected
    void setWallpaper(QString index);
signals:

private:
    QStringList listString;
};

#endif // CONTROLLER_H
