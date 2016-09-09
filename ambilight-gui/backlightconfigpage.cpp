#include "ui_backlightconfigpage.h"
#include "backlightconfigpage.h"
#include "backlightdialog.h"

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

QDialog *BacklightConfigPage::getDialog(QWidget *parent, int horizontalBorderLedCount, int verticalBorderLedCount, std::string port) const {
    return new BacklightDialog(horizontalBorderLedCount, verticalBorderLedCount, port, parent);
}
