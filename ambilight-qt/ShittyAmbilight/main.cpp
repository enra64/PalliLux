#include "mainwindow.h"
#include <QApplication>

#include "ledvaluecreator.h"

#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
    Magick::InitializeMagick(*argv);

    LedValueCreator l;
    float sum = 0;
    float timeShot = 0;
    float timePars= 0;
    float timePxl = 0;

    for(int i = 0; i < 100; i++){
        timeShot += l.createScreenShots();
        uint8_t* data;
        timePars += l.parseScreenshot();
        timePxl += l.getData(&data);
    }

    sum = timeShot + timePars + timePxl;
    std::cout << "avg sot: " << (timeShot / 100) << std::endl;
    std::cout << "avg prs: " << (timePars / 100) << std::endl;
    std::cout << "avg pxl: " << (timePxl / 100) << std::endl;
    std::cout << "avg all: " << (sum / 100) << std::endl;

    //l.printCurrentImages();

    exit(0);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
