#include <QTimer>

#include "letterboxingautoconfigdialog.h"
#include "ui_letterboxingautoconfigdialog.h"

#include <memory>

#include <screenshotprovider.h>
#include <letterboxometer.h>

#include "iconfigpage.h"

#include "screenshotfactory.h"

#ifdef __linux__
    #include <xlibscreenshotprovider.h>
#elif _WIN32_WINNT
    #include <winscreenshotprovider.h>
#else
    #error Platform not recognized
#endif



using namespace std;

LetterboxingAutoConfigDialog::LetterboxingAutoConfigDialog(size_t w, size_t h, size_t xOff, size_t yOff, QWidget *parent) : QDialog(parent),
    ui(new Ui::LetterboxingAutoConfigDialog), SCREEN_WIDTH(w), SCREEN_HEIGHT(h), SCREEN_XOFF(xOff), SCREEN_YOFF(yOff) {
    ui->setupUi(this);
}

LetterboxingAutoConfigDialog::~LetterboxingAutoConfigDialog() {
    delete ui;
}

size_t LetterboxingAutoConfigDialog::getLetterboxWidth() {
    return mResultWidth;
}

size_t LetterboxingAutoConfigDialog::getLetterboxHeight() {
    return mResultHeight;
}

void LetterboxingAutoConfigDialog::on_countdownStartButton_clicked() {
    if(mCountdownRunning){
        mCountdownRunning = false;
        return;
    }

    // set running state
    mCountdownRunning = true;

    // instantiate screener and meter
    shared_ptr<ScreenshotProvider> screener = ScreenshotFactory::getPlatformAppropriateScreenshotProvider();
    LetterboxOMeter meter(screener, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_XOFF, SCREEN_YOFF, ui->thresholdSpinbox->value());

    // wait for user to activate the video player, counting down
    for(int i = ui->delaySpinbox->value(); i > 0; i--) {
        ui->countdownStartButton->setText(QString("%1s left").arg(i));
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
        if(!mCountdownRunning){
            ui->countdownStartButton->setText("Start countdown");
            mCountdownRunning = false;
            return;
        }
    }

    // ui update
    ui->countdownStartButton->setText("calculating now");

    // measure
    pair<int, int> letterboxDimensions = meter.measure();

    // save auto values
    mResultHeight = letterboxDimensions.first;
    mResultWidth = letterboxDimensions.second;

    // show auto values
    ui->horizontalHeightLabel->setText(QString::number(mResultHeight));
    ui->verticalWidthLabel->setText(QString::number(mResultWidth));

    // set running state
    mCountdownRunning = false;

    // reset ui
    ui->countdownStartButton->setText("Start countdown");
}

void LetterboxingAutoConfigDialog::on_letterboxConfigDialogButtons_accepted() {
    close();
}

void LetterboxingAutoConfigDialog::on_letterboxConfigDialogButtons_rejected() {
    mResultHeight = -1;
    mResultWidth = -1;
    mCanceled = true;
    close();
}
