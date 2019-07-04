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

#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "LockupDlg.h"
#include "mainwindow.h"
#include "user.h"
#include "MyParams.h"

#include "keyboard.h"

LockupDlg::LockupDlg(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(300, 200);

    m_pBtnReset  = new QPushButton;
    m_pBtnReset->setText("Reset");

    m_pLUserNameL = new QLabel;
    m_pLUserNameL->setObjectName(QString::fromUtf8("Alarm"));
    m_pLUserNameL->setText("Leak Alarm");

    m_pMainLayout   =  new QGridLayout(this);
    m_pTopLayout    =  new QGridLayout; //QGridLayout
    m_pBottomLayout =  new QHBoxLayout;

    m_pTopLayout->addWidget(m_pLUserNameL,1,1);

    m_pBottomLayout->addWidget(m_pBtnReset);

    m_pMainLayout->addLayout(m_pTopLayout,0,0);
    m_pMainLayout->addLayout(m_pBottomLayout,1,0,1,2);

    setLayout(m_pMainLayout);

    connect(m_pBtnReset,SIGNAL(clicked()),this,SLOT(on_pushButton_Reset()));
}

LockupDlg::~LockupDlg()
{
    if (m_pBtnReset)     delete m_pBtnReset;
    if (m_pLUserNameL)   delete m_pLUserNameL;
    if (m_pTopLayout)    delete m_pTopLayout;
    if (m_pBottomLayout) delete m_pBottomLayout;
    
    if (m_pMainLayout) delete m_pMainLayout;

}

void LockupDlg::on_pushButton_Reset()
{
    DispCmdEntry(DISP_CMD_LEAK_RESET,NULL,0);
    close();

    gpMainWnd->setLokupState(false);
}

bool LockupDlg::eventFilter(QObject *watched, QEvent *event)
{

    return QDialog::eventFilter(watched,event);
}


