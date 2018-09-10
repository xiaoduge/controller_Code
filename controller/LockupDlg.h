#ifndef LOCKUPDLG_H
#define LOCKUPDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>

class LockupDlg : public QDialog {
    Q_OBJECT
public:
    LockupDlg(QWidget *parent = 0);
    ~LockupDlg();
public slots:
    bool eventFilter(QObject *, QEvent *);
private slots:
    void on_pushButton_Reset();

private:

    QPushButton *m_pBtnReset;

    QLabel      *m_pLUserNameL;

    QGridLayout *m_pMainLayout;

    QGridLayout *m_pTopLayout;
    QHBoxLayout *m_pBottomLayout;

};
#endif // LOGINDLG_H
