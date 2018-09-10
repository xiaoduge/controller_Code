#include "EngDialog.h"
#include <QInputDialog>
#include <QComboBox>
#include <QRect>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QTextCodec>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include "notify.h"

#include "keyboard.h"
#include "MyParams.h"
#include "switchbtn.h"
#include "mainwindow.h"

#include "rpc.h"
#include "sapp.h"
#include "Interface.h"

class exe_switchs
{
public :
    QString strName;
    int      id;
};

class exe_outputs
{
public :
    QString strName;
    int      id;
};


static exe_switchs aSwitchs[] = {
    {"E1",APP_EXE_E1_NO},
    {"E2",APP_EXE_E2_NO},
    {"E3",APP_EXE_E3_NO},
    {"E4",APP_EXE_E4_NO},
    {"E5",APP_EXE_E5_NO},
    {"E6",APP_EXE_E6_NO},
    {"E9",APP_EXE_E9_NO},
    {"E10",APP_EXE_E10_NO},
    {"C3",APP_EXE_C3_NO},
    {"C4",APP_EXE_C4_NO},
    {"C1",APP_EXE_C1_NO},
    {"C2",APP_EXE_C2_NO},
    {"N1",APP_EXE_N1_NO},
    {"N2",APP_EXE_N2_NO},
    {"N3",APP_EXE_N3_NO},
    {"T1",APP_EXE_T1_NO},
};

#define  SWITCH_NUM  (sizeof(aSwitchs)/sizeof(aSwitchs[0]))

static exe_switchs aIAndBs[] = {
    {"I1",APP_EXE_I1_NO},
    {"I2",APP_EXE_I2_NO},
    {"I3",APP_EXE_I3_NO},
    {"I4",APP_EXE_I4_NO},
    {"I5",APP_EXE_I5_NO},
    {"B1",APP_EXE_PM1_NO},
    {"B2",APP_EXE_PM2_NO},
    {"B3",APP_EXE_PM3_NO},

};

#define  IB_NUM  (sizeof(aIAndBs)/sizeof(aIAndBs[0]))

static exe_switchs aFMs[] = {
    {"FM1",APP_FM_FM1_NO},
    {"FM2",APP_FM_FM2_NO},
    {"FM3",APP_FM_FM3_NO},
    {"FM4",APP_FM_FM4_NO},

};

#define  FM_NUM  (sizeof(aFMs)/sizeof(aFMs[0]))

static exe_outputs aRPumps[] = {
    {"RP1",APP_EXE_HOLD_REG_RPUMP_OFFSET + 0},
    {"RP2",APP_EXE_HOLD_REG_RPUMP_OFFSET + 1},

};

#define  RP_NUM  (sizeof(aRPumps)/sizeof(aRPumps[0]))

static QString strRfNames[] = 
{
    "Header Size",
    "Property Size",
    "Device",
    "Type",
    "Trade Mark",
    "Name",
    "Catalogue Num",
    "Unknow",
    "LOT Num",
    "Unknow",
    "Install Date",
    "Other Info",
};

static QString strRfBtns[] = 
{
    "Search",
    "Read",
    "Write",
};


#define MAKE_UINT32(pData) ((pData[0] << 24)|(pData[1] << 16)|(pData[2] << 8)|(pData[3] << 0))
#define MAKE_UINT16(pData) ((pData[0] << 8)|(pData[1] << 0))


#define PARSE_UINT32(value,pData) \
    do {\
        pData[0] = (value >> 24) & 0xff;\
        pData[1] = (value >> 16) & 0xff;\
        pData[2] = (value >> 8) & 0xff;\
        pData[3] = (value >> 0) & 0xff;\
    }while(0);
#define PARSE_UINT16(value,pData) \
    do {\
        pData[0] = (value >> 8) & 0xff;\
        pData[1] = (value >> 0) & 0xff;\
    }while(0);


/*QLineEdit LEdit[] = {
{N3Duration}
};*/

