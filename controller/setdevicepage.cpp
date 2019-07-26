#include "setdevicepage.h"

#include "titlebar.h"
#include "mainwindow.h"
#include "cbitmapbutton.h"
#include <QScrollBar>
#include <QRect>
#include <QFile>
#include <QFileInfo>
#include "rpc.h"
#include "sapp.h"
#include "Interface.h"
#include "ToastDlg.h"

SetDevicePage::SetDevicePage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

}

void SetDevicePage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SetDevicePage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Connecting Device");

    setTitles(stringList);

}

void SetDevicePage::buildTranslation()
{
    m_pBtnQueryId->setText(tr("QueryId"));
    m_pBtnQueryVersion->setText(tr("QueryVer"));
    m_pBtnRmvDevice->setText(tr("Clear Screen"));

    m_plbDeviceSN->setText(tr("SN"));
    m_plbDeviceAdr->setText(tr("Address"));     
    m_plbDeviceType->setText(tr("Type"));
    m_plbDeviceVers->setText(tr("Version"));

    m_plbHandlerSN->setText(tr("SN"));
    m_plbHandlerAdr->setText(tr("Address"));     
    m_plbHandlerDef->setText(tr("Default"));
    m_plbHandlerType->setText(tr("Type"));
    m_plbHandlerOper->setText(tr("Operate"));

    m_pBtnQueryHandler->setText(tr("Query"));
    m_pBtnCfgHandler->setText(tr("Config."));
    m_pBtnResetHandler->setText(tr("Reset"));
//    m_pBtnRmvHandler->setText(tr("Clear"));
    m_pBtnSaveHandler->setText(tr("Save"));

#ifdef AUTO_CFG_RF_READER    
    m_pBtnCfgRfReader->setText(tr("Cfg Rfid"));
    m_pBtnResetRfReader->setText(tr("Reset Rfid"));
#endif    
    m_pBtnQueryRfReader->setText(tr("Query"));
    m_pBtnRmvRfReader->setText(tr("Clear Screen"));
    m_pBtnSaveRfReader->setText(tr("Save Rfid"));
    m_plbRfReaderSN->setText(tr("SN"));
    m_plbRfReaderAdr->setText(tr("Address"));

    m_pBtnQueryRfReader->setText(tr("Query"));
    m_pBtnRmvRfReader->setText(tr("Clear"));
    m_pBtnSaveRfReader->setText(tr("Save"));

    m_plbRfReaderSN->setText(tr("SN"));
    m_plbRfReaderAdr->setText(tr("Address"));     

    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));

    m_pTblWidget->setTabText(0,tr("Device"));
    m_pTblWidget->setTabText(1,tr("Dispenser"));
    m_pTblWidget->setTabText(2,tr("RF Reader"));
//    m_pTblWidget->setTabText(3,tr("Maintenance"));
}

void SetDevicePage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void SetDevicePage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void SetDevicePage::finishSave()
{
    ToastDlg::makeToast(tr("Successfully saved"));
}

void SetDevicePage::on_CmbIndexChange_trx_type(int index)
{

    int iIdx = m_pcombTrxType->currentIndex();

    if (APP_TRX_ZIGBEE == iIdx
        && !(gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_ZIGBEE)))
    {
        m_pcombTrxType->setCurrentIndex(APP_TRX_CAN);
    }

}

