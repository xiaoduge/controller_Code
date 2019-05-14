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

#include "DeviceDlg.h"
#include "mainwindow.h"

#include "keyboard.h"

#include "rpc.h"
#include "sapp.h"
#include "Interface.h"


DeviceDlg::DeviceDlg(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(800, 400);

    m_pParent = parent;
    
    QHBoxLayout *layout = new QHBoxLayout();  

    m_pTblWidget = new QTabWidget();  

    QWidget *widget = new QWidget();  

    QGridLayout *pCfg1Top    =  new QGridLayout; //QGridLayout
    QHBoxLayout *pCfg1Bottom =  new QHBoxLayout;
    QGridLayout *pCfg1Main   =  new QGridLayout();

    m_pBtnQueryId  = new QPushButton;
    m_pBtnQueryId->setText("QueryId");
    m_pBtnQueryVersion  = new QPushButton;
    m_pBtnQueryVersion->setText("QueryVer");
    m_pBtnRmvDevice  = new QPushButton;
    m_pBtnRmvDevice->setText("Remove");

    m_pListWgtDevice   = new QListWidget();

    pCfg1Bottom->addWidget(m_pBtnQueryId);
    pCfg1Bottom->addWidget(m_pBtnQueryVersion);
    pCfg1Bottom->addWidget(m_pBtnRmvDevice);

    pCfg1Top->addWidget(m_pListWgtDevice);

    pCfg1Main->addLayout(pCfg1Top,0,0);
    pCfg1Main->addLayout(pCfg1Bottom,1,0,1,2);

    widget->setLayout(pCfg1Main);  

    QIcon icon1(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(widget, icon1, "Device");     

    /* CFG 2*/
    widget = new QWidget();
    QGridLayout  *pcfg2Top =  new QGridLayout;
    QHBoxLayout  *pcfg2HBox =  new QHBoxLayout;
    QGridLayout *pcfg2Grid = new QGridLayout;

    m_pListWgtHandler   = new QListWidget();

    m_pBtnQueryHandler  = new QPushButton;
    m_pBtnQueryHandler->setText("Query Hdr");
    m_pBtnCfgHandler   = new QPushButton;
    m_pBtnCfgHandler->setText("Cfg Hdr");
    m_pBtnResetHandler   = new QPushButton;
    m_pBtnResetHandler->setText("Reset Hdr");
    m_pBtnRmvHandler   = new QPushButton;
    m_pBtnRmvHandler->setText("rmv Hdr");
    m_pBtnSaveHandler   = new QPushButton;
    m_pBtnSaveHandler->setText("Save Hdr");

    pcfg2Top->addWidget(m_pListWgtHandler);

    pcfg2HBox->addWidget(m_pBtnQueryHandler);
    pcfg2HBox->addWidget(m_pBtnCfgHandler);
    pcfg2HBox->addWidget(m_pBtnResetHandler);
    pcfg2HBox->addWidget(m_pBtnRmvHandler);

    pcfg2Grid->addLayout(pcfg2Top,0,0);
    pcfg2Grid->addLayout(pcfg2HBox,1,0,1,2);

    widget->setLayout(pcfg2Grid);  

    QIcon icon2(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(widget, icon2, "HANDLER");   
    

    /* CFG 3*/
    widget = new QWidget();
    QGridLayout  *pcfg3Top =  new QGridLayout;
    QHBoxLayout  *pcfg3HBox =  new QHBoxLayout;
    QGridLayout *pcfg3Grid = new QGridLayout;

    m_pListWgtRfReader   = new QListWidget();

    m_pBtnQueryRfReader  = new QPushButton;
    m_pBtnQueryRfReader->setText("Query Rfid");
    m_pBtnCfgRfReader   = new QPushButton;
    m_pBtnCfgRfReader->setText("Cfg Rfid");
    m_pBtnResetRfReader   = new QPushButton;
    m_pBtnResetRfReader->setText("Reset Rfid");
    m_pBtnRmvRfReader   = new QPushButton;
    m_pBtnRmvRfReader->setText("rmv Rfid");
    m_pBtnSaveRfReader   = new QPushButton;
    m_pBtnSaveRfReader->setText("Save Rfid");

    pcfg3Top->addWidget(m_pListWgtRfReader);

    pcfg3HBox->addWidget(m_pBtnQueryRfReader);
    pcfg3HBox->addWidget(m_pBtnCfgRfReader);
    pcfg3HBox->addWidget(m_pBtnResetRfReader);
    pcfg3HBox->addWidget(m_pBtnRmvRfReader);

    pcfg3Grid->addLayout(pcfg3Top,0,0);
    pcfg3Grid->addLayout(pcfg3HBox,1,0,1,2);

    widget->setLayout(pcfg3Grid);  

    QIcon icon3(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(widget, icon3, "RFReader");   

    layout->addWidget(m_pTblWidget); 

    /* common */
    QHBoxLayout *pCfgCommon =  new QHBoxLayout;

    m_pBtnCanCel = new QPushButton;
    m_pBtnCanCel->setText("CanCel");
    pCfgCommon->addWidget(m_pBtnCanCel);

    layout->addLayout(pCfgCommon);

    this->setLayout(layout);  

    connect(m_pBtnQueryId,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryId()));
    connect(m_pBtnQueryVersion,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryVersion()));
    connect(m_pBtnRmvDevice,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvDevice()));

    connect(m_pBtnQueryHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryHandler()));
    connect(m_pBtnCfgHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_CfgHandler()));
    connect(m_pBtnResetHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_ResetHandler()));
    connect(m_pBtnRmvHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvHandler()));
    connect(m_pBtnSaveHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvHandler()));

    connect(m_pBtnQueryRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryRfReader()));
    connect(m_pBtnCfgRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_CfgRfReader()));
    connect(m_pBtnResetRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_ResetRfReader()));
    connect(m_pBtnRmvRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvRfReader()));

    connect(m_pBtnCanCel,SIGNAL(clicked()),this,SLOT(on_pushButton_Cancel()));
}

