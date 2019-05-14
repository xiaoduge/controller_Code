#include "networkpage.h"

#include "titlebar.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "cbitmapbutton.h"

#include <QRect>
#include <QProcess>



NetworkPage::NetworkPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;

    creatTitle();

    initUi();

    buildTranslation();

}

void NetworkPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void NetworkPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Connectivity");

    setTitles(stringList);

}

void NetworkPage::buildTranslation()
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

#ifdef D_HTTPWORK
    m_pSSIDLab->setText(tr("SSID:"));
    m_pPSKLab->setText(tr("PSK:"));
    m_pWifiConfigBtn->setText(tr("Config"));
    m_pRefreshWifiBtn->setText(tr("Refresh"));
#endif
}

void NetworkPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void NetworkPage::setBackColor()
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

void NetworkPage::initUi()
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

//        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 160 + 70 * iLoop , 530 ,60));
        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 100 + 80 * (iLoop - 1) , 530 ,60));

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

    //add for wifi config
#ifdef D_HTTPWORK
    m_pWifiConfigWidget = new QWidget(m_widget);
    QPalette pal(m_pWifiConfigWidget->palette());
    pal.setColor(QPalette::Background, Qt::gray);
    m_pWifiConfigWidget->setAutoFillBackground(true);
    m_pWifiConfigWidget->setPalette(pal);
    m_pWifiConfigWidget->setGeometry(QRect(124 , 100 + 80 * 1 + 65 , 530 ,300));

    m_pSSIDLab = new QLabel(m_pWifiConfigWidget);
    m_pSSIDLab->setGeometry(QRect(5, 5 , 50 , 30));

    m_pSSIDEdit = new QLineEdit(m_pWifiConfigWidget);
    m_pSSIDEdit->setGeometry(QRect(60, 5 , 190 , 30));

    m_pPSKLab = new QLabel(m_pWifiConfigWidget);
    m_pPSKLab->setGeometry(QRect(255, 5 , 50 , 30));

    m_pPSKEdit = new QLineEdit(m_pWifiConfigWidget);
    m_pPSKEdit->setGeometry(QRect(310, 5, 190 , 30));

    m_pWifiConfigBtn = new QPushButton(m_pWifiConfigWidget);
    m_pWifiConfigBtn->setGeometry(320, 40, 100, 30);
    m_pRefreshWifiBtn = new QPushButton(m_pWifiConfigWidget);
    m_pRefreshWifiBtn->setGeometry(70, 40, 100, 30);
    m_pWifiMsgListWidget = new QListWidget(m_pWifiConfigWidget);
    m_pWifiMsgListWidget->setGeometry(5, 72, 520, 200);

    connect(m_pWifiConfigBtn, SIGNAL(clicked()), this, SLOT(on_wifiConfigBtn_clicked()));
    connect(m_pRefreshWifiBtn, SIGNAL(clicked()), this, SLOT(on_wifiRefreshBtn_clicked()));
    connect(m_pWifiMsgListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(on_wifiListWidget_currentRowChanged(int)));

    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(on_refreshWifiMsg()));

    if(!(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState()))
    {
        m_pWifiConfigWidget->hide();
    }

#endif
    //end
    
    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,NETWORKPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}

void NetworkPage::save()
{
    
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
       
    }
    
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_NETWORK);
    
    show(false);
    m_parent->show(true);

}

void NetworkPage::on_btn_clicked(int index)
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

void NetworkPage::on_checkBox_changeState(int state)
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
    }
    else
    {
        m_pWifiConfigWidget->hide();
    }
#endif
}

#ifdef D_HTTPWORK
void NetworkPage::on_wifiConfigBtn_clicked()
{
    if(m_pSSIDEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, tr("Waring"), tr("Invalid SSID"), QMessageBox::Ok);
        return;
    }

    QString strFileName(WIFICONFIG_FILE);
    if(strFileName.isEmpty())
    {
        return;
    }

    QFile sourceFile(strFileName);

    if(!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug() << "Open wifi config file failed: 1";
        return;
    }

    QString strAll = QString("# WPA-PSK/TKIP\n\nctrl_interface=/var/run/wpa_supplicant\n\nnetwork={\n\tssid=\"%1\"\n\tscan_ssid=1\n\tkey_mgmt=WPA-PSK WPA-EAP\n\tproto=RSN\n\tpairwise=TKIP CCMP\n\tgroup=TKIP CCMP\n\tpsk=\"%2\"\n        auth_alg=OPEN\n\tpriority=1\n}\n\n")
            .arg(m_pSSIDEdit->text())
            .arg(m_pPSKEdit->text());

    QByteArray array = strAll.toLatin1();
    char *data = array.data();

    sourceFile.write(data);

    if(sourceFile.isOpen())
    {
        sourceFile.close();
    }
}

void NetworkPage::on_wifiRefreshBtn_clicked()
{
    m_pProcess->start("iwlist wlan0 scanning");
    m_pWifiMsgListWidget->clear();
    m_pWifiMsgListWidget->addItem("Searching");
}

void NetworkPage::on_refreshWifiMsg()
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

void NetworkPage::on_wifiListWidget_currentRowChanged(int currentRow)
{
    if(currentRow < 0)
    {
        return;
    }
    QString strSSID = m_pWifiMsgListWidget->item(currentRow)->text().remove("ESSID:").remove("\"");
    m_pSSIDEdit->setText(strSSID);
}
#endif

void NetworkPage::leaveSubPage()
{    
    CSubPage::leaveSubPage();
}


