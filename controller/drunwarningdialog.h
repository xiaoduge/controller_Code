#ifndef DRUNWARNINGDIALOG_H
#define DRUNWARNINGDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class Ex_DialogTitle;

class DRunWarningDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DRunWarningDialog(QWidget *parent = 0);

private:
    void initTitleBar();
    void initUI();

protected slots:
    void on_confirmBtn_clicked();
    void on_quitBtn_clicked();

private:
    Ex_DialogTitle* m_titleBar;

    QLabel *m_pLabel;
    QPushButton *m_pConfirmBtn;
    QPushButton *m_pQuitBtn;
};

#endif // DRUNWARNINGDIALOG_H