EngineerDlg::EngineerDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("ENG"));
    int iLoop = 0 ;
    QString strTemp;

    QHBoxLayout *layout;
    QGridLayout *pGridLayout;

    int iCtrlIdOffset = 0;

    m_ulRfIdIdx = 0;

    Qt::WindowFlags flags=Qt::Dialog;

    //flags |=Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    layout = new QHBoxLayout();

    m_pTblWidget = new QTabWidget();  

    
    /* for page 1*/
    m_pWidgets[0]  = new QWidget();  
    m_pMainLayout  = new QGridLayout(this);
    m_ppSwitchBtns = new SwitchButton * [SWITCH_NUM];
    m_ppSwitchLabel      = new QLabel *[SWITCH_NUM];

    /* add child widget here */
    for(iLoop = 0;iLoop < (int)SWITCH_NUM; iLoop++)
    {
        strTemp = aSwitchs[iLoop].strName;
        strTemp +=":";
        m_ppSwitchLabel[iLoop] = new QLabel;
        m_ppSwitchLabel[iLoop]->setText(strTemp);
        m_ppSwitchBtns[iLoop] = new SwitchButton(this,iLoop + iCtrlIdOffset);
        m_ppSwitchBtns[iLoop]->setFixedSize(50,30);

        if (iLoop < (int)SWITCH_NUM / 2 )
        {
            m_pMainLayout->addWidget(m_ppSwitchLabel[iLoop],iLoop,1);
            m_pMainLayout->addWidget(m_ppSwitchBtns[iLoop],iLoop,2);
        }
        else
        {
            m_pMainLayout->addWidget(m_ppSwitchLabel[iLoop],iLoop - SWITCH_NUM / 2,3);
            m_pMainLayout->addWidget(m_ppSwitchBtns[iLoop],iLoop - SWITCH_NUM / 2,4);
        }

        connect(m_ppSwitchBtns[iLoop],SIGNAL(checkedChanged(bool,int)),this,SLOT(onSwitchBtnChanged(bool,int)));

    }

    iCtrlIdOffset += SWITCH_NUM;    
    m_pWidgets[0]->setLayout(m_pMainLayout); 

    QIcon icon1(":/image/radiobutton_on.png");  
    
    m_pTblWidget->addTab(m_pWidgets[0], icon1, "SWITCH");      


    /* for page 2 */
    m_pWidgets[1] = new QWidget();  
    pGridLayout  = new QGridLayout(this);
    m_ppIBBtns   = new SwitchButton *[IB_NUM];
    m_ppIBLabels = new QLabel *[IB_NUM];
    m_ppEditIBs  = new QLineEdit *[IB_NUM];  
    m_ppEditIBs  = new QLineEdit *[IB_NUM]; 
    m_ppEditTemps= new QLineEdit *[APP_EXE_ECO_NUM]; 

    /* add child widget here */
    for(iLoop = 0;iLoop < (int)IB_NUM; iLoop++)
    {
        strTemp = aIAndBs[iLoop].strName;
        strTemp +=":";
        m_ppIBLabels[iLoop] = new QLabel;
        m_ppIBLabels[iLoop]->setText(strTemp);
        m_ppIBBtns[iLoop] = new SwitchButton(this,iLoop + iCtrlIdOffset);
        m_ppIBBtns[iLoop]->setFixedSize(50,30);

        m_ppEditIBs[iLoop]  = new QLineEdit;
        m_ppEditIBs[iLoop]->setFixedSize(48,18);
        m_ppEditIBs[iLoop]->setReadOnly(true); /* read only */

        if (iLoop < APP_EXE_ECO_NUM)
        {
            m_ppEditTemps[iLoop]  = new QLineEdit;
            m_ppEditTemps[iLoop]->setFixedSize(48,18);
            m_ppEditTemps[iLoop]->setReadOnly(true); /* read only */
        
        }
        if (iLoop < (int)IB_NUM / 2 )
        {
            pGridLayout->addWidget(m_ppIBLabels[iLoop],iLoop,1);
            pGridLayout->addWidget(m_ppIBBtns[iLoop],iLoop  ,2);
            pGridLayout->addWidget(m_ppEditIBs[iLoop],iLoop ,3);
            if (iLoop < APP_EXE_ECO_NUM)
            {
                pGridLayout->addWidget(m_ppEditTemps[iLoop],iLoop,4);
            }
        }
        else
        {
            pGridLayout->addWidget(m_ppIBLabels[iLoop],iLoop - IB_NUM / 2,5);
            pGridLayout->addWidget(m_ppIBBtns[iLoop]  ,iLoop - IB_NUM / 2,6);
            pGridLayout->addWidget(m_ppEditIBs[iLoop] ,iLoop - IB_NUM / 2,7);
            if (iLoop < APP_EXE_ECO_NUM)
            {
                pGridLayout->addWidget(m_ppEditTemps[iLoop],iLoop - IB_NUM / 2,8);
            }
        }

        connect(m_ppIBBtns[iLoop],SIGNAL(checkedChanged(bool,int)),this,SLOT(onSwitchBtnChanged(bool,int)));

    }
    
    iCtrlIdOffset += IB_NUM;    
    m_pWidgets[1]->setLayout(pGridLayout); 

    QIcon icon2(":/image/radiobutton_on.png");  
    
    m_pTblWidget->addTab(m_pWidgets[1], icon2, "I and B");      


    /* for page 3 */
    m_pWidgets[2]       = new QWidget();  
    pGridLayout  = new QGridLayout(this);
    m_ppFMBtns   = new SwitchButton * [FM_NUM];
    m_ppFMLabels = new QLabel *[FM_NUM];
    m_ppEditFMs  = new QLineEdit *[FM_NUM];  

    /* add child widget here */
    for(iLoop = 0;iLoop < (int)FM_NUM; iLoop++)
    {
        strTemp = aIAndBs[iLoop].strName;
        strTemp +=":";
        m_ppFMLabels[iLoop] = new QLabel;
        m_ppFMLabels[iLoop]->setText(strTemp);
        m_ppFMBtns[iLoop] = new SwitchButton(this,iLoop + iCtrlIdOffset);
        m_ppFMBtns[iLoop]->setFixedSize(50,30);

        m_ppEditFMs[iLoop]  = new QLineEdit;
        m_ppEditFMs[iLoop]->setFixedSize(48,18);
        m_ppEditFMs[iLoop]->setReadOnly(true); /* read only */

        pGridLayout->addWidget(m_ppFMLabels[iLoop],iLoop,1);
        pGridLayout->addWidget(m_ppFMBtns[iLoop],iLoop,2);
        pGridLayout->addWidget(m_ppEditFMs[iLoop],iLoop,3);

        connect(m_ppFMBtns[iLoop],SIGNAL(checkedChanged(bool,int)),this,SLOT(onSwitchBtnChanged(bool,int)));

    }
    
    iCtrlIdOffset += FM_NUM;    
    m_pWidgets[2]->setLayout(pGridLayout); 

    QIcon icon3(":/image/radiobutton_on.png");  
    
    m_pTblWidget->addTab(m_pWidgets[2], icon3, "FM");     


    /* for page 4 */
    m_pWidgets[3]       = new QWidget();  
    pGridLayout  = new QGridLayout(this);
    m_ppRPLabels = new QLabel *[RP_NUM];
    m_ppRPSliders  = new CSliderEx *[RP_NUM];  

    /* add child widget here */
    for(iLoop = 0;iLoop < (int)RP_NUM; iLoop++)
    {
        strTemp = aRPumps[iLoop].strName;
        strTemp +=":";
        m_ppRPLabels[iLoop] = new QLabel;
        m_ppRPLabels[iLoop]->setText(strTemp);
        
        m_ppRPSliders[iLoop] = new CSliderEx(Qt::Horizontal,this,iLoop);
        m_ppRPSliders[iLoop]->setFixedSize(90,30);
        m_ppRPSliders[iLoop]->setMinimum(0);
        m_ppRPSliders[iLoop]->setMaximum(24);
        m_ppRPSliders[iLoop]->setValue(24);

        pGridLayout->addWidget(m_ppRPLabels[iLoop],iLoop,1);
        pGridLayout->addWidget(m_ppRPSliders[iLoop],iLoop,2);

        connect(m_ppRPSliders[iLoop],SIGNAL(valueChanged(int,int)),this,SLOT(onSliderChanged(int,int)));

    }
    
    iCtrlIdOffset += FM_NUM;    
    m_pWidgets[3]->setLayout(pGridLayout);

    QIcon icon4(":/image/radiobutton_on.png");
    
    m_pTblWidget->addTab(m_pWidgets[3], icon4, "PUMP");
  
    gpMainWnd->initRfIdLayout(&m_RfDataItems); 

    /* for page 5 RF id */
    m_pWidgets[4]    = new QWidget();  
    pGridLayout      = new QGridLayout(this);
    m_ppRfLabels     = new QLabel       *[RF_DATA_LAYOUT_NUM];
    m_ppEditRfs      = new QLineEdit    *[RF_DATA_LAYOUT_NUM];  
    m_ppCombBoxRfs   = new QComboBox    *[1];  
    m_ppBtnRfs       = new QPushButton  *[3];  
    m_btnGroupRf     = new QButtonGroup(this);

    m_ppCombBoxRfs[0] = new QComboBox();
    m_ppCombBoxRfs[0]->setFixedSize(48,18);

    pGridLayout->addWidget(m_ppCombBoxRfs[0],1,1);

    
    m_ppCombBoxRfs[0]->addItem("RFID0");
    m_ppCombBoxRfs[0]->addItem("RFID1");
    
    connect(m_ppCombBoxRfs[0], SIGNAL(currentIndexChanged(int)),
     this, SLOT(RfId_CmbIndexChange(int)));


    /* add child widget here */
    unsigned int ulRdMask = (1 << RF_DATA_LAYOUT_NUM) - 1;
    ulRdMask &= ~(1 << RF_DATA_LAYOUT_UNKNOW_DATA);
    
    for(iLoop = 0;iLoop < RF_DATA_LAYOUT_NUM; iLoop++)
    {
        strTemp = strRfNames[iLoop];
        strTemp +=":";
        m_ppRfLabels[iLoop] = new QLabel;
        m_ppRfLabels[iLoop]->setText(strTemp);
        

        m_ppEditRfs[iLoop]  = new QLineEdit;
        m_ppEditRfs[iLoop]->setFixedSize(72,18);
        if (ulRdMask & (1 << iLoop))
        {
            m_ppEditRfs[iLoop]->setReadOnly(true); /* read only */
        }

        pGridLayout->addWidget(m_ppRfLabels[iLoop], iLoop + 2,1);
        pGridLayout->addWidget(m_ppEditRfs[iLoop],iLoop + 2,2);
    }

    for (iLoop = 0; iLoop < 3; iLoop++)
    {
        m_ppBtnRfs[iLoop] = new QPushButton();
        m_ppBtnRfs[iLoop]->setObjectName(strRfBtns[iLoop]);
        m_ppBtnRfs[iLoop]->setText(strRfBtns[iLoop]);
        m_btnGroupRf->addButton(m_ppBtnRfs[iLoop],iLoop);
        
    }

    pGridLayout->addWidget(m_ppBtnRfs[0],RF_DATA_LAYOUT_NUM + 2,1);
    pGridLayout->addWidget(m_ppBtnRfs[1],RF_DATA_LAYOUT_NUM + 2,2);
    pGridLayout->addWidget(m_ppBtnRfs[2],RF_DATA_LAYOUT_NUM + 2,3);

    connect(m_btnGroupRf,SIGNAL(buttonClicked(int)),SLOT(rfBtnsClicked(int)));

    m_pWidgets[4]->setLayout(pGridLayout);

    QIcon icon5(":/image/radiobutton_on.png");
    
    m_pTblWidget->addTab(m_pWidgets[4], icon5, "RFID");


    layout->addWidget(m_pTblWidget);    

    /* common */
    QGridLayout *tmpGrid = new QGridLayout;

    m_pBtnExit = new QPushButton();
    m_pBtnExit->setObjectName(QString::fromUtf8("Exit"));
    m_pBtnExit->setText("Exit");

    m_pLabelStatus = new QLabel();
    m_pLabelStatus->setObjectName(QString::fromUtf8("status"));
    m_pLabelStatus->setText("IDLE");

    tmpGrid->addWidget(m_pBtnExit,0,0);
    tmpGrid->addWidget(m_pLabelStatus,1,0);

    layout->addLayout(tmpGrid);  

    this->setLayout(layout);  

    connect(m_pBtnExit,SIGNAL(clicked()),this,SLOT(Exit()));

    installEventFilter(this);

}