DeviceDlg::~DeviceDlg()
{
    if (m_pBtnQueryId)   delete m_pBtnQueryId;
    if (m_pBtnCanCel)    delete m_pBtnCanCel;

}

void DeviceDlg::on_pushButton_QueryId()
{
    IAP_CAN_CMD_STRU Cmd;
    
    Cmd.iCanChl     = 0;
    Cmd.ucCmd       = SBL_QUERY_ID_CMD;
    Cmd.iPayLoadLen = 0;
    Cmd.ulCanId     = 0x3ff;
    
    DispIapEntry(&Cmd);
}

void DeviceDlg::on_pushButton_QueryVersion()
{
    IAP_CAN_CMD_STRU Cmd;

    int iDevIdx     = m_pListWgtDevice->currentRow();

    if (iDevIdx >= 0 )
    {
        QString strAdr  = getDevice(iDevIdx,1);

        bool ok;
        
        Cmd.iCanChl     = 0;
        Cmd.ucCmd       = SBL_QUERY_VERSION_CMD;
        Cmd.iPayLoadLen = 0;
        Cmd.ulCanId     = strAdr.toInt(&ok,10);

        if (ok)
        {
            DispIapEntry(&Cmd);
        }
    }
}


void DeviceDlg::on_pushButton_RmvDevice()
{
    int iLoop;
    QListWidgetItem *pItem;    
    DeviceItem *pDevice;
    int iCount = m_pListWgtDevice->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        //pItem = m_pListWgtDevice->item(iLoop);

        //pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);

        pItem = m_pListWgtDevice->takeItem(0);

        pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);

        m_pListWgtDevice->removeItemWidget(pItem);
        
        delete pItem;       

        delete pDevice;

    }

   // m_pListWgtDevice->reset();

}

void DeviceDlg::on_pushButton_Cancel()
{
    close();
}

