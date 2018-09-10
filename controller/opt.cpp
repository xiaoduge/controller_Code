#include "opt.h"

#include "Display.h"
#include "user.h"

#include <QMutableLinkedListIterator>
#include <QPushButton>

COptDlg::COptDlg(QWidget *parent) :
    QDialog(parent)
{
    m_pBtnUserM  = new QPushButton;
    m_pBtnUserM->setText("UserM");

    m_pBtnCFG  = new QPushButton;
    m_pBtnCFG->setText("CFG");

    m_pBtnRO  = new QPushButton;
    m_pBtnRO->setText("RO");

    m_pBtnRH  = new QPushButton;
    m_pBtnRH->setText("RH");

    m_pBtnReset  = new QPushButton;
    m_pBtnReset->setText("Reset");

    m_pBtnRepS  = new QPushButton;
    m_pBtnRepS->setText("RepS");

    m_pBtnHInfQ  = new QPushButton;
    m_pBtnHInfQ->setText("HInfQ");

    m_pBtnCM  = new QPushButton;
    m_pBtnCM->setText("CM Mgr");

    m_pBtnEngineer  = new QPushButton;
    m_pBtnEngineer->setText("Engineer");

    m_pBtnExit  = new QPushButton;
    m_pBtnExit->setText("Exit");

    m_pMainLayout   = new QGridLayout(this);
    m_pTopLayout    = new QHBoxLayout();
    m_pMiddleLayout = new QHBoxLayout();
    m_pBottomLayout = new QHBoxLayout();
    m_pBottom2Layout = new QHBoxLayout();

    m_pTopLayout->addWidget(m_pBtnRO);
    m_pTopLayout->addWidget(m_pBtnRH);
    m_pTopLayout->addWidget(m_pBtnReset);
    
    m_pMiddleLayout->addWidget(m_pBtnHInfQ);
    m_pMiddleLayout->addWidget(m_pBtnUserM);
    m_pMiddleLayout->addWidget(m_pBtnRepS);

    m_pBottomLayout->addWidget(m_pBtnCFG);
    m_pBottomLayout->addWidget(m_pBtnCM);
    m_pBottomLayout->addWidget(m_pBtnEngineer);
    
    m_pBottom2Layout->addWidget(m_pBtnExit);

    if (gpActiveU)
    {
        if(!(gpActiveU->iPermission & 1 << DelP))
        {
            m_pBtnUserM->setEnabled(false);
        }
        if(!(gpActiveU->iPermission & 1 << PHP))
        {
            m_pBtnRH->setEnabled(false);
        }
        if(!(gpActiveU->iPermission & 1 << ROP))
        {
            m_pBtnRO->setEnabled(false);
        }
    }
    
    m_pMainLayout->addLayout(m_pTopLayout   ,0,0);
    m_pMainLayout->addLayout(m_pMiddleLayout,1,0);
    m_pMainLayout->addLayout(m_pBottomLayout,2,0);
    m_pMainLayout->addLayout(m_pBottom2Layout,3,0);
    
    setLayout(m_pMainLayout);

    m_pCfgDlg = new CfgDlg(this);
    m_pUserMgrDlg = new UserManageDlg(this);
    m_pDbDlg = new DbDlg(this);
    m_pCMDlg = new CMDlg(this);
    m_EngDlg = new EngineerDlg(this);

    connect(m_pBtnRO, SIGNAL(clicked()), this, SLOT(on_pushButton_RO()));
    connect(m_pBtnRH, SIGNAL(clicked()), this, SLOT(on_pushButton_RH()));
    connect(m_pBtnReset, SIGNAL(clicked()), this, SLOT(on_pushButton_Reset()));
    connect(m_pBtnCFG, SIGNAL(clicked()), this, SLOT(on_pushButton_CFG()));
    connect(m_pBtnUserM, SIGNAL(clicked()), this, SLOT(on_pushButton_UserM()));
    connect(m_pBtnRepS, SIGNAL(clicked()), this, SLOT(on_pushButton_RepS()));
    connect(m_pBtnHInfQ, SIGNAL(clicked()), this, SLOT(on_pushButton_HInfQ()));
    connect(m_pBtnCM, SIGNAL(clicked()), this, SLOT(on_pushButton_CM()));
    connect(m_pBtnEngineer, SIGNAL(clicked()), this, SLOT(on_pushButton_Engineer()));
    connect(m_pBtnExit, SIGNAL(clicked()), this, SLOT(on_pushButton_Exit()));
}

COptDlg::~COptDlg()
{
    delete m_pCfgDlg ;
    delete m_pUserMgrDlg ;
    delete m_pDbDlg ;
    delete m_EngDlg ;

    delete m_pBtnUserM;
    delete m_pBtnHInfQ;
    delete m_pBtnCFG;
    delete m_pBtnRO;
    delete m_pBtnRH;
    delete m_pBtnReset;
    delete m_pBtnRepS;
    delete m_pBtnEngineer;
    delete m_pBtnExit;
    delete m_pTopLayout;
    delete m_pMiddleLayout;
    delete m_pBottomLayout;
    delete m_pBottom2Layout;
    delete m_pMainLayout;
}

void COptDlg::on_pushButton_RO()
{

}

void COptDlg::on_pushButton_RH()
{

}

void COptDlg::on_pushButton_Reset()
{
    DispCmdEntry(DISP_CMD_RESET,NULL,0);
}

void COptDlg::on_pushButton_CFG()
{
    m_pCfgDlg->setGeometry(30,30,600,500);
    m_pCfgDlg->show();
}
void COptDlg::on_pushButton_RepS()
{

}
void COptDlg::on_pushButton_UserM()
{
    m_pUserMgrDlg->setGeometry(200,100,300,200);
    m_pUserMgrDlg->show();
}
void COptDlg::on_pushButton_HInfQ()
{
    m_pDbDlg->show();
}

void COptDlg::on_pushButton_CM()
{
   /* Later implement */
   m_pCMDlg->show();
}

void COptDlg::on_pushButton_Engineer()
{
    m_EngDlg->setGeometry(30,30,400,400);
    m_EngDlg->show();
}

void COptDlg::on_pushButton_Exit()
{
   close();
}

void COptDlg:: EngNotify(int iType,void *pItem)
{
    m_EngDlg->InfoUpdate(iType,pItem);

}

void COptDlg:: CfgNotify(int iType,void *pItem)
{
    m_pCfgDlg->InfoUpdate(iType,pItem);

}

void COptDlg:: EngRfMsg(IAP_NOTIFY_STRU *pNotify)
{
    m_EngDlg->RfNotify(pNotify);

}


