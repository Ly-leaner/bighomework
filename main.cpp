#include "mainwindow.h"
#include"startwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow w0;
    w0.show();


    //    MainWindow w;
    //    w.show();

    return a.exec();
}