void SetDevicePage::initUi()
{
    setBackColor();

    //QFile qss(":/app/checkbox.qss");
    //qss.open(QFile::ReadOnly);
    //m_strQss4Chk = QLatin1String (qss.readAll());
    //qss.close();    
    m_strQss4Chk = m_wndMain->getQss4Chk();

    QHBoxLayout *layout = new QHBoxLayout(); 

    m_widgetLayout = m_widget;

    m_widgetLayout = new QWidget(m_widget);

    m_widgetLayout->setGeometry(QRect(0,55,800,height() - 55));

    layout->setGeometry(QRect(0,100,m_widgetLayout->geometry().width(),m_widgetLayout->geometry().height()-120));

    m_pTblWidget = new QTabWidget();  

    QWidget *widget = new QWidget();  

    QGridLayout *pCfg1Top     =  new QGridLayout; //QGridLayout
    QHBoxLayout *pCfg1Bottom  =  new QHBoxLayout;
    QGridLayout *pCfg1Main    =  new QGridLayout;
    QHBoxLayout *pcfg1TilHBox =  new QHBoxLayout;

    m_pBtnQueryId  = new QPushButton;
    m_pBtnQueryVersion  = new QPushButton;
    m_pBtnRmvDevice  = new QPushButton;

    m_pListWgtDevice   = new QListWidget();

    pCfg1Bottom->addWidget(m_pBtnQueryId);
    pCfg1Bottom->addWidget(m_pBtnQueryVersion);
    pCfg1Bottom->addWidget(m_pBtnRmvDevice);

    m_plbDeviceSN = new QLabel;
    m_plbDeviceSN->setAlignment(Qt::AlignCenter);
    m_plbDeviceAdr = new QLabel;
    m_plbDeviceAdr->setAlignment(Qt::AlignCenter);
    m_plbDeviceType = new QLabel;
    m_plbDeviceType->setAlignment(Qt::AlignCenter);
    m_plbDeviceVers= new QLabel;
    m_plbDeviceVers->setAlignment(Qt::AlignCenter);

    pcfg1TilHBox->addWidget(m_plbDeviceSN);
    pcfg1TilHBox->addWidget(m_plbDeviceAdr);
    pcfg1TilHBox->addWidget(m_plbDeviceType);
    pcfg1TilHBox->addWidget(m_plbDeviceVers);

    pCfg1Top->addLayout(pcfg1TilHBox,0,0);
    pCfg1Top->addWidget(m_pListWgtDevice);
    m_pListWgtDevice->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:10px;}");

    pCfg1Main->addLayout(pCfg1Top,0,0);
    pCfg1Main->addLayout(pCfg1Bottom,1,0,1,2);

    widget->setLayout(pCfg1Main);  

    QIcon icon1(":/pic/unselected.png");  
    m_pTblWidget->addTab(widget, icon1, tr("Device"));     

    /* CFG 2*/
    widget = new QWidget();
    QGridLayout  *pcfg2Top =  new QGridLayout;
    QHBoxLayout  *pcfg2HBox =  new QHBoxLayout;
    QGridLayout  *pcfg2Grid = new QGridLayout;
    QHBoxLayout  *pcfg2TilHBox =  new QHBoxLayout;

    m_pListWgtHandler   = new QListWidget();
   
    
    m_plbHandlerSN = new QLabel;
    m_plbHandlerSN->setAlignment(Qt::AlignCenter);
    m_plbHandlerAdr = new QLabel;
    m_plbHandlerAdr->setAlignment(Qt::AlignCenter);
    m_plbHandlerDef = new QLabel;
    m_plbHandlerType = new QLabel;
    m_plbHandlerOper = new QLabel;
    

    m_pcombTrxType     = new QComboBox;

    m_pcombTrxType->addItem(tr("CAN"));
    m_pcombTrxType->addItem(tr("Zigbee"));
    m_pcombTrxType->setCurrentIndex(0);

    connect(m_pcombTrxType, SIGNAL(currentIndexChanged(int)),
     this, SLOT(on_CmbIndexChange_trx_type(int)));

    m_pBtnQueryHandler  = new QPushButton;
    m_pBtnCfgHandler   = new QPushButton;
    m_pBtnResetHandler   = new QPushButton;
//    m_pBtnRmvHandler   = new QPushButton;
    m_pBtnSaveHandler   = new QPushButton;

    pcfg2TilHBox->addWidget(m_plbHandlerSN);
    pcfg2TilHBox->addWidget(m_plbHandlerAdr);
    pcfg2TilHBox->addWidget(m_plbHandlerDef);
    pcfg2TilHBox->addWidget(m_plbHandlerType);
    pcfg2TilHBox->addWidget(m_plbHandlerOper);

    //m_pListWgtHandler->setLayout(pcfg2TilHBox);
    pcfg2Top->addLayout(pcfg2TilHBox,0,0);
    pcfg2Top->addWidget(m_pListWgtHandler);

    pcfg2HBox->addWidget(m_pcombTrxType);
    pcfg2HBox->addWidget(m_pBtnQueryHandler);
    pcfg2HBox->addWidget(m_pBtnCfgHandler);
    pcfg2HBox->addWidget(m_pBtnResetHandler);
