#include "letterboxingautoconfigdialog.h"
#include "singlescreenconfigpage.h"
#include "ui_singlescreenconfigpage.h"

#include <QTimer>

#include <ambirgblineprovider.h>
#include <letterboxometer.h>
#include <singlescreenborderprovider.h>

using namespace std;

SingleScreenConfigPage::SingleScreenConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleScreenConfigPage) {
    ui->setupUi(this);
}

SingleScreenConfigPage::~SingleScreenConfigPage() {
    delete ui;
}

shared_ptr<BorderProvider> SingleScreenConfigPage::getSingleScreenBorderProvider(shared_ptr<Screenshot> screener) const {
    int w = ui->resolutionWidthSpinbox->value();
    int h = ui->resolutionHeightSpinbox->value();
    int xOff = ui->xOffsetSpinbox->value();
    int yOff = ui->yOffsetSpinbox->value();
    int xLetterbox = ui->letterboxingWidthSpinBox->value();
    int yLetterbox = ui->letterboxingHeightSpinBox->value();

    return shared_ptr<BorderProvider>(new SingleScreenBorderProvider(w, h, screener, xOff, yOff, xLetterbox, yLetterbox));
}

QString SingleScreenConfigPage::pageLabel() const {
    return QString("Single Screen");
}

QString SingleScreenConfigPage::infoText() const {
    QString w = QString::number(ui->resolutionWidthSpinbox->value());
    QString h = QString::number(ui->resolutionHeightSpinbox->value());
    QString xOff = QString::number(ui->xOffsetSpinbox->value());
    QString yOff = QString::number(ui->yOffsetSpinbox->value());
    QString xLetterbox = QString::number(ui->letterboxingWidthSpinBox->value());
    QString yLetterbox = QString::number(ui->letterboxingHeightSpinBox->value());

    return QString("Single screen, %1x%2+%3+%4, letterboxing %5x%6").arg(w, h, xOff, yOff, xLetterbox, yLetterbox);
}

std::shared_ptr<RgbLineProvider> SingleScreenConfigPage::rgbProvider(int horizontalBorderLedCount, int verticalBorderLedCount) const {
    // instantiate the desired screenshot class
    shared_ptr<Screenshot> screener = IScreenConfigPage::getPlatformAppropriateScreenshot();

    // instantiate the desired borderProvider with the screener. it will use the Screenshot instance
    // to get screenshots from the system
    shared_ptr<BorderProvider> borderProvider = getSingleScreenBorderProvider(screener);

    // instantiate and return an AmbiRgbLineProvider, the RGB data source. It will use the
    // BorderProvider to get images of the borders and convert them to RGB arrays
    return shared_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, horizontalBorderLedCount, verticalBorderLedCount));
}

void SingleScreenConfigPage::on_letterboxAutoConfigButton_clicked() {
    // get relevant sizes
    int w = ui->resolutionWidthSpinbox->value();
    int h = ui->resolutionHeightSpinbox->value();
    int xOff = ui->xOffsetSpinbox->value();
    int yOff = ui->yOffsetSpinbox->value();

    LetterboxingAutoConfigDialog dialog(w, h, xOff, yOff, this);
    dialog.exec();

    if(!dialog.wasCanceled()){
        ui->letterboxingHeightSpinBox->setValue(static_cast<int>(dialog.getLetterboxHeight()));
        ui->letterboxingWidthSpinBox->setValue(static_cast<int>(dialog.getLetterboxWidth()));
    }
}
