#include "imagemodel.h"
#include <QAbstractListModel>
#include <QVector>
#include <QString>

ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}
int ImageModel::rowCount(const QModelIndex &) const
{
    return p_paths.size();
}
QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= p_paths.size())
        return {};

    if (role == PathRole)
        return p_paths.at(index.row());

    return {};
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    return {
        { PathRole, "imagePath" }
    };
}

void ImageModel::setImages(const QStringList &paths)
{
    beginResetModel();
    p_paths = paths;
    endResetModel();
}