EngineerDlg::~EngineerDlg()
{
    delete m_pBtnExit;
    delete m_pMainLayout;
}


bool EngineerDlg::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this)
    {
        if(QEvent::WindowActivate == event->type())  
        {  
            unsigned char buf[1];
            buf[1] = 1;
        
            DispCmdEntry(DISP_CMD_ENG_MODE,buf,1);
            return true ;  
        }  

        if (QEvent::WindowDeactivate == event->type())
        {  
            unsigned char buf[1];
            buf[1] = 0;
        
            DispCmdEntry(DISP_CMD_ENG_MODE,buf,1);
            return true ;  
        }  
    }

    return QDialog::eventFilter(watched,event);
}

void EngineerDlg::closeEvent(QCloseEvent *event)
{
    (void)event;
}

void EngineerDlg::rfBtnsClicked(int id)
{
    switch(id)
    {
    case 0:   /* scan */
        {
            char buf[64];
            IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
        
            APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

            pCmd->iCanChl     = 0;
            pCmd->ucCmd       = SAPP_CMD_DATA;
            pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
            pCmd->ulCanId     = m_ulRfIdIdx;
        
            pHo->hdr.ucLen     = 1 + 0;
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
            pHo->ucOpsType     = APP_PACKET_RF_SEARCH;

            pCmd->iPayLoadLen += pHo->hdr.ucLen;  
            
            DispAfRfEntry(pCmd);
        }
        break;  
    case 1:   /* read */
        {
            char buf[64];
            IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
        
            APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

            pCmd->iCanChl     = 0;
            pCmd->ucCmd       = SAPP_CMD_DATA;
            pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
            pCmd->ulCanId     = m_ulRfIdIdx;
        
            pHo->hdr.ucLen     = 1 + sizeof(APP_PACKET_RF_READ_REQ_STRU);
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
        
            pHo->ucOpsType     = APP_PACKET_RF_READ;

            {
                APP_PACKET_RF_READ_REQ_STRU *pRd = (APP_PACKET_RF_READ_REQ_STRU *)pHo->aucData;

                pRd->ucLen = 192;
                pRd->ucOff = 0;
                
            }   
            pCmd->iPayLoadLen += pHo->hdr.ucLen;  
            
            DispAfRfEntry(pCmd);
        }
        break;
    case 2:   /* write */
        {
            char buf[64];
            IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
        
            APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

            pCmd->iCanChl     = 0;
            pCmd->ucCmd       = SAPP_CMD_DATA;
            pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
            pCmd->ulCanId     = m_ulRfIdIdx;
        
            pHo->hdr.ucLen     = 1 + 2;
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
        
            pHo->ucOpsType     = APP_PACKET_RF_WRITE;

            {
                APP_PACKET_RF_WRITE_REQ_STRU *pWr = (APP_PACKET_RF_WRITE_REQ_STRU *)pHo->aucData;

                pWr->ucLen = 4;
                pWr->ucOff = m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset;

                m_ulUnknowData = m_ppEditRfs[RF_DATA_LAYOUT_UNKNOW_DATA]->text().toInt();
                
                pHo->hdr.ucLen += pWr->ucLen;
                
                PARSE_UINT32(m_ulUnknowData,pWr->aucData);

                qDebug() << "off: " << pWr->ucOff << "value: " << m_ulUnknowData;
                
            }   
            pCmd->iPayLoadLen += pHo->hdr.ucLen;  
            
            DispAfRfEntry(pCmd);            
        }
        break;
    }
}

