#pragma once

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

private:
    std::unique_ptr<Ui::MainWindow> ui;
};