//    pcfg2HBox->addWidget(m_pBtnRmvHandler);
    pcfg2HBox->addWidget(m_pBtnSaveHandler);

    pcfg2Grid->addLayout(pcfg2Top,0,0);
    pcfg2Grid->addLayout(pcfg2HBox,1,0,1,2);

    widget->setLayout(pcfg2Grid);  

    QIcon icon2(":/pic/unselected.png");  
    m_pTblWidget->addTab(widget, icon2, tr("HANDLER"));

    /* CFG 3*/
    widget = new QWidget();
    QGridLayout  *pcfg3Top =  new QGridLayout;
    QHBoxLayout  *pcfg3HBox =  new QHBoxLayout;
    QGridLayout *pcfg3Grid = new QGridLayout;
    QHBoxLayout  *pcfg3TilHBox =  new QHBoxLayout;

    m_pListWgtRfReader   = new QListWidget();

    m_pBtnQueryRfReader  = new QPushButton;
#ifdef AUTO_CFG_RF_READER    
    m_pBtnCfgRfReader   = new QPushButton;
    m_pBtnResetRfReader   = new QPushButton;
#endif    
    m_pBtnRmvRfReader   = new QPushButton;
    m_pBtnSaveRfReader   = new QPushButton;

    m_plbRfReaderSN = new QLabel;
    m_plbRfReaderSN->setAlignment(Qt::AlignCenter);
    m_plbRfReaderAdr= new QLabel;
    m_plbRfReaderAdr->setAlignment(Qt::AlignCenter);

    pcfg3TilHBox->addWidget(m_plbRfReaderSN);
    pcfg3TilHBox->addWidget(m_plbRfReaderAdr);

    pcfg3Top->addLayout(pcfg3TilHBox,0,0);
    pcfg3Top->addWidget(m_pListWgtRfReader);
    m_pListWgtRfReader->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:10px;}");

    pcfg3HBox->addWidget(m_pBtnQueryRfReader);
#ifdef AUTO_CFG_RF_READER    
    pcfg3HBox->addWidget(m_pBtnCfgRfReader);
    pcfg3HBox->addWidget(m_pBtnResetRfReader);
#endif
    pcfg3HBox->addWidget(m_pBtnRmvRfReader);
    pcfg3HBox->addWidget(m_pBtnSaveRfReader);

    pcfg3Grid->addLayout(pcfg3Top,0,0);
    pcfg3Grid->addLayout(pcfg3HBox,1,0,1,2);

    widget->setLayout(pcfg3Grid);  

    QIcon icon3(":/pic/unselected.png");  
    m_pTblWidget->addTab(widget, icon3, tr("RFReader"));   

    /* CFG 4*/
    widget = new QWidget();
    QHBoxLayout  *pcfg4HBox =  new QHBoxLayout;


    m_plbZigbeeUpd = new QLabel;
    m_plbZigbeeUpd->setAlignment(Qt::AlignCenter);
    m_plbZigbeeUpd->setText("00");
    pcfg4HBox->addWidget(m_plbZigbeeUpd);

    m_pBtnZigbeeUpd  = new QPushButton;
    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));
    pcfg4HBox->addWidget(m_pBtnZigbeeUpd);

    widget->setLayout(pcfg4HBox);  

//    QIcon icon4(":/pic/unselected.png");
//    m_pTblWidget->addTab(widget, icon4, tr("Maintenance"));


    layout->addWidget(m_pTblWidget); 

    m_widgetLayout->setLayout(layout);

    connect(m_pBtnQueryId,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryId()));
    connect(m_pBtnQueryVersion,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryVersion()));
    connect(m_pBtnRmvDevice,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvDevice()));

    connect(m_pBtnQueryHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryHandler()));
    connect(m_pBtnCfgHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_CfgHandler()));
    connect(m_pBtnResetHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_ResetHandler()));
//    connect(m_pBtnRmvHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvHandler()));
    connect(m_pBtnSaveHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_SaveHandler()));

    connect(m_pBtnQueryRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryRfReader()));
#ifdef AUTO_CFG_RF_READER    
    connect(m_pBtnCfgRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_CfgRfReader()));
    connect(m_pBtnResetRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_ResetRfReader()));
#endif
    connect(m_pBtnRmvRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvRfReader()));
    connect(m_pBtnSaveRfReader,SIGNAL(clicked()),this,SLOT(on_pushButton_SaveRfReader()));

    connect(m_pBtnZigbeeUpd,SIGNAL(clicked()),this,SLOT(on_pushButton_ZigbeeUpd()));

}

void SetDevicePage::on_btn_clicked(int index)
{
    (void)index;
}

