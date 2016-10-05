#include "ui_backlightconfigpage.h"
#include "backlightconfigpage.h"
#include "backlightcontrolwidget.h"

BacklightConfigPage::BacklightConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BacklightConfigPage) {
    ui->setupUi(this);
}

BacklightConfigPage::~BacklightConfigPage() {
    delete ui;
}

ControlWidget *BacklightConfigPage::getWidget(QWidget* parent, LedCount d) const {
    return new BacklightControlWidget(d, parent);
}


void BacklightConfigPage::updateLedCount(const LedCount &l) {
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
