#ifndef PERMISSIONDLG_H
#define PERMISSIONDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>

class CMDlg ;

class PermissionDlg : public QDialog {
    Q_OBJECT
public:
    PermissionDlg(QWidget *parent = 0);
    ~PermissionDlg();
    int m_eventId;
    CMDlg *m_pParent;
public slots:
    bool eventFilter(QObject *, QEvent *);

    
private slots:
    void on_pushButton_Confirm();
    void on_pushButton_Cancel();

private:

    QPushButton *m_pBtnConfirm;
    QPushButton *m_pBtnCanCel;

    QLineEdit   *m_pEditUserName;

    QLabel      *m_pLUserNameL;

    QGridLayout *m_pMainLayout;

    QGridLayout *m_pTopLayout;
    QHBoxLayout *m_pBottomLayout;

};
#endif // LOGINDLG_H