void SetDevicePage::on_pushButton_QueryId()
{
    IAP_CAN_CMD_STRU Cmd;
    
    Cmd.iCanChl     = 0;
    Cmd.ucCmd       = SBL_QUERY_ID_CMD;
    Cmd.iPayLoadLen = 0;
    Cmd.ulCanId     = 0x3ff;
    
    DispIapEntry(&Cmd);

    m_wndMain->prepareKeyStroke();
}

void SetDevicePage::on_pushButton_QueryVersion()
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
    m_wndMain->prepareKeyStroke();
}


void SetDevicePage::on_pushButton_RmvDevice()
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
    m_wndMain->prepareKeyStroke();
}


void SetDevicePage::on_pushButton_QueryHandler()
{
    char buf[64];

    on_pushButton_RmvHandler();  //20190108 test

    switch (m_pcombTrxType->currentIndex())
    {
    case APP_TRX_CAN:
        {
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
        break;
    case APP_TRX_ZIGBEE:
        {
           
            IAP_ZB_CMD_STRU *pCmd = (IAP_ZB_CMD_STRU *)buf;
        
            APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;
    
            pCmd->addr.addrMode = AddrBroadcast;
            pCmd->addr.addr.shortAddr = 0XFFFF;
            
            pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
        
            pHo->hdr.ucLen     = 1;
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
            
            pHo->ucOpsType     = APP_PACKET_HO_ADR_QRY;

            pCmd->iPayLoadLen += pHo->hdr.ucLen;  
            
            DispZigbeeEntry(pCmd);
        }                    
        break;
    
    }    
    m_wndMain->prepareKeyStroke();
}


void SetDevicePage::cfgHandlerRsp()
{
    ToastDlg::makeToast(tr("Be sure to save the handsets' configuration when collected all responses!"));
}

void SetDevicePage::on_pushButton_CfgHandler()
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
                ulAllocMsk |= (1 << iLoop); /* Need to config in case address conflict */
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
            if ((iLoop + APP_HAND_SET_BEGIN_ADDRESS >= APP_HAND_SET_BEGIN_ADDRESS)
                 && (iLoop + APP_HAND_SET_BEGIN_ADDRESS < APP_HAND_SET_END_ADDRESS ))
            {
                /* to config */
                char buf[64];
                
                switch (m_pcombTrxType->currentIndex())
                {
                case APP_TRX_CAN:
                    {
                        IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
                    
                        APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;
        
                        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);
        
                        QString strSn = pHandler->getItemText(HANDLER_ITEM_SN);
                    
                        
                        pCmd->iCanChl     = 0;
                        pCmd->ucCmd       = SAPP_CMD_DATA;
                        pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
                        pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;
                    
                        pHo->hdr.ucLen     = APP_SN_LENGTH + 2 + 1;
                        pHo->hdr.ucTransId = 0XF0;
                        pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
                        pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
                    
                        pHo->ucOpsType     = APP_PACKET_HO_ADR_SET;
        
                        QByteArray bytes = strSn.toAscii();
        
                        memcpy(&pHo->aucData[0],bytes.data(),bytes.size());
        
                        pHo->aucData[APP_SN_LENGTH + 0] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 8) & 0XFF;
                        pHo->aucData[APP_SN_LENGTH + 1] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 0) & 0XFF;
        
                        pCmd->iPayLoadLen += pHo->hdr.ucLen;  
                        
                        DispAfEntry(pCmd);
                    }
                    break;
                case APP_TRX_ZIGBEE:
                    {
                        
                        IAP_ZB_CMD_STRU *pCmd = (IAP_ZB_CMD_STRU *)buf;
                    
                        APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;
        
                        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);
        
                        QString strSn = pHandler->getItemText(HANDLER_ITEM_SN);
                    
                        pCmd->addr.addrMode = AddrBroadcast;
                        pCmd->addr.addr.shortAddr = 0XFFFF;
                        
                        pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
                    
                        pHo->hdr.ucLen     = APP_SN_LENGTH + 2 + 1;
                        pHo->hdr.ucTransId = 0XF0;
                        pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
                        pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
                    
                        pHo->ucOpsType     = APP_PACKET_HO_ADR_SET;
        
                        QByteArray bytes = strSn.toAscii();
        
                        memcpy(&pHo->aucData[0],bytes.data(),bytes.size());
        
                        pHo->aucData[APP_SN_LENGTH + 0] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 8) & 0XFF;
                        pHo->aucData[APP_SN_LENGTH + 1] = ((iLoop + APP_HAND_SET_BEGIN_ADDRESS) >> 0) & 0XFF;
        
                        pCmd->iPayLoadLen += pHo->hdr.ucLen;  
                        
                        DispZigbeeEntry(pCmd);
                    }                    
                    break;
                
                }                
            }
        }
    }
    m_wndMain->prepareKeyStroke();
    on_pushButton_RmvHandler();  //20190108
    on_pushButton_QueryHandler();  //20190108
}

