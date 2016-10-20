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
    IScreenConfigPage(),
    ui(new Ui::SingleScreenConfigPage) {
    ui->setupUi(this);
    ui->tabMainLayout->addWidget(getControlWidget(parent));

    loadConfigFromSettings();

    connect(ui->resolutionWidthSpinbox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
    connect(ui->resolutionHeightSpinbox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
    connect(ui->xOffsetSpinbox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
    connect(ui->yOffsetSpinbox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
    connect(ui->letterboxingWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
    connect(ui->letterboxingHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateBorderProvider()));
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

void SingleScreenConfigPage::loadConfigFromSettings() {
    QSettings s;
    ui->resolutionWidthSpinbox->setValue(s.value("singlescreenconfigpage/xRes", 1920).toInt());
    ui->resolutionHeightSpinbox->setValue(s.value("singlescreenconfigpage/yRes", 1080).toInt());
    ui->xOffsetSpinbox->setValue(s.value("singlescreenconfigpage/xOff", 0).toInt());
    ui->yOffsetSpinbox->setValue(s.value("singlescreenconfigpage/yOff", 0).toInt());
    ui->letterboxingWidthSpinBox->setValue(s.value("singlescreenconfigpage/lbWidth", 0).toInt());
    ui->letterboxingHeightSpinBox->setValue(s.value("singlescreenconfigpage/lbHeight", 0).toInt());
}

void SingleScreenConfigPage::saveConfigToSettings()
{
    QSettings s;
    s.setValue("singlescreenconfigpage/xRes", ui->resolutionWidthSpinbox->value());
    s.setValue("singlescreenconfigpage/yRes", ui->resolutionHeightSpinbox->value());
    s.setValue("singlescreenconfigpage/xOff", ui->xOffsetSpinbox->value());
    s.setValue("singlescreenconfigpage/yOff", ui->yOffsetSpinbox->value());
    s.setValue("singlescreenconfigpage/lbWidth", ui->letterboxingWidthSpinBox->value());
    s.setValue("singlescreenconfigpage/lbHeight", ui->letterboxingHeightSpinBox->value());
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

void SingleScreenConfigPage::updateBorderProvider() {
    // persist new settings
    saveConfigToSettings();

    // get AmbiColorDataProvider
    std::shared_ptr<AmbiColorDataProvider> ambiProvider = std::dynamic_pointer_cast<AmbiColorDataProvider>(
                mCurrentControlWidget->getArduinoConnector()->getColorDataProvider());

    // save the old screenshot provider
    shared_ptr<ScreenshotProvider> sP = ambiProvider->getBorderProvider()->getScreenshotProvider();

    // controlwidget not yet initialized
    if(!sP || ! ambiProvider)
        return;

    // apply new borderprovider, and re-set the screenshot provider
    ambiProvider->setBorderProvider(getSingleScreenBorderProvider());
    ambiProvider->getBorderProvider()->setScreenshotProvider(sP);
}

ControlWidget* SingleScreenConfigPage::getControlWidget(QWidget* parent) {
    // get a builder
    AmbiConnectorBuilder builder;

    // instantiate and set the desired screenshot class
    builder.setScreenshotProvider(ScreenshotFactory::getPlatformAppropriateScreenshotProvider());

    // instantiate and set a single screen BorderProvider
    builder.setBorderProvider(getSingleScreenBorderProvider());

    // instantiate and set an AmbiColorDataProvider
    builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(mLedConfiguration)));

    // initialize control widget
    mCurrentControlWidget = new AmbiControlWidget(builder.build(), parent);

    return mCurrentControlWidget;
}
