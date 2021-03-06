#include "ledconfigdialog.h"
#include "ui_ledconfigdialog.h"

#include <QSettings>
#include <QCheckBox>
#include <colordataprovider.h>

LedConfigDialog::LedConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LedConfigDialog) {
    ui->setupUi(this);

    // get qsettings instance
    QSettings s;

    // load stored led config values
    ui->bLedSpin->setValue(s.value("leds/bottom_count", LedConfig::INVALID).toInt());
    ui->rLedSpin->setValue(s.value("leds/right_count", LedConfig::INVALID).toInt());
    ui->tLedSpin->setValue(s.value("leds/top_count", LedConfig::INVALID).toInt());
    ui->lLedSpin->setValue(s.value("leds/left_count", LedConfig::INVALID).toInt());
    ui->ccwCheckBox->setChecked(s.value("leds/counter_clock_wise", true).toBool());

    // refresh led count
    refreshLedCount();

    // connect to change signals from the led spinboxes
    connect(ui->bLedSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LedConfigDialog::refreshLedCount);
    connect(ui->rLedSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LedConfigDialog::refreshLedCount);
    connect(ui->tLedSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LedConfigDialog::refreshLedCount);
    connect(ui->lLedSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LedConfigDialog::refreshLedCount);
}

LedConfigDialog::~LedConfigDialog() {
    delete ui;
}

LedConfig LedConfigDialog::getLedConfig() {
    QSettings s;
    return LedConfig(s.value("leds/bottom_count", LedConfig::INVALID).toInt(),
                    s.value("leds/right_count", LedConfig::INVALID).toInt(),
                    s.value("leds/top_count", LedConfig::INVALID).toInt(),
                    s.value("leds/left_count", LedConfig::INVALID).toInt(),
                    s.value("leds/counter_clock_wise", true).toBool());
}

bool LedConfigDialog::isLedCountSet() {
    return getLedConfig().valid();
}

void LedConfigDialog::refreshLedCount() {
    const int ledCount = ui->bLedSpin->value() + ui->rLedSpin->value() + ui->tLedSpin->value() + ui->lLedSpin->value();
    ui->ledCountLabel->setText(QString::number(ledCount));
}


void LedConfigDialog::accept() {
    // get QSettings instance
    QSettings s;

    // store current led config values
    s.setValue("leds/bottom_count", ui->bLedSpin->value());
    s.setValue("leds/right_count", ui->rLedSpin->value());
    s.setValue("leds/top_count", ui->tLedSpin->value());
    s.setValue("leds/left_count", ui->lLedSpin->value());
    s.setValue("leds/counter_clock_wise", ui->ccwCheckBox->isChecked());

    QDialog::accept();
}
