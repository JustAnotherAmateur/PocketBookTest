#pragma once

#include <QAbstractTableModel>
#include <QDir>
#include <QFileSystemWatcher>

#include <memory>

class ImagesTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ImagesTableModel(QObject *parent = nullptr);

    void setFolder(const QDir& folder);
    void processItem(const QModelIndex& index);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

signals:
    void error(const QString& error_message);

private slots:
    void updateFileList(const QString& fodler_path);
    void threadStarted();
    void threadFinished();
    void onError(const QString& error_message);

private:
    void sort_internal();
    int file_index(const QString& filename) const;

private:
    enum EColumn
    {
        Name = 0,
        Type = 1,
        Size = 2,
        Count = 3
    };

    QFileInfoList m_image_files;
    std::unique_ptr<QFileSystemWatcher> m_folder_watcher;

    EColumn m_sort_column;
    Qt::SortOrder m_sort_order;

    QString m_processed_filename;
    QString m_target_filename;
    QString m_process_suffix;
};
