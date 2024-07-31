#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    connect(ui->folderSelectButton, &QAbstractButton::clicked,
            this, &MainWindow::onFolderSelectButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetFolder(const QDir& folder)
{
    ui->folderPathLineEdit->setText(folder.absolutePath());
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