void EngineerDlg::onSliderChanged(int id, int value)
{
    unsigned char buf[128];
    int iIdx = 0;

    DISPHANDLE hdl;

    if (value >= 8)
    {
        int tmp = DispConvertVoltage2RPumpSpeed(value) ;
        
        qDebug()<<"fRwb:" <<tmp ;

        buf[iIdx++] = ENG_CMD_TYPE_RPUMP;
        buf[iIdx++] = ((id + APP_EXE_HOLD_REG_RPUMP_OFFSET) >> 8) & 0XFF;
        buf[iIdx++] = ((id + APP_EXE_HOLD_REG_RPUMP_OFFSET) >> 0) & 0XFF;
        buf[iIdx++] = ((tmp) >> 8) & 0XFF;
        buf[iIdx++] = ((tmp) >> 0) & 0XFF;
        hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);

        if (hdl)
        {
            qDebug()<<"pump:" <<id <<"State:"<< value;
        }
    }
}

void EngineerDlg::onSwitchBtnChanged(bool bState, int iCtrlId)
{
    unsigned char buf[128];
    int iIdx = 0;

    DISPHANDLE hdl;
    
    qDebug()<<"iCtrlId:" <<iCtrlId <<"State:"<<bState;

    if (iCtrlId < (int)SWITCH_NUM)
    {
        buf[iIdx++] = ENG_CMD_TYPE_SWITCH;
        buf[iIdx++] = aSwitchs[iCtrlId].id;
        buf[iIdx++] = bState ? 1 : 0;
        hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
        
        if (!hdl)
        {
            m_ppSwitchBtns[iCtrlId]->setState(!bState);
        }
        
    }
    else if (iCtrlId < (int)(SWITCH_NUM + IB_NUM))
    {
        buf[iIdx++] = ENG_CMD_TYPE_IB;
        buf[iIdx++] = aIAndBs[iCtrlId - SWITCH_NUM].id;
        buf[iIdx++] = bState ? 1 : 0;
        hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
        
        if (!hdl)
        {
            m_ppIBBtns[iCtrlId - SWITCH_NUM]->setState(!bState);
        }
    }
    else if (iCtrlId < (int)(SWITCH_NUM + IB_NUM + FM_NUM))
    {
        buf[iIdx++] = ENG_CMD_TYPE_FM;
        buf[iIdx++] = aFMs[iCtrlId - SWITCH_NUM - IB_NUM].id;
        buf[iIdx++] = bState ? 1 : 0;
        hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
        
        if (!hdl)
        {
            m_ppFMBtns[iCtrlId - SWITCH_NUM - IB_NUM]->setState(!bState);
        }
    }

}

