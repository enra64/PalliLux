#include "mainwindow.h"
#include <QApplication>

#include "screenshot.h"

#include <ImageMagick-6/Magick++.h>

#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Magick::InitializeMagick(*argv);

    ScreenShot screen(0,0,1024,1280);
    Magick::Image img;
    screen(img);
    img.write("test.png");
    exit(0);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
