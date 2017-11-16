#include "spectrometerconfigpage.h"
#include "spectrometercontrolwidget.h"
#include "ui_spectrometerconfigpage.h"

#include <memory>

using namespace std;

SpectrometerConfigPage::SpectrometerConfigPage(QWidget *parent) : QWidget(parent), IScreenConfigPage(), ui(new Ui::SpectrometerConfigPage) {
    ui->setupUi(this);
    ui->pageMainLayout->addWidget(getControlWidget(parent));

    connect(mCurrentControlWidget, SIGNAL(gainChanged(double)), ui->gainSpin, SLOT(setValue(double)));
    connect(ui->lengthSpin, SIGNAL(valueChanged(int)), this, SLOT(parametersUpdated()));
    connect(ui->offsetSpin, SIGNAL(valueChanged(int)), this, SLOT(parametersUpdated()));
    connect(ui->gainSpin, SIGNAL(valueChanged(double)), this, SLOT(parametersUpdated()));

    // limit the offset to acceptable values
    ui->offsetSpin->setMaximum(mLedConfiguration.getLedCount() - 2);

    ui->lengthSpin->setValue(20);
    ui->offsetSpin->setValue(86);
    ui->gainSpin->setValue(100);
}

SpectrometerConfigPage::~SpectrometerConfigPage()
{
    delete ui;
}

void SpectrometerConfigPage::parametersUpdated() {
    SpectrometerControlWidget* controlWidget = dynamic_cast<SpectrometerControlWidget*>(mCurrentControlWidget);

    // read values from UI
    int offset = ui->offsetSpin->value();
    int ledPerChannel = ui->lengthSpin->value();
    const int leds = mLedConfiguration.getLedCount();

    // change the maximum led count appropriately to deny more leds than available
    ui->lengthSpin->setMaximum((leds - offset) / 2);

    if(offset + ledPerChannel * 2 > mLedConfiguration.getLedCount()){
        // fix the value of the led spinner back down if we exceeded maximum
        int ledPerChannel = (leds - offset) / 2;
        ui->lengthSpin->setValue(ledPerChannel);
    }

    controlWidget->setParameters(offset, ledPerChannel, ui->gainSpin->value());
}

ControlWidget *SpectrometerConfigPage::getControlWidget(QWidget *) {
    mCurrentControlWidget = new SpectrometerControlWidget(mLedConfiguration);
    return mCurrentControlWidget;
}
