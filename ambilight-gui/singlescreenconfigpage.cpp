#include <QTimer>

#include "singlescreenconfigpage.h"
#include "ui_singlescreenconfigpage.h"

#include "letterboxingautoconfigdialog.h"

#include "ambicontrolwidget.h"
#include "screenshotfactory.h"

#include <ambicolordataprovider.h>
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

shared_ptr<BorderProvider> SingleScreenConfigPage::getSingleScreenBorderProvider() const {
    int w = ui->resolutionWidthSpinbox->value();
    int h = ui->resolutionHeightSpinbox->value();
    int xOff = ui->xOffsetSpinbox->value();
    int yOff = ui->yOffsetSpinbox->value();
    int xLetterbox = ui->letterboxingWidthSpinBox->value();
    int yLetterbox = ui->letterboxingHeightSpinBox->value();

    return shared_ptr<BorderProvider>(new SingleScreenBorderProvider(w, h, xOff, yOff, xLetterbox, yLetterbox));
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

void SingleScreenConfigPage::on_letterboxAutoConfigButton_clicked() {
    // get relevant sizes
    int w = ui->resolutionWidthSpinbox->value();
    int h = ui->resolutionHeightSpinbox->value();
    int xOff = ui->xOffsetSpinbox->value();
    int yOff = ui->yOffsetSpinbox->value();

    LetterboxingAutoConfigDialog dialog(w, h, xOff, yOff, this);
    dialog.exec();

    if(!dialog.wasCanceled()) {
        ui->letterboxingHeightSpinBox->setValue(static_cast<int>(dialog.getLetterboxHeight()));
        ui->letterboxingWidthSpinBox->setValue(static_cast<int>(dialog.getLetterboxWidth()));
    }
}

ControlWidget* SingleScreenConfigPage::getWidget(QWidget* parent, LedCount d) const {
    // get a builder
    AmbiConnectorBuilder builder;

    // instantiate and set the desired screenshot class
    builder.setScreenshotProvider(ScreenshotFactory::getPlatformAppropriateScreenshotProvider());

    // instantiate and set a single screen BorderProvider
    builder.setBorderProvider(getSingleScreenBorderProvider());

    // instantiate and set an AmbiColorDataProvider
    builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(d.bottom, d.right, d.top, d.left)));

    // initialize control widget
    AmbiControlWidget* w = new AmbiControlWidget(builder.build(), parent, infoText());

    return w;
}
