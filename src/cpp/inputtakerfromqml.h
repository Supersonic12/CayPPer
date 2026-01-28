#ifndef INPUTTAKERFROMQML_H
#define INPUTTAKERFROMQML_H

#include <QObject>

class inputTakerFromQML : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString dirChanged READ dirChanged NOTIFY dirChanged())
public:
    explicit inputTakerFromQML(QObject *parent = nullptr);
    Q_INVOKABLE void takeInput(const QString &text);

signals:
    QString dirChanged(const QString &text);
private:
    QString p_dir;
};

#endif // INPUTTAKERFROMQML_H
