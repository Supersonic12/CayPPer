#ifndef INPUTTAKERFROMQML_H
#define INPUTTAKERFROMQML_H

#include <QObject>

class inputTakerFromQML : public QObject
{
    Q_OBJECT
public:
    explicit inputTakerFromQML(QObject *parent = nullptr);
    Q_INVOKABLE void takeInput(const QString &text);
    Q_INVOKABLE void chooseWallpaper(const QString &text);

signals:
    void dirChanged(const QString &text);
    void wallpaperSelected(const QString &text);
private:
    QString p_dir;
    QString p_fileIndex;
};

#endif // INPUTTAKERFROMQML_H
