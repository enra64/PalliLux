#include "spectrometerconfigpage.h"
#include "spectrometercontrolwidget.h"
#include "ui_spectrometerconfigpage.h"

SpectrometerConfigPage::SpectrometerConfigPage(QWidget *parent) : QWidget(parent), IScreenConfigPage(), ui(new Ui::SpectrometerConfigPage) {
    ui->setupUi(this);
    ui->pageMainLayout->addWidget(getControlWidget(parent));
}

SpectrometerConfigPage::~SpectrometerConfigPage()
{
    delete ui;
}

ControlWidget *SpectrometerConfigPage::getControlWidget(QWidget *) {
    mCurrentControlWidget = new SpectrometerControlWidget(mLedConfiguration);
    return mCurrentControlWidget;
}
