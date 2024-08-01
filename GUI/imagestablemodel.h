#pragma once

#include <QAbstractTableModel>
#include <QDir>

class ImagesTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ImagesTableModel(QObject *parent = nullptr);

    void setFolder(const QDir& folder);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QFileInfoList m_image_files;

    enum EColumn
    {
        Name = 0,
        Type = 1,
        Size = 2,
        Count = 3
    };
};
