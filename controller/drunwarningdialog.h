#ifndef DRUNWARNINGDIALOG_H
#define DRUNWARNINGDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class DDialogTitle;

class DRunWarningDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DRunWarningDialog(const QString& text, QWidget *parent = 0);

    void setButtonText(int num, const QString& text);

private:
    void initTitleBar();
    void initUI();

protected slots:
    void on_confirmBtn_clicked();
    void on_quitBtn_clicked();

private:
    DDialogTitle* m_titleBar;

    QString m_text;
    QLabel *m_pLabel;
    QPushButton *m_pConfirmBtn;
    QPushButton *m_pQuitBtn;
};

#endif // DRUNWARNINGDIALOG_H