void DeviceDlg::on_pushButton_QueryHandler()
{
    char buf[64];
    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;

    APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;

    
    pCmd->iCanChl     = 0;
    pCmd->ucCmd       = SAPP_CMD_DATA;
    pCmd->iPayLoadLen = APP_POROTOL_PACKET_HO_COMMON_LENGTH;
    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;

    pHo->hdr.ucLen     = 1;
    pHo->hdr.ucTransId = 0XF0;
    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;

    pHo->ucOpsType     = APP_PACKET_HO_ADR_QRY;

    DispAfEntry(pCmd);

}

void DeviceDlg::on_pushButton_CfgHandler()
{
    QListWidgetItem *pItem;    
    HandlerItem     *pHandler;
    int             iLoop;
    
    unsigned int    ulAllocMsk = 0;

    unsigned char  aucAdrCnt[APP_HAND_SET_MAX_NUMBER];
    
    int iCount = m_pListWgtHandler->count();

    memset(aucAdrCnt,0,sizeof(aucAdrCnt));

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        int adr = pHandler->getItemText(HANDLER_ITEM_ADDRESS).toInt();


        if (!(adr >= APP_HAND_SET_BEGIN_ADDRESS
               && adr < APP_HAND_SET_END_ADDRESS ))
        {
            ulAllocMsk |= (1 << iLoop);
        }
        else 
        {
            if (aucAdrCnt[adr - APP_HAND_SET_BEGIN_ADDRESS])
            {
                ulAllocMsk |= (1 << iLoop); /* Need to config incase address conflict */
            }
            else
            {
                aucAdrCnt[adr - APP_HAND_SET_BEGIN_ADDRESS] = 1;
            }
        
        }
    }

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        if (ulAllocMsk & ((1 << iLoop)))
        {
            if ((iLoop + APP_HAND_SET_BEGIN_ADDRESS >= APP_HAND_SET_BEGIN_ADDRESS
                   && iLoop + APP_HAND_SET_BEGIN_ADDRESS < APP_HAND_SET_END_ADDRESS ))
            {
                /* to config */
                {
                    char buf[64];
                    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
                
                    APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;

                    pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

                    QString strSn = pHandler->getItemText(HANDLER_ITEM_SN);
                
                    
                    pCmd->iCanChl     = 0;
                    pCmd->ucCmd       = SAPP_CMD_DATA;
                    pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
                    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;
                
                    pHo->hdr.ucLen     = 9 + 2 + 1;
                    pHo->hdr.ucTransId = 0XF0;
                    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
                    pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
                
                    pHo->ucOpsType     = APP_PACKET_HO_ADR_SET;

                    QByteArray bytes = strSn.toAscii();

                    memcpy(&pHo->aucData[0],bytes.data(),bytes.size());

                    pHo->aucData[9] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 8) & 0XFF;
                    pHo->aucData[10] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 0) & 0XFF;

                    pCmd->iPayLoadLen += pHo->hdr.ucLen;  
                    
                    DispAfEntry(pCmd);
                
                }                
            }
        }
    }    
}

void DeviceDlg::on_pushButton_ResetHandler()
{
    char buf[32];
    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;

    APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;

    
    pCmd->iCanChl     = 0;
    pCmd->ucCmd       = SAPP_CMD_DATA;
    pCmd->iPayLoadLen = APP_POROTOL_PACKET_HO_COMMON_LENGTH;
    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;

    pHo->hdr.ucLen     = 1;
    pHo->hdr.ucTransId = 0XF0;
    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;

    pHo->ucOpsType     = APP_PACKET_HO_ADR_RST;

    DispAfEntry(pCmd);
}


void DeviceDlg::on_pushButton_RmvHandler()
{
    int iLoop;
    QListWidgetItem *pItem;    
    HandlerItem *pDevice;
    int iCount = m_pListWgtHandler->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        //pItem = m_pListWgtHandler->item(iLoop);

        pItem = m_pListWgtHandler->takeItem(0);

        pDevice = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);
        
        m_pListWgtHandler->removeItemWidget(pItem);

        delete pItem;       

        delete pDevice;

    }

    //m_pListWgtHandler->reset();

}

