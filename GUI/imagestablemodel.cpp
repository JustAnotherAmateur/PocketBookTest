#include "imagestablemodel.h"

ImagesTableModel::ImagesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_sort_column(EColumn::Name)
    , m_sort_order(Qt::AscendingOrder)
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

void ImagesTableModel::sort(int column, Qt::SortOrder order)
{
    m_sort_column = static_cast<EColumn>(column);
    m_sort_order = order;
    beginResetModel();
    sort_internal();
    endResetModel();
}

void ImagesTableModel::updateFileList(const QString& folder_path)
{
    QDir folder(folder_path);
    QStringList name_filters;
    name_filters << "*.bmp" << "*.png" << "*.barch";
    beginResetModel();
    m_image_files = folder.entryInfoList(name_filters, QDir::Files);
    sort_internal();
    endResetModel();
}

void ImagesTableModel::sort_internal()
{
    const auto comparator = [this](const QFileInfo& lhs, const QFileInfo& rhs)
    {
        switch (m_sort_column)
        {
        case EColumn::Type:
            return (m_sort_order == Qt::AscendingOrder) ? (lhs.suffix() < rhs.suffix()) :
                       (lhs.suffix() > rhs.suffix());
        case EColumn::Size:
            return (m_sort_order == Qt::AscendingOrder) ? (lhs.size() < rhs.size()) :
                       (lhs.size() > rhs.size());
        case EColumn::Name:
        default:
            return (m_sort_order == Qt::AscendingOrder) ? (lhs.fileName() < rhs.fileName()) :
                       (lhs.fileName() > rhs.fileName());
        }
    };

    std::sort(m_image_files.begin(), m_image_files.end(), comparator);
}
