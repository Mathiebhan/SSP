#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Dolle User App");
    //w.setFixedSize(500,300);
    w.show();

    return a.exec();
}



