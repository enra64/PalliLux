#include <QTimer>

#include "letterboxingautoconfigdialog.h"
#include "ui_letterboxingautoconfigdialog.h"

#include <memory>

#include <screenshot.h>
#include <letterboxometer.h>

#include "iconfigpage.h"

#ifdef __linux__
    #include <xlibscreenshot.h>
#elif _WIN32_WINNT
    #include <winscreenshot.h>
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
    shared_ptr<Screenshot> screener = IScreenConfigPage::getPlatformAppropriateScreenshot();
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
    meter.measure();

    // ui update
    ui->countdownStartButton->setText("Start countdown");

    // save auto values
    mResultWidth = meter.getLetterboxWidth();
    mResultHeight = meter.getLetterboxHeight();

    // show auto values
    ui->verticalWidthLabel->setText(QString::number(mResultWidth));
    ui->horizontalHeightLabel->setText(QString::number(mResultHeight));

    // set running state
    mCountdownRunning = false;
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
