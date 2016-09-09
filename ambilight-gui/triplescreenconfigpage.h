#ifndef TRIPLESCREENCONFIGPAGE_H
#define TRIPLESCREENCONFIGPAGE_H

#include <QWidget>

#include "iconfigpage.h"

namespace Ui {
class TripleScreenConfigPage;
}

class TripleScreenConfigPage : public QWidget, public IScreenConfigPage
{
    Q_OBJECT

public:
    explicit TripleScreenConfigPage(QWidget *parent = 0);
    ~TripleScreenConfigPage();

private:
    Ui::TripleScreenConfigPage *ui;

    // IScreenConfigPage interface
public:
    QString infoText() const override;
    QString pageLabel() const override;
    void parametriseBuilder(AmbiConnectorBuilder &builder, int horizontalBorderLedCount, int verticalBorderLedCount) const override;
};



#endif // TRIPLESCREENCONFIGPAGE_H
