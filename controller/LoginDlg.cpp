#include <QTextCodec>
#include <QMessageBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>

#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "LoginDlg.h"
#include "mainwindow.h"
#include "user.h"
#include "MyParams.h"

extern QString select_sql_User;
LoginDlg::LoginDlg(QWidget *parent,bool bPassword) :
    QDialog(parent)
{
    setWindowModality(Qt::WindowModal);

    //this->setFixedSize(300,200); //设置窗体固定大小
    setFixedSize(300, 200);

    m_pBtnLogin  = new QPushButton;
    m_pBtnLogin->setText(tr("Login"));
    m_pBtnCanCel = new QPushButton;
    m_pBtnCanCel->setText(tr("CanCel"));

    m_pEditUserName = new QLineEdit;
    m_pEditPassWord = new QLineEdit;

    m_pLUserNameL = new QLabel;
    m_pLUserNameL->setObjectName(QString::fromUtf8("User Name"));
    m_pLUserNameL->setText(tr("User Name:"));
    m_pLPassWordL = new QLabel;
    m_pEditPassWord->setObjectName(QString::fromUtf8("PassWord"));
    m_pLPassWordL->setText(tr("PassWord:"));

    m_pMainLayout   =  new QGridLayout(this);
    m_pTopLayout    =  new QGridLayout; //QGridLayout
    m_pBottomLayout =  new QHBoxLayout;

    m_pTopLayout->addWidget(m_pLUserNameL,1,1);
    m_pTopLayout->addWidget(m_pLPassWordL,2,1);
    m_pTopLayout->addWidget(m_pEditUserName,1,2);
    m_pTopLayout->addWidget(m_pEditPassWord,2,2);

    m_pBottomLayout->addWidget(m_pBtnLogin);
    m_pBottomLayout->addWidget(m_pBtnCanCel);

    m_pMainLayout->addLayout(m_pTopLayout,0,0);
    m_pMainLayout->addLayout(m_pBottomLayout,1,0,1,2);

    setLayout(m_pMainLayout);

    m_pEditUserName->installEventFilter(this);
    m_pEditPassWord->installEventFilter(this);
    
    connect(m_pBtnLogin,SIGNAL(clicked()),this,SLOT(on_pushButton_Login()));
    connect(m_pBtnCanCel,SIGNAL(clicked()),this,SLOT(on_pushButton_Cancel()));

    if (!bPassword)
    {
        m_pEditPassWord->hide();
        m_pLPassWordL->hide();
    }

}

LoginDlg::~LoginDlg()
{
    if (m_pBtnLogin)     delete m_pBtnLogin;
    if (m_pBtnCanCel)    delete m_pBtnCanCel;
    if (m_pEditUserName) delete m_pEditUserName;
    if (m_pEditPassWord) delete m_pEditPassWord;
    if (m_pLUserNameL)   delete m_pLUserNameL;
    if (m_pLPassWordL)   delete m_pLPassWordL;
    if (m_pTopLayout)    delete m_pTopLayout;
    if (m_pBottomLayout) delete m_pBottomLayout;
    
    if (m_pMainLayout)   delete m_pMainLayout;

}

void LoginDlg::on_pushButton_Login()
{
    if(!m_pEditUserName->text().isEmpty())
    {
        m_strUserName = m_pEditUserName->text();
        m_strPassword = m_pEditPassWord->text();
        close();
        m_iLogInResult = 0;
    }
}

void LoginDlg::on_pushButton_Cancel()
{
    close();

    m_iLogInResult = 1;
}

void LoginDlg::closeEvent(QCloseEvent *e)
{
    close();

    m_iLogInResult = 1;
}

bool LoginDlg::eventFilter(QObject *watched, QEvent *event)
{
    return QDialog::eventFilter(watched,event);
}