void EngineerDlg::Exit()
{
    close();
}

void EngineerDlg::InfoUpdate(int iType,void *pData)
{
    if (!isVisible())
    {
        return ;
    }

    switch(iType)
    {
    case DISP_NOT_ECO:
        {
            if (m_pWidgets[1] == m_pTblWidget->currentWidget()) 
            {
                 NOT_ECO_ITEM_STRU *pItem = (NOT_ECO_ITEM_STRU *)pData;
                 
                 if (pItem->ucId < APP_EXE_ECO_NUM)
                 {
                     m_ppEditIBs[pItem->ucId]->setText(QString::number(pItem->fValue,'f', 2));
                     m_ppEditTemps[pItem->ucId]->setText(QString::number(pItem->usValue));
                 }
            }
        }
        break;
    case DISP_NOT_PM:
        {
            
            if (m_pWidgets[1] == m_pTblWidget->currentWidget())
            {
                NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pData;
                if (pItem->ucId < APP_EXE_PRESSURE_METER)
                {
                    switch (pItem->ucId)
                    {
                    case APP_EXE_PM1_NO:
                        m_ppEditIBs[pItem->ucId + APP_EXE_ECO_NUM]->setText(QString::number(CcbConvert2Pm1Data(pItem->ulValue),'f',2));
                        break;
                    case APP_EXE_PM2_NO:
                        m_ppEditIBs[pItem->ucId + APP_EXE_ECO_NUM]->setText(QString::number(CcbConvert2Pm2Data(pItem->ulValue),'f',2));
                        break;
                    case APP_EXE_PM3_NO:
                        m_ppEditIBs[pItem->ucId + APP_EXE_ECO_NUM]->setText(QString::number(CcbConvert2Pm3Data(pItem->ulValue),'f',2));
                        break;
                    }
                    
                }
            }
        }
        break;
    case DISP_NOT_FM:
        {
            
            if (m_pWidgets[2] == m_pTblWidget->currentWidget())
            {
                NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pData;
                if (pItem->ucId < APP_FM_FLOW_METER_NUM)
                {
                    m_ppEditFMs[pItem->ucId ]->setText(QString::number(pItem->ulValue));
        
                }
            }
        }
        break;

    }
}


