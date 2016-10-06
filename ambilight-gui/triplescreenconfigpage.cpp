#include "ui_triplescreenconfigpage.h"
#include "triplescreenconfigpage.h"

#include <ambicolordataprovider.h>
#include <triplescreenborderprovider.h>

#include "ambicontrolwidget.h"
#include "screenshotfactory.h"

using namespace std;

TripleScreenConfigPage::TripleScreenConfigPage(QWidget *parent) :
    QWidget(parent),
    IScreenConfigPage(),
    ui(new Ui::TripleScreenConfigPage) {
    ui->setupUi(this);
    ui->tabMainLayout->addWidget(getControlWidget(parent));
}

TripleScreenConfigPage::~TripleScreenConfigPage() {
    delete ui;
}

ControlWidget *TripleScreenConfigPage::getControlWidget(QWidget *parent) {
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
    builder.setAmbiColorDataProvider(shared_ptr<AmbiColorDataProvider>(new AmbiColorDataProvider(mLedConfiguration)));

    // initialize control widget
    mCurrentControlWidget = new AmbiControlWidget(builder.build(), parent);

    return mCurrentControlWidget;
}
