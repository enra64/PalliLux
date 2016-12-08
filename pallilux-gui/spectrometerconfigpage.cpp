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

    ui->lengthSpin->setValue(40);
    ui->offsetSpin->setValue(0);
}

SpectrometerConfigPage::~SpectrometerConfigPage()
{
    delete ui;
}

void SpectrometerConfigPage::parametersUpdated() {
    SpectrometerControlWidget* controlWidget = dynamic_cast<SpectrometerControlWidget*>(mCurrentControlWidget);
    controlWidget->setParameters(ui->offsetSpin->value(), ui->lengthSpin->value());
}

ControlWidget *SpectrometerConfigPage::getControlWidget(QWidget *) {
    mCurrentControlWidget = new SpectrometerControlWidget(mLedConfiguration);
    return mCurrentControlWidget;
}
