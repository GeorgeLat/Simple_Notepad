#include <QCoreApplication>
#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Text Editor");
    MainWindow mainWin;
    mainWin.show();

    return app.exec();
}

