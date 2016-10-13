#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("oerntec productions");
    QCoreApplication::setApplicationName("pallixlux-gui");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