void SetDevicePage::on_pushButton_ResetHandler()
{
    char buf[32];
    IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;

    APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;

    switch (m_pcombTrxType->currentIndex())
    {
    case APP_TRX_CAN:
        {
            IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
            
            APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;
            
            pCmd->iCanChl     = 0;
            pCmd->ucCmd       = SAPP_CMD_DATA;
            pCmd->ulCanId     = APP_PROTOL_CANID_BROADCAST;
            pCmd->iPayLoadLen = APP_POROTOL_PACKET_HO_COMMON_LENGTH;
            
            pHo->hdr.ucLen     = 1;
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
            
            pHo->ucOpsType     = APP_PACKET_HO_ADR_RST;
            
            DispAfEntry(pCmd);

        }
        break;
    case APP_TRX_ZIGBEE:
        {
            
            IAP_ZB_CMD_STRU *pCmd = (IAP_ZB_CMD_STRU *)buf;
        
            APP_PACKET_HO_STRU *pHo = (APP_PACKET_HO_STRU *)pCmd->data;
    

            pCmd->addr.addrMode = AddrBroadcast;
            pCmd->addr.addr.shortAddr = 0XFFFF;
            
            pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
        
            pHo->hdr.ucLen     = 1;
            pHo->hdr.ucTransId = 0XF0;
            pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
            pHo->hdr.ucMsgType = APP_PACKET_HAND_OPERATION;
            
            pHo->ucOpsType     = APP_PACKET_HO_ADR_RST;

            pCmd->iPayLoadLen += pHo->hdr.ucLen;  
            
            DispZigbeeEntry(pCmd);
        }                    
        break;
    
    }
    m_wndMain->prepareKeyStroke();
    on_pushButton_RmvHandler();  //20190108
    on_pushButton_QueryHandler();  //20190108

}


void SetDevicePage::on_pushButton_RmvHandler()
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
    m_wndMain->prepareKeyStroke();
}

void SetDevicePage::on_pushButton_SaveHandler()
{
    int iLoop;
    int iCount = m_pListWgtHandler->count();
    HandlerItem *pHandler;
    QListWidgetItem *pItem;  

    int iDefault[2] = {0,0};

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (pHandler->getCheckState())
        {
            iDefault[pHandler->getType()]++ ; 
        }

        if (pHandler->getAddress() < APP_HAND_SET_BEGIN_ADDRESS
            || pHandler->getAddress() >= APP_HAND_SET_END_ADDRESS)
        {
            return;
        }
    }

    if (1 == iDefault[0]
        || 1 == iDefault[1])
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

            memset(&hdl[idx],0,sizeof(DB_HANDLER_STRU));
            
            hdl[idx].address = addr;
            hdl[idx].type    = type;
            hdl[idx].def     = def;

            
            strncpy(hdl[idx].name,pHandler->getItemText(HANDLER_ITEM_SN).toAscii(),APP_SN_LENGTH);


            qDebug() << __FUNCTION__ << pHandler->getItemText(HANDLER_ITEM_SN) << hdl[idx].name << hdl[idx].address << hdl[idx].type << hdl[idx].def;
        }

        m_wndMain->updHandler(iMask,hdl);
        m_wndMain->prepareKeyStroke();
        finishSave();
    }
    else
    {
        m_wndMain->prepareKeyStroke();
        QMessageBox::about(NULL, tr("About"), tr("One and only one default handler per category shoud be configured !"));
    }
    
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_PERIPHERAL_DEVICE_MANAGER);
}

void SetDevicePage::on_pushButton_QueryRfReader()
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

    m_wndMain->prepareKeyStroke();
}

