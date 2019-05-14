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

#include "PermissionDlg.h"
#include "mainwindow.h"
#include "user.h"
#include "MyParams.h"

#include "keyboard.h"


PermissionDlg::PermissionDlg(QWidget *parent) :
    QDialog(parent)
{
    //this->setFixedSize(300,200); //
    setFixedSize(300, 200);

    m_pBtnConfirm  = new QPushButton;
    m_pBtnConfirm->setText("Confirm");
    m_pBtnCanCel = new QPushButton;
    m_pBtnCanCel->setText("CanCel");

    m_pEditUserName = new QLineEdit;

    m_pLUserNameL = new QLabel;
    m_pLUserNameL->setObjectName(QString::fromUtf8("UserName"));
    m_pLUserNameL->setText("UserName:");

    m_pMainLayout   =  new QGridLayout(this);
    m_pTopLayout    =  new QGridLayout; //QGridLayout
    m_pBottomLayout =  new QHBoxLayout;

    m_pTopLayout->addWidget(m_pLUserNameL,1,1);
    m_pTopLayout->addWidget(m_pEditUserName,1,2);

    m_pBottomLayout->addWidget(m_pBtnConfirm);
    m_pBottomLayout->addWidget(m_pBtnCanCel);

    m_pMainLayout->addLayout(m_pTopLayout,0,0);
    m_pMainLayout->addLayout(m_pBottomLayout,1,0,1,2);

    setLayout(m_pMainLayout);

    m_pEditUserName->installEventFilter(this);

    connect(m_pBtnConfirm,SIGNAL(clicked()),this,SLOT(on_pushButton_Confirm()));
    connect(m_pBtnCanCel,SIGNAL(clicked()),this,SLOT(on_pushButton_Cancel()));

    setWindowModality(Qt::WindowModal); // only block its parent window

}

PermissionDlg::~PermissionDlg()
{
    if (m_pBtnConfirm)   delete m_pBtnConfirm;
    if (m_pBtnCanCel)    delete m_pBtnCanCel;
    if (m_pEditUserName) delete m_pEditUserName;
    if (m_pLUserNameL)   delete m_pLUserNameL;
    if (m_pTopLayout)    delete m_pTopLayout;
    if (m_pBottomLayout) delete m_pBottomLayout;
    
    if (m_pMainLayout) delete m_pMainLayout;

}



void PermissionDlg::on_pushButton_Confirm()
{
    QString strName = m_pEditUserName->text();

    if (!strName.isEmpty())
    {
        m_pParent->resetItem(m_eventId,strName);
        close();
    }

}

void PermissionDlg::on_pushButton_Cancel()
{
    close();
}

bool PermissionDlg::eventFilter(QObject *watched, QEvent *event)
{

    return QDialog::eventFilter(watched,event);
}


