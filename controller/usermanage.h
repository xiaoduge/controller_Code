#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QDialog>

#include <QTextCodec>
#include <QMessageBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QLineEdit>
#include <QButtonGroup>
#include <QCheckBox>

#include "keyboard.h"
#include <QCloseEvent>  


class UserManageDlg : public QDialog
{
    Q_OBJECT
public:
    explicit UserManageDlg(QWidget *parent = 0);
    ~UserManageDlg();

    int Permission ;
protected:  
    void closeEvent(QCloseEvent *event);  


private:

    QPushButton *m_pBtnAddU;
    QPushButton *m_pBtnDelU;
    QPushButton *m_pBtnCancelU;

    QLineEdit *m_pEditUserName;
    QLineEdit *m_pEditPassWord;

    QLabel *m_pLUserNameL;
    QLabel *m_pLPassWordL;

    QButtonGroup *m_pButtonGroup;
    QCheckBox    *m_pCheckBox;

    QLabel      *m_pQlpermission;

    QCheckBox   *m_pROPBOX;
    QCheckBox   *m_pPHPBOX;
    QCheckBox   *m_pDelPBOX;
    QCheckBox   *m_pAdminPBOX;

    QGridLayout *m_pMainLayout;

    QGridLayout *m_pTopLayout;
    QGridLayout *m_pPLayout;
    QHBoxLayout *m_pBottomLayout;
    
public slots:
    void ROPBOX_Checked(int state);
    void PHPBOX_Checked(int state);
    void DelPBOX_Checked(int state);
    void AdminPBOX_Checked(int state);

    void on_pushButton_AddU();
    void on_pushButton_DelU();
    void on_pushButton_CancelU();

    bool eventFilter(QObject *, QEvent *);

    //void lineEditGetTem(QString str);
    void write_back_str(QString str);
    
};

#endif // USERMANAGE_H
