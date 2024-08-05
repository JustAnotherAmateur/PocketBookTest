#pragma once

#include "imagestablemodel.h"

#include <QMainWindow>
#include <QDir>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetFolder(const QDir& folder);

private slots:
    void onFolderSelectButtonClicked();
    void onItemDoubleClicked(const QModelIndex& index);
    void onError(const QString& error_message);

private:
    std::unique_ptr<Ui::MainWindow> ui;

    ImagesTableModel* images_model = nullptr;
};
