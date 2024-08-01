#include "imagestablemodel.h"

ImagesTableModel::ImagesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void ImagesTableModel::setFolder(const QDir &folder)
{
    m_folder_watcher.reset(new QFileSystemWatcher());
    m_folder_watcher->addPath(folder.absolutePath());

    connect(m_folder_watcher.get(), &QFileSystemWatcher::directoryChanged,
            this, &ImagesTableModel::updateFileList);

    updateFileList(folder.absolutePath());
}

int ImagesTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_image_files.size();
}

int ImagesTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return EColumn::Count;
}


QVariant ImagesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Orientation::Horizontal) && (role == Qt::DisplayRole))
    {
        switch (section)
        {
        case EColumn::Name:
            return "Name";
        case EColumn::Type:
            return "Type";
        case EColumn::Size:
            return "Size, MiB";
        }
    }

    return QVariant();
}

QVariant ImagesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    auto file_info = m_image_files.at(index.row());
    switch (index.column())
    {
    case EColumn::Name:
        return file_info.fileName();
    case EColumn::Type:
        return file_info.suffix();
    case EColumn::Size:
        return QString::number(double(file_info.size()) / (1024 * 1024), 'f', 2);
    }

    return QVariant();
}

void ImagesTableModel::updateFileList(const QString& folder_path)
{
    QDir folder(folder_path);
    QStringList name_filters;
    name_filters << "*.bmp" << "*.png" << "*.barch";
    beginResetModel();
    m_image_files = folder.entryInfoList(name_filters, QDir::Files);
    endResetModel();
}