void DeviceDlg::on_pushButton_SaveHandler()
{
    int iLoop;
    int iCount = m_pListWgtHandler->count();
    HandlerItem *pHandler;
    QListWidgetItem *pItem;  

    bool bDefault;

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (pHandler->getCheckState())
        {
            bDefault = true ; 
        }

        if (pHandler->getAddress() < APP_HAND_SET_BEGIN_ADDRESS
            || pHandler->getAddress() >= APP_HAND_SET_END_ADDRESS)
        {
            return;
        }
    }

    if (bDefault)
    {
        DB_HANDLER_STRU hdl[APP_HAND_SET_MAX_NUMBER];
        int iMask = 0;
        for (iLoop = 0; iLoop < iCount; iLoop++)
        {
            int addr;
            int type;
            int def;
            int idx;
            
            pItem = m_pListWgtHandler->item(iLoop);
        
            pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

            addr = pHandler->getAddress();
            type = pHandler->getType();
            def  = pHandler->getCheckState();

            idx = addr - APP_HAND_SET_BEGIN_ADDRESS;

            iMask |= 1 << idx;

            hdl[idx].address = addr;
            hdl[idx].type    = type;
            hdl[idx].def     = def;
            strncpy(hdl[idx].name,pHandler->getItemText(HANDLER_ITEM_SN).toAscii(),APP_SN_LENGTH);

        }

        gpMainWnd->updHandler(iMask,hdl);
    }
}

void DeviceDlg::on_pushButton_QueryRfReader()
{
    char buf[64];
    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;

    APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;

    
    pCmd->iCanChl     = 0;
    pCmd->ucCmd       = SAPP_CMD_DATA;
    pCmd->iPayLoadLen = APP_POROTOL_PACKET_RF_COMMON_LENGTH;
    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;

    pHo->hdr.ucLen     = 1;
    pHo->hdr.ucTransId = 0XF0;
    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;

    pHo->ucOpsType     = APP_PACKET_RF_ADR_QRY;

    DispAfEntry(pCmd);

}

void DeviceDlg::on_pushButton_CfgRfReader()
{
    QListWidgetItem *pItem;    
    RfReaderItem     *pHandler;
    int             iLoop;
    
    unsigned int    ulAllocMsk = 0;

    unsigned char   aucAdrCnt[APP_RF_READER_MAX_NUMBER];
    
    int iCount = m_pListWgtRfReader->count();

    memset(aucAdrCnt,0,sizeof(aucAdrCnt));

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtRfReader->item(iLoop);

        pHandler = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);

        int adr = pHandler->getItemText(RFREADER_ITEM_ADDRESS).toInt();


        if (!(adr >= APP_RF_READER_BEGIN_ADDRESS
               && adr < APP_RF_READER_END_ADDRESS ))
        {
            ulAllocMsk |= (1 << iLoop);
        }
        else 
        {
            if (aucAdrCnt[adr - APP_RF_READER_BEGIN_ADDRESS])
            {
                ulAllocMsk |= (1 << iLoop); /* Need to config incase address conflict */
            }
            else
            {
                aucAdrCnt[adr - APP_RF_READER_BEGIN_ADDRESS] = 1;
            }
        
        }
    }

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtRfReader->item(iLoop);

        if (ulAllocMsk & ((1 << iLoop)))
        {
            if ((iLoop + APP_RF_READER_BEGIN_ADDRESS >= APP_RF_READER_BEGIN_ADDRESS
               && iLoop + APP_RF_READER_BEGIN_ADDRESS < APP_RF_READER_END_ADDRESS ))
            {
                /* to config */
                {
                    char buf[64];
                    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
                
                    APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

                    pHandler = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);

                    QString strSn = pHandler->getItemText(RFREADER_ITEM_SN);
                
                    
                    pCmd->iCanChl     = 0;
                    pCmd->ucCmd       = SAPP_CMD_DATA;
                    pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
                    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;
                
                    pHo->hdr.ucLen     = 9 + 2 + 1;
                    pHo->hdr.ucTransId = 0XF0;
                    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
                    pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
                
                    pHo->ucOpsType     = APP_PACKET_RF_ADR_SET;

                    QByteArray bytes = strSn.toAscii();

                    memcpy(&pHo->aucData[0],bytes.data(),bytes.size());

                    pHo->aucData[9] = ((iLoop + APP_RF_READER_BEGIN_ADDRESS) >> 8) & 0XFF;
                    pHo->aucData[10] = ((iLoop + APP_RF_READER_BEGIN_ADDRESS) >> 0) & 0XFF;

                    pCmd->iPayLoadLen += pHo->hdr.ucLen;  

                    DispAfEntry(pCmd);
                
                }                
            }
        }
    }    
}

