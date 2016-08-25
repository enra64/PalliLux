#ifndef TRIPLESCREENCONFIGPAGE_H
#define TRIPLESCREENCONFIGPAGE_H

#include "iconfigpage.h"

#include <QWidget>

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
    std::shared_ptr<RgbLineProvider> rgbProvider(int horizontalBorderLedCount, int verticalBorderLedCount) const override;
    QString infoText() const override;
    QString pageLabel() const override;
};



#endif // TRIPLESCREENCONFIGPAGE_H
