#include "ex_init_handlecfgpage.h"
#include "titlebar.h"
#include "mainwindow.h"
#include <QPainter>
#include <QScrollBar>
#include <QProcess>
#include <QListWidgetItem>
#include "cbitmapbutton.h"
#include <QRect>
#include <QtCore/QFile>
#include <QFileInfo>
#include <QDate>
#include "rpc.h"
#include "sapp.h"
#include "Interface.h"
#include "ToastDlg.h"
#include "ExtraDisplay.h"

Ex_Init_HandleCfgpage::Ex_Init_HandleCfgpage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

    this->hideTitleBar();

}

void Ex_Init_HandleCfgpage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_Init_HandleCfgpage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Connecting Device");

    setTitles(stringList);

}

void Ex_Init_HandleCfgpage::buildTranslation()
{
    m_plbHandlerSN->setText(tr("SN"));
    m_plbHandlerAdr->setText(tr("Address"));
    m_plbHandlerDef->setText(tr("Default"));
    m_plbHandlerType->setText(tr("Type"));
    m_plbHandlerOper->setText(tr("Operate"));

    m_pExFinishBtn->setText(tr("Finished")); //ex_dcj
    m_pbackBtn->setText(tr("Back"));

    m_pBtnQueryHandler->setText(tr("Query"));
    m_pBtnCfgHandler->setText(tr("Config."));
    m_pBtnResetHandler->setText(tr("Reset"));
//    m_pBtnRmvHandler->setText(tr("Clear"));
    m_pBtnSaveHandler->setText(tr("Save"));

    m_pTblWidget->setTabText(0,tr("Dispenser"));

}

void Ex_Init_HandleCfgpage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void Ex_Init_HandleCfgpage::setBackColor()
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

void Ex_Init_HandleCfgpage::finishSave()
{
    ToastDlg::makeToast(tr("Successfully saved"));
}


void Ex_Init_HandleCfgpage::on_CmbIndexChange_trx_type(int index)
{

    int iIdx = m_pcombTrxType->currentIndex();

    if (APP_TRX_ZIGBEE == iIdx
        && !(gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_ZIGBEE)))
    {
        m_pcombTrxType->setCurrentIndex(APP_TRX_CAN);
    }

}

void Ex_Init_HandleCfgpage::on_pushButton_FinishBtn()
{
    QDate curDate = QDate::currentDate();
    QString strDate = curDate.toString("yyyy-MM-dd");
    ex_gGlobalParam.Ex_System_Msg.Ex_InsDate = strDate;
    MainSaveInstallMsg(gGlobalParam.iMachineType);

    ex_gGlobalParam.Ex_Default = 1;
    MainSaveDefaultState(gGlobalParam.iMachineType);
    MainUpdateGlobalParam();

    m_wndMain->restart();
}

void Ex_Init_HandleCfgpage::on_pushButton_BackBtn()
{
    emit handlercfgSwitchBtnClicked(0);
    m_wndMain->prepareKeyStroke();
}

void Ex_Init_HandleCfgpage::initUi()
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

    QGridLayout  *pcfg2Top =  new QGridLayout;
    QHBoxLayout  *pcfg2HBox =  new QHBoxLayout;
    QGridLayout  *pcfg2Grid = new QGridLayout;
    QHBoxLayout  *pcfg2TilHBox =  new QHBoxLayout;
    QHBoxLayout* pcfg2ExHBox = new QHBoxLayout;

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

    m_pExFinishBtn = new QPushButton; //
    m_pbackBtn = new QPushButton;
    m_pExFinishBtn->setMaximumWidth(200);
    m_pExFinishBtn->setMinimumWidth(180);
    m_pbackBtn->setMaximumWidth(200);
    m_pbackBtn->setMinimumWidth(180);

    pcfg2ExHBox->addStretch(2);
    pcfg2ExHBox->addWidget(m_pbackBtn);
    pcfg2ExHBox->addStretch(1);
    pcfg2ExHBox->addWidget(m_pExFinishBtn);
    pcfg2ExHBox->addStretch(2);

    pcfg2Grid->addLayout(pcfg2Top,0,0);
    pcfg2Grid->addLayout(pcfg2HBox,1,0,1,2);
    pcfg2Grid->addLayout(pcfg2ExHBox, 2, 0, 1, 2);

    widget->setLayout(pcfg2Grid);

    QIcon icon2(":/pic/unselected.png");
    m_pTblWidget->addTab(widget, icon2, tr("HANDLER"));


    layout->addWidget(m_pTblWidget);

    m_widgetLayout->setLayout(layout);

    connect(m_pBtnQueryHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_QueryHandler()));
    connect(m_pBtnCfgHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_CfgHandler()));
    connect(m_pBtnResetHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_ResetHandler()));
//    connect(m_pBtnRmvHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_RmvHandler()));
    connect(m_pBtnSaveHandler,SIGNAL(clicked()),this,SLOT(on_pushButton_SaveHandler()));
    connect(m_pExFinishBtn, SIGNAL(clicked()), this, SLOT(on_pushButton_FinishBtn()));
    connect(m_pbackBtn, SIGNAL(clicked()), this, SLOT(on_pushButton_BackBtn()));
}

void Ex_Init_HandleCfgpage::on_btn_clicked(int index)
{
    (void)index;
}


void Ex_Init_HandleCfgpage::on_pushButton_QueryHandler()
{
    char buf[64];

    on_pushButton_RmvHandler();  //20190108

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


void Ex_Init_HandleCfgpage::cfgHandlerRsp()
{
    ToastDlg::makeToast(tr("Be sure to save the handsets' configuration when collected all responses!"));
}


void Ex_Init_HandleCfgpage::on_pushButton_CfgHandler()
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

void Ex_Init_HandleCfgpage::on_pushButton_ResetHandler()
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


void Ex_Init_HandleCfgpage::on_pushButton_RmvHandler()
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
    m_wndMain->prepareKeyStroke();
    //m_pListWgtHandler->reset();

}

void Ex_Init_HandleCfgpage::on_pushButton_SaveHandler()
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
        //QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("Handler"), tr("One and only one default handler shoud be configured ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        QMessageBox::about(NULL, tr("About"), tr("One and only one default handler per category shoud be configured !"));
    }

}


QListWidgetItem * Ex_Init_HandleCfgpage::searchHandler(const QString& info,int iIndex)
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


void Ex_Init_HandleCfgpage::on_checkBox_changeState(int state)
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

void Ex_Init_HandleCfgpage::update()
{
    on_pushButton_RmvHandler();
    on_pushButton_QueryHandler();
}

void Ex_Init_HandleCfgpage::on_pushButton_DeleteHandler()
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

QString Ex_Init_HandleCfgpage::getHandler(int iDevIdx,int iInfoIdx)
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

QListWidgetItem * Ex_Init_HandleCfgpage::searchHandlerBySN(const QString& info)
{
   return searchHandler(info,0);

}

QListWidgetItem * Ex_Init_HandleCfgpage::searchHandlerByAddress(const QString& info)
{
    return searchHandler(info,1);

}

void Ex_Init_HandleCfgpage::addHandler(int iType,const QString& text0,const QString& text1)
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












