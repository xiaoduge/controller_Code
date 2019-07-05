#include "ex_init_networkpage.h"
#include "titlebar.h"
#include "mainwindow.h"

#include <QPainter>
#include <QScrollBar>
#include <QListWidgetItem>
#include <QRect>
#include <QProcess>

#include "cbitmapbutton.h"
#include "dlineedit.h"

Ex_Init_Networkpage::Ex_Init_Networkpage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;
    creatTitle();
    initUi();
    buildTranslation();
    this->hideTitleBar();

}

void Ex_Init_Networkpage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_Init_Networkpage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Connectivity");
    setTitles(stringList);

}

void Ex_Init_Networkpage::buildTranslation()
{
    int iLoop;
    m_astrNetName[0] = tr("CAN");
    m_astrNetName[1] = tr("Zigbee");
    m_astrNetName[2] = tr("WIFI");

    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
        m_laName[iLoop]->setText(m_astrNetName[iLoop]);
    }
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
    m_pExBackBtn->setText(tr("Back"));
    m_pExNextBtn->setText(tr("Next"));

#ifdef D_HTTPWORK
    m_pSSIDLab->setText(tr("SSID:"));
    m_pAddSSIDBtn->setText(tr("Add"));
    m_pRefreshWifiBtn->setText(tr("Refresh"));
    m_pAddCheckBox->setText(tr("Add network"));
#endif
}

void Ex_Init_Networkpage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_Init_Networkpage::setBackColor()
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

void Ex_Init_Networkpage::initUi()
{
    int iLoop;

    m_strQss4Chk = m_wndMain->getQss4Chk();

    setBackColor();

    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
        m_pBackWidget[iLoop] = new QWidget(m_widget);

        QPalette pal(m_pBackWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::gray);

        m_pBackWidget[iLoop]->setAutoFillBackground(true);
        m_pBackWidget[iLoop]->setPalette(pal);

        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 80 + 70 * (iLoop - 1) , 530 ,60));

        m_laName[iLoop]      = new QLabel(m_pBackWidget[iLoop]);
        m_laName[iLoop]->setPixmap(NULL);
        m_laName[iLoop]->setGeometry(QRect(25, 30 , 100 , 20));
        m_laName[iLoop]->show();

        m_chkSwitchs[iLoop] = new QCheckBox(m_pBackWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(480 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(m_strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        if (m_iNetworkMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));

        //2019.3.14 add
        if(DISPLAY_NETWORK_CAN == iLoop)
        {
            m_pBackWidget[iLoop]->hide();
        }
    }

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,NETWORKPAGE_BTN_SAVE);

    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);

    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    //700,560,
    m_pBtnSave->setGeometry(350,400,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());

    m_pBtnSave->setStyleSheet("background-color:transparent");

    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

//    m_pBtnSave->show();
    m_pBtnSave->hide();

    m_pExBackBtn = new QPushButton(m_widget);
    m_pExNextBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_m_pExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_m_pExBackBtn_clicked()));

    m_pExBackBtn->move(200, 420);
    m_pExNextBtn->move(500, 420);

#ifdef D_HTTPWORK
    m_pWifiConfigWidget = new QWidget(m_widget);
    QPalette pal(m_pWifiConfigWidget->palette());
    pal.setColor(QPalette::Background, Qt::gray);
    m_pWifiConfigWidget->setAutoFillBackground(true);
    m_pWifiConfigWidget->setPalette(pal);
    m_pWifiConfigWidget->setGeometry(QRect(124 , 215 , 530 ,242));

    m_pWifiMsgListWidget = new QListWidget(m_pWifiConfigWidget);
    m_pWifiMsgListWidget->setGeometry(5, 5, 520, 200);

    m_pRefreshWifiBtn = new QPushButton(m_pWifiConfigWidget);
    m_pRefreshWifiBtn->setGeometry(210, 208, 100, 30);

    m_pAddCheckBox = new QCheckBox(m_pWifiConfigWidget);
    m_pAddCheckBox->setGeometry(390, 208, 120, 30);
    connect(m_pAddCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_addCheckBox_stateChanged(int)));

    QString strQss4Chk = m_wndMain->getQss4Chk();
    m_pAddCheckBox->setStyleSheet(strQss4Chk);

    m_pWifiSSIDAddWidget = new QWidget(m_widget);
    pal = m_pWifiSSIDAddWidget->palette();
    pal.setColor(QPalette::Background, Qt::gray);
    m_pWifiSSIDAddWidget->setAutoFillBackground(true);
    m_pWifiSSIDAddWidget->setPalette(pal);
    m_pWifiSSIDAddWidget->setGeometry(QRect(124 , 465, 530 ,40));

    m_pSSIDLab = new QLabel(m_pWifiSSIDAddWidget);
    m_pSSIDLab->setGeometry(QRect(5, 5 , 50 , 30));

    m_pSSIDEdit = new DLineEdit(m_pWifiSSIDAddWidget);
    m_pSSIDEdit->setGeometry(QRect(60, 5 , 190 , 30));

    m_pAddSSIDBtn = new QPushButton(m_pWifiSSIDAddWidget);
    m_pAddSSIDBtn->setGeometry(400, 5, 100, 30);

    connect(m_pAddSSIDBtn, SIGNAL(clicked()), this, SLOT(on_addSSIDBtn_clicked()));
    connect(m_pRefreshWifiBtn, SIGNAL(clicked()), this, SLOT(on_wifiRefreshBtn_clicked()));

    connect(m_pWifiMsgListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(on_wifiListWidget_itemClicked(QListWidgetItem*)));

    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(on_refreshWifiMsg()));

    if(!(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState()))
    {
        m_pWifiConfigWidget->hide();
        m_pWifiSSIDAddWidget->hide();
        m_pExBackBtn->move(200, 420);
        m_pExNextBtn->move(500, 420);
    }

    if(!(Qt::Checked == m_pAddCheckBox->checkState()))
    {
        m_pWifiSSIDAddWidget->hide();
    }

