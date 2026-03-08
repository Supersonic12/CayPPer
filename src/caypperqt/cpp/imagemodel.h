#pragma once
#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <qabstractitemmodel.h>

class ImageModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
        PathRole = Qt::UserRole+1
    };
    explicit ImageModel(QObject *parent=nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int ,QByteArray> roleNames() const override;

    void setImages(const QStringList &paths);
private:
    QStringList p_paths;
};

#endif // IMAGEMODEL_H
