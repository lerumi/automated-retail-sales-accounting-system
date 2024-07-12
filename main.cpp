#include "mainwindow.h"
#include"shipmentstab.h"
#include"authoriz.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    authoriz aa;
    aa.show();
    return a.exec();
}
