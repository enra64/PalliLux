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

QString TripleScreenConfigPage::infoText() const {
    QString x1 = QString::number(ui->x1->value());
    QString y1 = QString::number(ui->y1->value());
    QString x2 = QString::number(ui->x2->value());
    QString y2 = QString::number(ui->y2->value());
    QString x3 = QString::number(ui->x3->value());
    QString y3 = QString::number(ui->y3->value());

    return QString("Triple screen, %1x%2 left of %3x%4 left of %5x%6").arg(x1, y1, x2, y2, x3, y3);
}

QString TripleScreenConfigPage::pageLabel() const {
    return QString("Triple Screen");
}

ControlWidget *TripleScreenConfigPage::getWidget(QWidget *parent, LedCount d) const
{
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
    AmbiControlWidget* w = new AmbiControlWidget(builder.build(), parent, infoText());

    return w;
}
