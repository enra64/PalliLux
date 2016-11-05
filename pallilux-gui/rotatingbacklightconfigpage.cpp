#include "rotatingbacklightconfigpage.h"
#include "rotatingbacklightcontrolwidget.h"
#include "ui_rotatingbacklightconfigpage.h"

RotatingBacklightConfigPage::RotatingBacklightConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotatingBacklightConfigPage)
{
    ui->setupUi(this);
    ui->tabMainLayout->addWidget(getControlWidget(parent));
}

RotatingBacklightConfigPage::~RotatingBacklightConfigPage()
{
    delete ui;
}

ControlWidget *RotatingBacklightConfigPage::getControlWidget(QWidget *parent)
{
    mCurrentControlWidget = new RotatingBacklightControlWidget(mLedConfiguration, parent);
    return mCurrentControlWidget;
}
