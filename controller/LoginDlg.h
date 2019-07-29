#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

class DLineEdit;
class QPushButton;
class QLabel;
class QGridLayout;
class QHBoxLayout;

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

    DLineEdit *m_pEditUserName;
    DLineEdit *m_pEditPassWord;

    QLabel      *m_pLUserNameL;
    QLabel      *m_pLPassWordL;

    QGridLayout *m_pMainLayout;

    QGridLayout *m_pTopLayout;
    QHBoxLayout *m_pBottomLayout;

};
#endif // LOGINDLG_H