void EngineerDlg::RfNotify(IAP_NOTIFY_STRU *pIapNotify)
{
    if (!isVisible())
    {
        return ;
    }

    if (m_pWidgets[4] == m_pTblWidget->currentWidget()) 
    {
        /* do thing here */
        APP_PACKET_COMM_STRU *pmg = (APP_PACKET_COMM_STRU *)&pIapNotify->data[1 + RPC_POS_DAT0]; 
        switch((pmg->ucMsgType & 0x7f))
        {

        case APP_PACKET_RF_OPERATION:
            if ((pmg->ucMsgType & 0x80))
            {
                APP_PACKET_RF_STRU *pmg = (APP_PACKET_RF_STRU *)&pIapNotify->data[1 + RPC_POS_DAT0]; 
            
                switch(pmg->ucOpsType)
                {
                case APP_PACKET_RF_SEARCH:
                    {
                        APP_PACKET_RF_SEARCH_RSP_STRU *pSearchRsp = (APP_PACKET_RF_SEARCH_RSP_STRU *)pmg->aucData;

                        if (0 < pSearchRsp->ucLen)
                        {
                            m_pLabelStatus->setText(tr("RF SR SUCC"));
                        }
                        else
                        {
                            m_pLabelStatus->setText(tr("RF SR FAIL"));
                        }
                        
                    }
                    break;
                case APP_PACKET_RF_READ:
                    {
                        APP_PACKET_RF_READ_RSP_STRU *pRdRsp = (APP_PACKET_RF_READ_RSP_STRU *)pmg->aucData;

                        if (APP_PACKET_RF_ERROR_CODE_SUCC == pRdRsp->ucRslt)
                        {
                            APP_PACKET_RPT_RF_READ_CONT_STRU *pCont = (APP_PACKET_RPT_RF_READ_CONT_STRU *)pRdRsp->aucData;

                            memcpy(&m_rfLayout.data[pCont->ucOff],pCont->aucData,pCont->ucLen);

                            ParseData(m_rfLayout);

                            m_pLabelStatus->setText(tr("RF RD SUCC"));
                        }
                        else
                        {
                            m_pLabelStatus->setText(tr("RF RD FAIL"));
                        }
                    }
                    break;
                case APP_PACKET_RF_WRITE:
                    {
                        APP_PACKET_RF_WRITE_RSP_STRU *pRdRsp = (APP_PACKET_RF_WRITE_RSP_STRU *)pmg->aucData;

                        if (APP_PACKET_RF_ERROR_CODE_SUCC == pRdRsp->ucRslt)
                        {
                            m_pLabelStatus->setText(tr("RF WR SUCC"));
                        }
                        else
                        {
                            m_pLabelStatus->setText(tr("RF WR FAIL"));
                        }
                        
                    }
                    break;
                }        
            }        
            break;
        default: // yet to be implemented
            break;
        }
        
    }

}


