#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filename = "";
    //write(filename);
    //read(filename);
    MainWindow w;
    w.show();

    return a.exec();
}