void DeviceDlg::on_pushButton_ResetRfReader()
{
    char buf[32];
    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;

    APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

    
    pCmd->iCanChl     = 0;
    pCmd->ucCmd       = SAPP_CMD_DATA;
    pCmd->iPayLoadLen = APP_POROTOL_PACKET_RF_COMMON_LENGTH;
    pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;

    pHo->hdr.ucLen     = 1;
    pHo->hdr.ucTransId = 0XF0;
    pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;

    pHo->ucOpsType     = APP_PACKET_RF_ADR_RST;

    DispAfEntry(pCmd);
}


void DeviceDlg::on_pushButton_RmvRfReader()
{
    int iLoop;
    QListWidgetItem *pItem;    
    RfReaderItem *pDevice;
    int iCount = m_pListWgtRfReader->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        //pItem = m_pListWgtHandler->item(iLoop);

        pItem = m_pListWgtRfReader->takeItem(0);

        pDevice = (RfReaderItem *)m_pListWgtHandler->itemWidget(pItem);
        
        m_pListWgtRfReader->removeItemWidget(pItem);

        delete pItem;       

        delete pDevice;

    }

    //m_pListWgtHandler->reset();

}

void DeviceDlg::on_pushButton_SaveRfReader()
{
    int iLoop;
    QListWidgetItem *pItem;    
    RfReaderItem *pDevice;
    int iCount = m_pListWgtRfReader->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
 
    }

}


bool DeviceDlg::eventFilter(QObject *watched, QEvent *event)
{

    return QDialog::eventFilter(watched,event);
}


void DeviceDlg::addDevice(const QString& text0,const QString& text1, const QString& text2)
{
    if (!searchDeviceByElecId(text0))
    {
        DeviceItem *pDevice = new DeviceItem(this, text0, text1, text2);
        QListWidgetItem *lstItem = new QListWidgetItem();
        lstItem->setSizeHint(QSize(0, 50));
        
        m_pListWgtDevice->addItem(lstItem);
        m_pListWgtDevice->setItemWidget(lstItem, pDevice);
    }
}

QListWidgetItem * DeviceDlg::searchDeviceByElecId(const QString& info)
{
   return searchDevice(info,0);
    
}

QListWidgetItem * DeviceDlg::searchDeviceByAddress(const QString& info)
{
    return searchDevice(info,1);
    
}

QListWidgetItem * DeviceDlg::searchDevice(const QString& info,int iIndex)
{
    int iLoop;
    QListWidgetItem *pItem;    
    DeviceItem *pDevice;
    int iCount = m_pListWgtDevice->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtDevice->item(iLoop);

        pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);

        if (QString::compare(info,pDevice->getItemText(iIndex)) == 0)
        {
            return  pItem ; 
        }
    }

    return NULL;    
}

QString DeviceDlg::getDevice(int iDevIdx,int iInfoIdx)
{
    QListWidgetItem *pItem;    
    DeviceItem *pDevice;
    
    int iCount = m_pListWgtDevice->count();

    if (iCount <= iDevIdx)
    {
        return "";
    }

    pItem = m_pListWgtDevice->item(iDevIdx);
    
    pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);
    
    return pDevice->getItemText(iInfoIdx);

}

void DeviceDlg::deviceVersion(const QString& addr,const QString& version)
{
    QListWidgetItem *pItem;    
    DeviceItem *pDevice;

    if ((pItem = searchDeviceByAddress(addr)))
    {
        pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);

        pDevice->setItemText(DEVICE_ITEM_VERSION,version);
    }
}


