#include "triplescreenconfigpage.h"
#include "ui_triplescreenconfigpage.h"

#include <ambirgblineprovider.h>
#include <triplescreenborderprovider.h>

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

std::shared_ptr<RgbLineProvider> TripleScreenConfigPage::rgbProvider(int horizontalBorderLedCount, int verticalBorderLedCount) const {
    // first, we need a screenshot implementation
    shared_ptr<Screenshot> screenshotProvider = shared_ptr<Screenshot>(new XlibScreenshot());

    // second, we need a triplescreen borderprovider
    shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new TripleScreenBorderProvider(
                                                                               screenshotProvider,
                                                                               ui->x1->value(),
                                                                               ui->y1->value(),
                                                                               ui->x2->value(),
                                                                               ui->y2->value(),
                                                                               ui->x3->value(),
                                                                               ui->y3->value()));

    // third, we need an rgb line provider
    return shared_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, horizontalBorderLedCount, verticalBorderLedCount));
}