#ifdef AUTO_CFG_RF_READER    
void SetDevicePage::on_pushButton_CfgRfReader()
{
    QListWidgetItem *pItem;    
    RfReaderItem     *pHandler;
    int             iLoop;
    
    unsigned int    ulAllocMsk = 0;

    unsigned char  aucAdrCnt[APP_RF_READER_MAX_NUMBER];
    
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

void SetDevicePage::on_pushButton_ResetRfReader()
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
#endif


void SetDevicePage::on_pushButton_RmvRfReader()
{
    int iLoop;
    QListWidgetItem *pItem;    
    RfReaderItem *pDevice;
    int iCount = m_pListWgtRfReader->count();

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        //pItem = m_pListWgtHandler->item(iLoop);

        pItem = m_pListWgtRfReader->takeItem(0);

        pDevice = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);
        
        m_pListWgtRfReader->removeItemWidget(pItem);

        delete pItem;       

        delete pDevice;

    }

    //m_pListWgtHandler->reset();
    m_wndMain->prepareKeyStroke();
}

void SetDevicePage::on_pushButton_SaveRfReader()
{
    int iLoop;
    int iCount = m_pListWgtRfReader->count();
    RfReaderItem *pRfReader;
    QListWidgetItem *pItem;  

    //bool bDefault;

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtRfReader->item(iLoop);

        pRfReader = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);


        if (pRfReader->getAddress() < APP_RF_READER_BEGIN_ADDRESS
            || pRfReader->getAddress() >= APP_RF_READER_END_ADDRESS)
        {
            return;
        }
    }

    {
        DB_RFID_STRU hdl[APP_RF_READER_MAX_NUMBER];
        int iMask = 0;
        for (iLoop = 0; iLoop < iCount; iLoop++)
        {
            int addr;
            int idx;
            
            memset(&hdl[iLoop],0,sizeof(DB_RFID_STRU));
            
            pItem = m_pListWgtRfReader->item(iLoop);
        
            pRfReader = (RfReaderItem *)m_pListWgtRfReader->itemWidget(pItem);

            addr = pRfReader->getAddress();

            idx = addr - APP_RF_READER_BEGIN_ADDRESS;

            iMask |= 1 << idx;

            hdl[idx].address = addr;
            hdl[idx].type    = 0;
            strncpy(hdl[idx].name,pRfReader->getItemText(RFREADER_ITEM_SN).toAscii(),APP_SN_LENGTH);
        }

        m_wndMain->updRfReader(iMask,hdl);
    }
    
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_PERIPHERAL_DEVICE_MANAGER);
    m_wndMain->prepareKeyStroke();
}


void SetDevicePage::on_pushButton_ZigbeeUpd()
{
    int iLength;
    
    QString BIN_FILE =  ":/other/shznApp.bin";


    QFileInfo info(BIN_FILE);
    if (info.exists())
    {
        iLength = info.size();
    }

    if (iLength <= MAX_FILE_SIZE)
    {
        QFile tmpfile(BIN_FILE);  
        tmpfile.open(QIODevice::ReadOnly);  
        tmpfile.read((char*)gFileMem, iLength);  
        tmpfile.close();  
        
        DispStartZigbeeUpd(iLength,NULL);
    }
    m_wndMain->prepareKeyStroke();
}

void SetDevicePage::zigbeeUpdResult(int iResult,int iPercent)
{
    if (iResult)
    {
        m_plbZigbeeUpd->setText(tr("FAIL"));
    }
    else
    {
        m_plbZigbeeUpd->setText(QString::number(iPercent));
    }
}

void SetDevicePage::addDevice(const QString& text0,const QString& text1, const QString& text2)
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

QListWidgetItem * SetDevicePage::searchDeviceByElecId(const QString& info)
{
   return searchDevice(info,HANDLER_ITEM_SN);
    
}

QListWidgetItem * SetDevicePage::searchDeviceByAddress(const QString& info)
{
    return searchDevice(info,HANDLER_ITEM_ADDRESS);
    
}

QListWidgetItem * SetDevicePage::searchDevice(const QString& info,int iIndex)
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

QString SetDevicePage::getDevice(int iDevIdx,int iInfoIdx)
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

void SetDevicePage::deviceVersion(const QString& addr,const QString& version)
{
    QListWidgetItem *pItem;    
    DeviceItem *pDevice;

    if ((pItem = searchDeviceByAddress(addr)))
    {
        pDevice = (DeviceItem *)m_pListWgtDevice->itemWidget(pItem);

        pDevice->setItemText(DEVICE_ITEM_VERSION,version);
    }
}


