#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , images_model(new ImagesTableModel(this))
{
    ui->setupUi(this);

    ui->imagesTableView->setModel(images_model);

    connect(ui->folderSelectButton, &QAbstractButton::clicked,
            this, &MainWindow::onFolderSelectButtonClicked);

    connect(ui->imagesTableView, &QAbstractItemView::doubleClicked,
            this, &MainWindow::onItemDoubleClicked);

    connect(images_model, &ImagesTableModel::error, this, &MainWindow::onError);
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetFolder(const QDir& folder)
{
    ui->folderPathLineEdit->setText(folder.absolutePath());
    images_model->setFolder(folder);
}

void MainWindow::onFolderSelectButtonClicked()
{
    QFileDialog folder_dialog(this);
    folder_dialog.setWindowTitle("Select Images Folder");
    folder_dialog.setFileMode(QFileDialog::Directory);
    folder_dialog.setOptions(QFileDialog::ShowDirsOnly);

    if (folder_dialog.exec())
    {
        QDir selected_folder = folder_dialog.directory();
        SetFolder(selected_folder);
    }
}

void MainWindow::onItemDoubleClicked(const QModelIndex &index)
{
    images_model->processItem(index);
}

void MainWindow::onError(const QString& error_message)
{
    QMessageBox::critical(ui->imagesTableView, "Error", error_message);
}