QListWidgetItem * DeviceDlg::searchHandler(const QString& info,int iIndex)
{
    int iLoop;
    QListWidgetItem *pItem;    
    HandlerItem *pHandler;
    int iCount = m_pListWgtHandler->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (QString::compare(info,pHandler->getItemText(iIndex)) == 0)
        {
            return  pItem ; 
        }
    }

    return NULL;    
}


void DeviceDlg::on_checkBox_changeState(int state)
{
    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }
    
    int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;
    
    int iLoop;
    int iCount = m_pListWgtHandler->count();
    HandlerItem *pHandler;
    QListWidgetItem *pItem;    

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (tmp && pHandler->getCheckState())
        {
            pHandler->setCheckState(false) ; 
        }
    }
    
}


QString DeviceDlg::getHandler(int iDevIdx,int iInfoIdx)
{
    QListWidgetItem *pItem;    
    HandlerItem *pDevice;
    
    int iCount = m_pListWgtHandler->count();

    if (iCount <= iDevIdx)
    {
        return "";
    }

    pItem = m_pListWgtHandler->item(iDevIdx);
    
    pDevice = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);
    
    return pDevice->getItemText(iInfoIdx);

}

QListWidgetItem * DeviceDlg::searchHandlerBySN(const QString& info)
{
   return searchHandler(info,0);
    
}

QListWidgetItem * DeviceDlg::searchHandlerByAddress(const QString& info)
{
    return searchHandler(info,1);
    
}

void DeviceDlg::addHandler(const QString& text0,const QString& text1,const QString& text2)
{
    QListWidgetItem * item = searchHandlerBySN(text0);

    if (!item)
    {
        HandlerItem *pHandler = new HandlerItem(this, text0, text1,text2);
        QListWidgetItem *lstItem = new QListWidgetItem();
        lstItem->setSizeHint(QSize(0, 50));
        
        m_pListWgtHandler->addItem(lstItem);
        m_pListWgtHandler->setItemWidget(lstItem, pHandler);
    }
    else
    {
        HandlerItem *pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(item);

        pHandler->setItemText(HANDLER_ITEM_ADDRESS,text1);
    }
}

QListWidgetItem * DeviceDlg::searchRfReader(const QString& info,int iIndex)
{
    int iLoop;
    QListWidgetItem *pItem;    
    RfReaderItem *pHandler;
    int iCount = m_pListWgtRfReader->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtRfReader->item(iLoop);

        pHandler = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);

        if (QString::compare(info,pHandler->getItemText(iIndex)) == 0)
        {
            return  pItem ; 
        }
    }

    return NULL;    
}

QString DeviceDlg::getRfReader(int iDevIdx,int iInfoIdx)
{
    QListWidgetItem *pItem;    
    RfReaderItem *pDevice;
    
    int iCount = m_pListWgtRfReader->count();

    if (iCount <= iDevIdx)
    {
        return "";
    }

    pItem = m_pListWgtRfReader->item(iDevIdx);
    
    pDevice = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);
    
    return pDevice->getItemText(iInfoIdx);

}

QListWidgetItem * DeviceDlg::searchRfReaderBySN(const QString& info)
{
   return searchRfReader(info,0);
    
}

QListWidgetItem * DeviceDlg::searchRfReaderByAddress(const QString& info)
{
    return searchRfReader(info,1);
    
}

void DeviceDlg::addRfReader(const QString& text0,const QString& text1)
{
    QListWidgetItem * item = searchRfReaderBySN(text0);

    if (!item)
    {
        RfReaderItem *pHandler = new RfReaderItem(this, text0, text1);
        QListWidgetItem *lstItem = new QListWidgetItem();
        lstItem->setSizeHint(QSize(0, 50));
        
        m_pListWgtRfReader->addItem(lstItem);
        m_pListWgtRfReader->setItemWidget(lstItem, pHandler);
    }
    else
    {
        RfReaderItem *pHandler = (RfReaderItem *)m_pListWgtRfReader->itemWidget(item);

        pHandler->setItemText(RFREADER_ITEM_ADDRESS,text1);
    }
}


