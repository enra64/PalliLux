#include "ui_backlightconfigpage.h"
#include "backlightconfigpage.h"
#include "backlightcontrolwidget.h"

BacklightConfigPage::BacklightConfigPage(QWidget *parent) :
    QWidget(parent),
    IScreenConfigPage(),
    ui(new Ui::BacklightConfigPage) {

    ui->setupUi(this);
    ui->tabMainLayout->addWidget(getControlWidget(parent));
}

BacklightConfigPage::~BacklightConfigPage() {
    delete ui;
}

ControlWidget *BacklightConfigPage::getControlWidget(QWidget* parent) {
    mCurrentControlWidget = new BacklightControlWidget(mLedConfiguration, parent);
    return mCurrentControlWidget;
}
