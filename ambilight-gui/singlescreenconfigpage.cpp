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
    builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(d)));

    // initialize control widget
    AmbiControlWidget* w = new AmbiControlWidget(builder.build(), parent);

    return w;
}


void SingleScreenConfigPage::updateLedCount(const LedCount &l) {
    // remove, disconnect & delete previous widget if exists
    if(mCurrentControlWidget != nullptr){
        ui->tabMainLayout->removeWidget(mCurrentControlWidget);
        disconnect(this, &QWidget::destroyed, mCurrentControlWidget, &ControlWidget::stop);
        delete mCurrentControlWidget;
    }

    // load control widget
    mCurrentControlWidget = getWidget(0, l);

    // window destroyed -> stop connection
    connect(this, &QWidget::destroyed, mCurrentControlWidget, &ControlWidget::stop);

    //display widget
    ui->tabMainLayout->addWidget(mCurrentControlWidget);
}