void EngineerDlg::MakeData(RF_DATA_LAYOUT_STRU &rfDataLayout) 
{
    // parse header size
    unsigned char *pData;

    int iLoop;

    char tmpbuf[256];

    memset(tmpbuf,0,sizeof(tmpbuf));

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_HEAD_SIZE].offset];
    PARSE_UINT32(m_ulHeaderSize,pData);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_PROPERTY_SIZE].offset];
    PARSE_UINT32(m_ulPropertySize,pData);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_DEVICE].offset];
    PARSE_UINT32(m_ulDevice,pData);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TYPE].offset];
    PARSE_UINT32(m_ulType,pData);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_MARK].offset];
    memset(tmpbuf,0,sizeof(tmpbuf));
    strcpy(tmpbuf,m_strTradeMark.toAscii());
    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_MARK].size; iLoop += 4)
    {
        pData[iLoop]   = tmpbuf[iLoop + 3];
        pData[iLoop+1] = tmpbuf[iLoop + 2];
        pData[iLoop+2] = tmpbuf[iLoop + 1];
        pData[iLoop+3] = tmpbuf[iLoop + 0];
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_NAME].offset];
    memset(tmpbuf,0,sizeof(tmpbuf));
    strcpy(tmpbuf,m_strTradeName.toAscii());
    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_NAME].size; iLoop += 4)
    {
        pData[iLoop]   = tmpbuf[iLoop + 3];
        pData[iLoop+1] = tmpbuf[iLoop + 2];
        pData[iLoop+2] = tmpbuf[iLoop + 1];
        pData[iLoop+3] = tmpbuf[iLoop + 0];
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset];
    memset(tmpbuf,0,sizeof(tmpbuf));
    strcpy(tmpbuf,m_strCatalogueNumber.toAscii());

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size; iLoop += 4)
    {
        pData[iLoop]   = tmpbuf[iLoop + 3];
        pData[iLoop+1] = tmpbuf[iLoop + 2];
        pData[iLoop+2] = tmpbuf[iLoop + 1];
        pData[iLoop+3] = tmpbuf[iLoop + 0];
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW].offset];
    memset(tmpbuf,0,sizeof(tmpbuf));
    strcpy(tmpbuf,m_strUnknow.toAscii());

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW].size; iLoop += 4)
    {
        pData[iLoop]   = tmpbuf[iLoop + 3];
        pData[iLoop+1] = tmpbuf[iLoop + 2];
        pData[iLoop+2] = tmpbuf[iLoop + 1];
        pData[iLoop+3] = tmpbuf[iLoop + 0];
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset];
    memset(tmpbuf,0,sizeof(tmpbuf));
    strcpy(tmpbuf,m_strEditLotNumber.toAscii());

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size; iLoop += 4)
    {
        pData[iLoop]   = tmpbuf[iLoop + 3];
        pData[iLoop+1] = tmpbuf[iLoop + 2];
        pData[iLoop+2] = tmpbuf[iLoop + 1];
        pData[iLoop+3] = tmpbuf[iLoop + 0];
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset];
    
    {
        pData[0] = 0;
        pData[1] = 0;
        pData += 2;
        PARSE_UINT16(0,pData);
    }

}



