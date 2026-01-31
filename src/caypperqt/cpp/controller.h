#ifndef CONTROLLER_H
#define CONTROLLER_H
#include<QStringList>
#include<filesystem>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    QStringList VectorToQList(const std::vector<std::filesystem::path> stdPath);
    QStringList listString;

    int setWallpaper(QString input);
signals:
};

#endif // CONTROLLER_H
