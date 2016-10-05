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


QString BacklightConfigPage::pageLabel() const {
    return "Backlight config";
}

QString BacklightConfigPage::infoText() const {
    return "Backlight mode";
}

ControlWidget *BacklightConfigPage::getWidget(QWidget* parent, LedCount d) const {
    return new BacklightControlWidget(d, parent, infoText());
}