QListWidgetItem * SetDevicePage::searchHandler(const QString& info,int iIndex)
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


void SetDevicePage::on_checkBox_changeState(int state)
{
    (void)state;
    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }
    
    int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;
    
    int iLoop;
    int iCount = m_pListWgtHandler->count();
    HandlerItem *pHandler;
    HandlerItem *dstHandler = NULL;
    QListWidgetItem *pItem;    

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (pHandler->getCheck() == pChkBox)
        {
            dstHandler = pHandler; 
            break;
        }
    }

    if (!dstHandler)
    {
        return;
    }

    for (iLoop = 0; iLoop < iCount; iLoop++)
    {
        pItem = m_pListWgtHandler->item(iLoop);

        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        if (tmp && pHandler->getCheckState()
            && pHandler != dstHandler
            && pHandler->getType() == dstHandler->getType())
        {
            pHandler->setCheckState(false) ; 
        }
    }
    
}

void SetDevicePage::update()
{
    on_pushButton_RmvHandler();
    on_pushButton_QueryHandler();
}

void SetDevicePage::on_pushButton_DeleteHandler()
{
    int iCount = m_pListWgtHandler->count();
    HandlerItem *pHandler;
    QListWidgetItem *pItem;    

    if(iCount)
    {
        DB_HANDLER_STRU hdl;
    
        pItem = m_pListWgtHandler->currentItem();
        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(pItem);

        //strncpy(hdl.name,pHandler->getItemText(HANDLER_ITEM_SN).toAscii(),APP_SN_LENGTH);

        //qDebug() << __FUNCTION__ << pHandler->getItemText(HANDLER_ITEM_SN) << hdl.name;
        
        //m_wndMain->delHandler(hdl.name);
       m_pListWgtHandler->removeItemWidget(pItem);

       delete pItem;       

       delete pHandler;

    }
    else
    {
        return;
    }

}

QString SetDevicePage::getHandler(int iDevIdx,int iInfoIdx)
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

QListWidgetItem * SetDevicePage::searchHandlerBySN(const QString& info)
{
   return searchHandler(info,0);
    
}

QListWidgetItem * SetDevicePage::searchHandlerByAddress(const QString& info)
{
    return searchHandler(info,1);
    
}

void SetDevicePage::addHandler(int iType,const QString& text0,const QString& text1)
{
    QListWidgetItem * item = searchHandlerBySN(text0);
    HandlerItem *pHandler;

    QColor colors[] = {QColor(119,136,153),QColor(240,248,255)};

    qDebug() << text0 << text1;

 
    if (!item)
    {
        pHandler = new HandlerItem(this, text0, text1);
        pHandler->setCheckBoxStyle(m_strQss4Chk);     

        QListWidgetItem *lstItem = new QListWidgetItem();
        lstItem->setSizeHint(QSize(0, 50));

        m_pListWgtHandler->addItem(lstItem);
        m_pListWgtHandler->setItemWidget(lstItem, pHandler);
        lstItem->setBackground(colors[iType % 2]);
    }
    else
    {
        pHandler = (HandlerItem *)m_pListWgtHandler->itemWidget(item);

        pHandler->setItemText(HANDLER_ITEM_ADDRESS,text1);
        
        item->setBackground(colors[iType % 2]);
    }

    if (pHandler)
    {
       /* update other setting from database */
       DB_HANDLER_STRU * dbH = m_wndMain->getHandler(text1.toInt());
       if (dbH)
       {
           qDebug() << "dbH" << dbH->type << dbH->def;
       
           pHandler->setItemIndex(0,dbH->type);
           pHandler->setCheckState((dbH->def != 0));
       }
       qDebug() << "dbH" << dbH;
    }
}

QListWidgetItem * SetDevicePage::searchRfReader(const QString& info,int iIndex)
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

QString SetDevicePage::getRfReader(int iDevIdx,int iInfoIdx)
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

QListWidgetItem * SetDevicePage::searchRfReaderBySN(const QString& info)
{
   return searchRfReader(info,RFREADER_ITEM_SN);
    
}

QListWidgetItem * SetDevicePage::searchRfReaderByAddress(const QString& info)
{
    return searchRfReader(info,RFREADER_ITEM_ADDRESS);
    
}

void SetDevicePage::addRfReader(const QString& text0,const QString& text1)
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



