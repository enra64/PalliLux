#include "spectrometerconfigpage.h"
#include "spectrometercontrolwidget.h"
#include "ui_spectrometerconfigpage.h"

#include <memory>

using namespace std;

SpectrometerConfigPage::SpectrometerConfigPage(QWidget *parent) : QWidget(parent), IScreenConfigPage(), ui(new Ui::SpectrometerConfigPage) {
    ui->setupUi(this);
    ui->pageMainLayout->addWidget(getControlWidget(parent));

    connect(ui->lengthSpin, SIGNAL(valueChanged(int)), this, SLOT(parametersUpdated()));
    connect(ui->offsetSpin, SIGNAL(valueChanged(int)), this, SLOT(parametersUpdated()));
    connect(ui->gainSpin, SIGNAL(valueChanged(double)), this, SLOT(parametersUpdated()));

    // limit the offset to acceptable values
    ui->offsetSpin->setMaximum(mLedConfiguration.getLedCount() - 2);

    ui->lengthSpin->setValue(18);
    ui->offsetSpin->setValue(90);
    ui->gainSpin->setValue(10);
}

SpectrometerConfigPage::~SpectrometerConfigPage()
{
    delete ui;
}

void SpectrometerConfigPage::parametersUpdated() {
    SpectrometerControlWidget* controlWidget = dynamic_cast<SpectrometerControlWidget*>(mCurrentControlWidget);
    int offset = ui->offsetSpin->value();
    int ledPerChannel = ui->lengthSpin->value();
    const int leds = mLedConfiguration.getLedCount();

    // change the maximum led count appropriately to deny more leds than available
    ui->lengthSpin->setMaximum((leds - offset) / 2);

    if(offset + ledPerChannel * 2 > mLedConfiguration.getLedCount()){
        // fix the value of the led spinner back down if we exceeded maximum
        ui->lengthSpin->setValue((leds - offset) / 2);
    }

    controlWidget->setParameters(ui->offsetSpin->value(), ui->lengthSpin->value(), ui->gainSpin->value());
}

ControlWidget *SpectrometerConfigPage::getControlWidget(QWidget *) {
    mCurrentControlWidget = new SpectrometerControlWidget(mLedConfiguration);
    return mCurrentControlWidget;
}