#endif

}

void Ex_Init_Networkpage::save()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    }

}

void Ex_Init_Networkpage::on_btn_clicked(int index)
{

   switch(index)
   {
   case NETWORKPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }

   m_wndMain->prepareKeyStroke();
}

void Ex_Init_Networkpage::on_checkBox_changeState(int state)
{
    (void)state;
    int iLoop;

    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }

    //int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;


    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
       if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
       {
           m_iNetworkMask |= 1 << iLoop;
       }
       else
       {
           m_iNetworkMask &= ~(1 << iLoop);
       }
    }

#ifdef D_HTTPWORK
    if(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState())
    {
        m_pWifiConfigWidget->show();
        m_pExBackBtn->move(200, 510);
        m_pExNextBtn->move(500, 510);
    }
    else
    {
        m_pWifiConfigWidget->hide();
        m_pWifiSSIDAddWidget->hide();
        m_pExBackBtn->move(200, 420);
        m_pExNextBtn->move(500, 420);
    }
#endif

}

void Ex_Init_Networkpage::on_m_pExNextBtn_clicked()
{
    save(); //next clicked save

    emit networkSwitchBtnClicked(1);

}

void Ex_Init_Networkpage::on_m_pExBackBtn_clicked()
{
    emit networkSwitchBtnClicked(0);
    m_wndMain->prepareKeyStroke();
}

void Ex_Init_Networkpage::leaveSubPage()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    }
    CSubPage::leaveSubPage();
}

void Ex_Init_Networkpage::update()
{
#ifdef D_HTTPWORK
    m_pAddCheckBox->setChecked(false);
#endif

    if(gGlobalParam.MiscParam.iNetworkMask & 1 << DISPLAY_NETWORK_ZIGBEE)
    {
        m_chkSwitchs[DISPLAY_NETWORK_ZIGBEE]->setChecked(true);
    }
    else
    {
        m_chkSwitchs[DISPLAY_NETWORK_ZIGBEE]->setChecked(false);
    }

    if(gGlobalParam.MiscParam.iNetworkMask & 1 << DISPLAY_NETWORK_WIFI)
    {
        m_chkSwitchs[DISPLAY_NETWORK_WIFI]->setChecked(true);
#ifdef D_HTTPWORK
        m_pWifiConfigWidget->show();
        m_pExBackBtn->move(200, 510);
        m_pExNextBtn->move(500, 510);
#endif
    }
    else
    {
        m_chkSwitchs[DISPLAY_NETWORK_WIFI]->setChecked(false);
#ifdef D_HTTPWORK
        m_pWifiConfigWidget->hide();
        m_pWifiSSIDAddWidget->hide();
        m_pExBackBtn->move(200, 420);
        m_pExNextBtn->move(500, 420);
#endif
    }
}

#ifdef D_HTTPWORK
void Ex_Init_Networkpage::on_addSSIDBtn_clicked()
{
    QString strSSID = QString("ESSID:\"%1\"").arg(m_pSSIDEdit->text());
    m_pWifiMsgListWidget->addItem(strSSID);
}

void Ex_Init_Networkpage::on_wifiRefreshBtn_clicked()
{
    m_pProcess->start("iwlist wlan0 scanning");
    m_pWifiMsgListWidget->clear();
    m_pWifiMsgListWidget->addItem("Searching");
}

void Ex_Init_Networkpage::on_refreshWifiMsg()
{
    QString strAll = m_pProcess->readAllStandardOutput();

    QRegExp ssidRx("ESSID:\"([^\"]*)\"");
    QStringList ssidList;
    int pos = 0;
    while((pos = ssidRx.indexIn(strAll, pos)) != -1)
    {
        QString strSSID = ssidRx.cap(0);
        ssidList << strSSID;
        pos += ssidRx.matchedLength();
    }

    m_pWifiMsgListWidget->clear();
    QSet<QString> ssidSet = ssidList.toSet();
    QSet<QString>::const_iterator it;
    for(it = ssidSet.begin(); it != ssidSet.end(); ++it)
    {
        m_pWifiMsgListWidget->addItem(*it);
    }
}

void Ex_Init_Networkpage::on_wifiListWidget_itemClicked(QListWidgetItem *item)
{
    //
    QString strName = item->text().remove("ESSID:").remove("\"");
    m_wndMain->showWifiConfigDlg(strName);
}

void Ex_Init_Networkpage::on_addCheckBox_stateChanged(int state)
{
    if(state == Qt::Checked)
    {
        m_pSSIDEdit->clear();
        m_pWifiSSIDAddWidget->show();
    }
    else
    {
        m_pWifiSSIDAddWidget->hide();
    }
}
#endif



