#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>

const QDir get_working_folder(const QCoreApplication& app)
{
    QCommandLineParser parser;
    parser.addPositionalArgument("folder", "a folder with images");
    parser.process(app);

    QDir folder = QDir::current();
    const auto args = parser.positionalArguments();
    if (args.size() == 1)
    {
        QDir arg_folder(args.first());
        if (arg_folder.exists())
        {
            folder = arg_folder;
        }
    }

    return folder;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.SetFolder(get_working_folder(a));
    w.show();
    return a.exec();
}
