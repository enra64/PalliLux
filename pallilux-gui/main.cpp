#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("oerntec productions");
    QCoreApplication::setApplicationName("pallixlux-gui");

    QApplication a(argc, argv);


    QPixmap pixmap(":/images/splash.jpg");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);

    MainWindow w;

    QTimer::singleShot(2000, &splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));

    splash.show();

    return a.exec();
}
