#ifndef LETTERBOXINGAUTOCONFIGDIALOG_H
#define LETTERBOXINGAUTOCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class LetterboxingAutoConfigDialog;
}

class LetterboxingAutoConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LetterboxingAutoConfigDialog(size_t w, size_t h, size_t xOff, size_t yOff, QWidget *parent = 0);
    ~LetterboxingAutoConfigDialog();
    size_t getLetterboxWidth();
    size_t getLetterboxHeight();
    bool wasCanceled(){
        return mCanceled;
    }

private slots:
    void on_countdownStartButton_clicked();

    void on_letterboxConfigDialogButtons_accepted();

    void on_letterboxConfigDialogButtons_rejected();

private:
    Ui::LetterboxingAutoConfigDialog *ui;
    const size_t SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_XOFF, SCREEN_YOFF;
    size_t mResultWidth = -1, mResultHeight = -1;
    bool mCanceled = false;
    bool mCountdownRunning = false;
};

#endif // LETTERBOXINGAUTOCONFIGDIALOG_H
