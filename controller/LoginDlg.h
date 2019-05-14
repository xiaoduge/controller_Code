#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>

class LoginDlg : public QDialog {
    Q_OBJECT
public:
    LoginDlg(QWidget *parent = 0,bool bPassword=true);
    ~LoginDlg();
    
public:    
    QString m_strUserName;
    QString m_strPassword;
    int     m_iLogInResult;

public slots:
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *, QEvent *);
private slots:
    void on_pushButton_Login();
    void on_pushButton_Cancel();

private:

    QPushButton *m_pBtnLogin;
    QPushButton *m_pBtnCanCel;

    QLineEdit   *m_pEditUserName;
    QLineEdit   *m_pEditPassWord;

    QLabel      *m_pLUserNameL;
    QLabel      *m_pLPassWordL;

    QGridLayout *m_pMainLayout;

    QGridLayout *m_pTopLayout;
    QHBoxLayout *m_pBottomLayout;
};
#endif // LOGINDLG_H
