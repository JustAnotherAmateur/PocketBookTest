#include "imagestablemodel.h"

#include "codecthread.h"

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

void ImagesTableModel::processItem(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    const auto file_info = m_image_files.at(index.row());
    m_target_filename = "";
    CodecThread::EMode mode;
    if (file_info.suffix() == "bmp")
    {
        m_target_filename = file_info.baseName() + "_packed.barch";
        mode = CodecThread::EMode::Encode;
        m_process_suffix = " Encoding...";
    }
    else if (file_info.suffix() == "barch")
    {
        m_target_filename = file_info.baseName() + "_unpacked.bmp";
        mode = CodecThread::EMode::Decode;
        m_process_suffix = " Decoding...";
    }

    if (!m_target_filename.isEmpty())
    {
        m_processed_filename = file_info.fileName();
        auto thread = new CodecThread(this, file_info.absoluteFilePath(), file_info.dir().absolutePath() + "/" + m_target_filename, mode);

        connect(thread, &QThread::started, this, &ImagesTableModel::threadStarted, Qt::QueuedConnection);
        connect(thread, &QThread::finished, this, &ImagesTableModel::threadFinished, Qt::QueuedConnection);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater, Qt::QueuedConnection);

        thread->start();
    }
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

QVariant ImagesTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const auto file_info = m_image_files.at(index.row());
    switch (index.column())
    {
    case EColumn::Name:
        {
        if (file_info.fileName() == m_processed_filename)
            return file_info.fileName() + m_process_suffix;
        else
            return file_info.fileName();
        }
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

void ImagesTableModel::threadStarted()
{
    const auto idx = index(file_index(m_processed_filename), EColumn::Name);
    if (idx.isValid())
        emit dataChanged(idx, idx);
}

void ImagesTableModel::threadFinished()
{
    const auto processed_model_idx = index(file_index(m_processed_filename), EColumn::Name);
    if (processed_model_idx.isValid())
        emit dataChanged(processed_model_idx, processed_model_idx);

    const auto target_file_index = file_index(m_target_filename);
    if (target_file_index >= 0)
    {
        // QFileSystemWatcher will detect new file as soon as it will be created, therefore empty
        // we need to update size correctly
        m_image_files[target_file_index].refresh();
        const auto target_model_idx = index(target_file_index, EColumn::Size);
        emit dataChanged(target_model_idx, target_model_idx);
    }

    m_processed_filename = "";
    m_target_filename = "";
    m_process_suffix = "";
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

int ImagesTableModel::file_index(const QString& filename) const
{
    for (int i = 0; i < m_image_files.size(); ++i)
    {
        if (m_image_files.at(i).fileName() == filename)
            return i;
    }

    return -1;
}
