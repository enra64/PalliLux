#include "ui_triplescreenconfigpage.h"
#include "triplescreenconfigpage.h"

#include <ambicolordataprovider.h>
#include <triplescreenborderprovider.h>

#include "ambicontrolwidget.h"
#include "screenshotfactory.h"

using namespace std;

TripleScreenConfigPage::TripleScreenConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TripleScreenConfigPage) {
    ui->setupUi(this);
}

TripleScreenConfigPage::~TripleScreenConfigPage() {
    delete ui;
}

ControlWidget *TripleScreenConfigPage::getWidget(QWidget *parent, LedCount d) const {
    // get a builder
    AmbiConnectorBuilder builder;

    // instantiate and set the desired screenshot class
    builder.setScreenshotProvider(ScreenshotFactory::getPlatformAppropriateScreenshotProvider());

    // instantiate and set a single screen BorderProvider
    builder.setBorderProvider(shared_ptr<BorderProvider>(new TripleScreenBorderProvider(
                                  ui->x1->value(),
                                  ui->y1->value(),
                                  ui->x2->value(),
                                  ui->y2->value(),
                                  ui->x3->value(),
                                  ui->y3->value())));

    // instantiate and set an AmbiColorDataProvider
    builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(d)));

    // initialize control widget
    AmbiControlWidget* w = new AmbiControlWidget(builder.build(), parent);

    return w;
}


void TripleScreenConfigPage::updateLedCount(const LedCount &l) {
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