void EngineerDlg::ParseData(RF_DATA_LAYOUT_STRU &rfDataLayout) 
{
    // parse header size
    unsigned char *pData;

    int iLoop;

    char tmpbuf[256];

    memset(tmpbuf,0,sizeof(tmpbuf));

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_HEAD_SIZE].offset];
    
    m_ulHeaderSize = MAKE_UINT32(pData);

    m_ppEditRfs[RF_DATA_LAYOUT_HEAD_SIZE]->setText(QString::number(m_ulHeaderSize));


    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_PROPERTY_SIZE].offset];
    
    m_ulPropertySize = MAKE_UINT32(pData);

    m_ppEditRfs[RF_DATA_LAYOUT_PROPERTY_SIZE]->setText(QString::number(m_ulPropertySize));

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_DEVICE].offset];
    
    m_ulDevice = MAKE_UINT32(pData);

    m_ppEditRfs[RF_DATA_LAYOUT_DEVICE]->setText(QString::number(m_ulDevice));

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TYPE].offset];
    
    m_ulType = MAKE_UINT32(pData);

    m_ppEditRfs[RF_DATA_LAYOUT_TYPE]->setText(QString::number(m_ulType));

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_MARK].offset];

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_MARK].size; iLoop += 4)
    {
        tmpbuf[iLoop] = pData[iLoop + 3];
        tmpbuf[iLoop+1] = pData[iLoop + 2];
        tmpbuf[iLoop+2] = pData[iLoop + 1];
        tmpbuf[iLoop+3] = pData[iLoop + 0];
    }
    m_strTradeMark = tmpbuf;
    m_ppEditRfs[RF_DATA_LAYOUT_TRADE_MARK]->setText(m_strTradeMark);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_NAME].offset];

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_TRADE_NAME].size; iLoop += 4)
    {
        tmpbuf[iLoop] = pData[iLoop + 3];
        tmpbuf[iLoop+1] = pData[iLoop + 2];
        tmpbuf[iLoop+2] = pData[iLoop + 1];
        tmpbuf[iLoop+3] = pData[iLoop + 0];
    }
    m_strTradeName = tmpbuf;
    m_ppEditRfs[RF_DATA_LAYOUT_TRADE_NAME]->setText(m_strTradeName);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset];

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size; iLoop += 4)
    {
        tmpbuf[iLoop] = pData[iLoop + 3];
        tmpbuf[iLoop+1] = pData[iLoop + 2];
        tmpbuf[iLoop+2] = pData[iLoop + 1];
        tmpbuf[iLoop+3] = pData[iLoop + 0];
    }
    m_strCatalogueNumber = tmpbuf;
    m_ppEditRfs[RF_DATA_LAYOUT_CATALOGUE_NUM]->setText(m_strCatalogueNumber);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW].offset];

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW].size; iLoop += 4)
    {
        tmpbuf[iLoop] = pData[iLoop + 3];
        tmpbuf[iLoop+1] = pData[iLoop + 2];
        tmpbuf[iLoop+2] = pData[iLoop + 1];
        tmpbuf[iLoop+3] = pData[iLoop + 0];
    }
    m_strUnknow = tmpbuf;
    m_ppEditRfs[RF_DATA_LAYOUT_UNKNOW]->setText(m_strUnknow);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset];

    for (iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size; iLoop += 4)
    {
        tmpbuf[iLoop] = pData[iLoop + 3];
        tmpbuf[iLoop+1] = pData[iLoop + 2];
        tmpbuf[iLoop+2] = pData[iLoop + 1];
        tmpbuf[iLoop+3] = pData[iLoop + 0];
    }
    m_strEditLotNumber = tmpbuf;
    m_ppEditRfs[RF_DATA_LAYOUT_LOT_NUMBER]->setText(m_strEditLotNumber);

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset];
    {
        int day,mon,year;

        day = pData[0];
        mon = pData[1];
        year = (pData[2] << 8)|pData[3];

        //CTime   time(year,mon,day,0,0,0);

        //m_installDate = time;

        m_strInstallDate.sprintf("%04d-%02d-%02d",year,mon,day);
        
        m_ppEditRfs[RF_DATA_LAYOUT_INSTALL_DATE]->setText(m_strInstallDate);
    }

    pData = &rfDataLayout.data[m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset];
    
    m_ulUnknowData = MAKE_UINT32(pData);
    
    m_ppEditRfs[RF_DATA_LAYOUT_UNKNOW_DATA]->setText(QString::number(m_ulUnknowData));


}

void EngineerDlg::RfId_CmbIndexChange(int index)
{
    (void)index;
    m_ulRfIdIdx = m_ppCombBoxRfs[0]->currentIndex();

}

