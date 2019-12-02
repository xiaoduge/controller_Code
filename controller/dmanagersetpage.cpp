#include "dmanagersetpage.h"
#include "mainwindow.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>
#include <time.h>
#include <QButtonGroup>
#include <QFile>
#include "Display.h"
#include "exconfig.h"
#include "dhintdialog.h"
#include <QRadioButton>
#include "dlineedit.h"
#include <QButtonGroup>
#include <QCalendarWidget>
#include <QSlider>

DManagerSetPage::DManagerSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

DManagerSetPage::~DManagerSetPage()
{
	switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
        if(gAdditionalCfgParam.machineInfo.iMachineFlow == 500)
        {
        	if(m_pageWidget[MANAGER_PAGE_CALIBRATION])
        	{
        		m_pageWidget[MANAGER_PAGE_CALIBRATION]->deleteLater();
				m_pageWidget[MANAGER_PAGE_CALIBRATION] = NULL;
        	}
			if(m_pageWidget[MANAGER_PAGE_ADDSETTINGS])
			{
				m_pageWidget[MANAGER_PAGE_ADDSETTINGS]->deleteLater();
				m_pageWidget[MANAGER_PAGE_ADDSETTINGS] = NULL;
			}
        }
        break;
    default:
        break;
    }
}

void DManagerSetPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DManagerSetPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Manager Config");
    setTitles(stringList);

}

void DManagerSetPage::buildTranslation()
{
    m_tabWidget->setTabText(0, tr("Time & Date"));
    m_tabWidget->setTabText(1, tr("Calibration"));
    m_tabWidget->setTabText(2, tr("Audio"));
    m_tabWidget->setTabText(3, tr("LCD"));
    m_tabWidget->setTabText(4, tr("Additional Settings"));

    //Time
    int iLoop;

    m_astrDateName[0] = tr("Select Date");
    m_astrDateName[1] = tr("Select Time");

    for( iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        lbName[iLoop]->setText(m_astrDateName[iLoop]);

    }

    lbTitName->setText(tr("Date & Time"));

    m_pBtns[TIMEPAGE_BTN_CANCEL]->setText(tr("Cancel"));
    m_pBtns[TIMEPAGE_BTN_OK]->setText(tr("OK"));

    //Dispense Rate
    m_pCaliS1Label->setText(tr("Disp. Rate"));
    m_pCaliBtn->setText(tr("Save"));

    //Audio
    m_strSounds[0] = tr("Touch-tone");
//    m_strSounds[1] = tr("Audio Alerts");
    m_strSounds[1] = tr("Audio Alarms");

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_lblNames[iLoop]->setText(m_strSounds[iLoop]);
    }
    m_pAudioBtnSave->setText(tr("Save"));

    //LCD
    m_DispNames[0] = tr("Brightness");
    m_DispNames[1] = tr("Energy-saving");
    m_sleepLabel -> setText(tr("SleepTime"));

    for( iLoop = 0 ; iLoop < 2 ; iLoop++)
    {
        laName[iLoop]->setText(m_DispNames[iLoop]);
    }
    m_pLcdBtnSave->setText(tr("Save"));


    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    default:
        m_pAdditionalLb[HPCIR_SETTING]->setText(tr("HP Recir."));
        m_pAddBtnSave->setText(tr("Save"));
        break;
    }

}

void DManagerSetPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}


void DManagerSetPage::setBackColor()
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



void DManagerSetPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;
    //add page
    initTimePage();
    initCalibrationPage();
    initAudioPage();
    initLcdPage();

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    default:
        initAdditionalSettingsPage();
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
        if(gAdditionalCfgParam.machineInfo.iMachineFlow == 500)
        {
            m_tabWidget->removeTab(1);
			m_tabWidget->removeTab(4);
        }
        break;
    default:
        break;
    }

    mainLayout->addWidget(m_tabWidget, 0, 0);
    m_mainWidget->setLayout(mainLayout);

    QFile qss(":/app/tabWidget.qss");
    qss.open(QFile::ReadOnly);
    QString tabWidgetqss = QLatin1String (qss.readAll());
    qss.close();

    m_tabWidget->setStyleSheet(tabWidgetqss);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);

}


void DManagerSetPage::update()
{
    //Time
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    m_pBtns[TIMEPAGE_BTN_DATE_SET]->setText(sysDateTime.toString("yyyy-MM-dd"));
    m_pBtns[TIMEPAGE_BTN_TIME_SET]->setText(sysDateTime.toString("hh:mm:ss"));

    //rate
    if(gGlobalParam.Caliparam.pc[m_caliId].fk > 100)
    {
        m_pCaliS1LineEdit->setText(QString::number(gCaliParam.pc[m_caliId].fk));
    }
    else
    {
        m_pCaliS1LineEdit->setText(QString::number(gCaliParam.pc[m_caliId].fk,'f',3));
    }

    //
    m_iEnergySave = gGlobalParam.MiscParam.iEnerySave;
    if(m_iEnergySave)
    {
        m_pCheckEnergySave->setCheckState(Qt::Checked);
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_pCheckEnergySave->setCheckState(Qt::Unchecked);
        m_pLcdBackWidget[2]->hide();
    }

    if((gGlobalParam.iMachineType != MACHINE_ADAPT)
        && (gGlobalParam.iMachineType != MACHINE_PURIST))
    {
        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            m_pAdditionalCheck[HPCIR_SETTING]->setChecked(true);
        }
        else
        {
            m_pAdditionalCheck[HPCIR_SETTING]->setChecked(false);
        }
    }

    m_iSleepTime = gAdditionalCfgParam.additionalParam.iScreenSleepTime;
    m_comboBox->setCurrentIndex(m_iSleepTime - 1);

    for(int iLoop = 0; iLoop < DISPLAY_SOUND_NUM; ++iLoop)
    {
        if (m_iSoundMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }
    }

}

void DManagerSetPage::show(bool bShow)
{
    if (bShow)
    {
        m_widget->show() ;

        if (m_wndMain ) m_wndMain->setCurrentPage(this);

        m_secondTimer = startTimer(1000);
    }
    else
    {
        m_widget->hide();
        if(0 != m_secondTimer) killTimer(m_secondTimer);
    }
}

void DManagerSetPage::timerEvent(QTimerEvent *event)
{
    if(m_secondTimer == event->timerId())
    {
        if(0 == m_tabWidget->currentIndex())
        {
            QTime curTime;
            curTime = QTime::currentTime();
            m_pBtns[TIMEPAGE_BTN_TIME_SET]->setText(curTime.toString("hh:mm:ss"));
        }
    }
}

void DManagerSetPage::on_timeDateSet_clicked()
{
    ShowWidget[DATE_SET]->hide();
    ShowWidget[TIME_SET]->hide();

    CalS->show();
    m_pBtns[TIMEPAGE_BTN_CANCEL]->show();
    m_pBtns[TIMEPAGE_BTN_OK]->show();

    m_wndMain->prepareKeyStroke();

}

void DManagerSetPage::on_timeTimeSet_clicked()
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    int hour = sysDateTime.time().hour();
    int min  = sysDateTime.time().minute();
    int sec  = sysDateTime.time().second();

    TimeHCbox->show();
    TimeHCbox->setCurrentIndex(hour);

    TimeMCbox->show();
    TimeMCbox->setCurrentIndex(min);

    TimeSCbox->show();
    TimeSCbox->setCurrentIndex(sec);

    m_pBtns[TIMEPAGE_BTN_OK]->show();
    m_pBtns[TIMEPAGE_BTN_CANCEL]->show();

    lbTitPic->hide();
    lbTitName->hide();

    ShowWidget[DATE_SET]->hide();
    ShowWidget[TIME_SET]->hide();

    m_wndMain->prepareKeyStroke();

}

void DManagerSetPage::on_timeOkBtn_clicked()
{

    if(CalS->isVisible())
    {
        QDate date  = CalS->selectedDate();
        QString tmp = date.toString("yyyy-MM-dd");
        m_pBtns[TIMEPAGE_BTN_DATE_SET]->setText(tmp);

        CalS->hide();
        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    else if(TimeHCbox->isVisible())
    {
        QString TimeStr = TimeHCbox->currentText();
        TimeStr.append(":");
        TimeStr.append(TimeMCbox->currentText());
        TimeStr.append(":");
        TimeStr.append(TimeSCbox->currentText());
        m_pBtns[TIME_SET]->setText(TimeStr);

        //
        TimeHCbox->hide();
        TimeMCbox->hide();
        TimeSCbox->hide();

        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

        lbTitPic->show();
        lbTitName->show();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }

    changeTime();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_TIME);

    m_wndMain->prepareKeyStroke();

}

void DManagerSetPage::on_timeCancelBtn_clicked()
{
    if(CalS->isVisible())
    {
        CalS->hide();
        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    else if(TimeHCbox->isVisible())
    {
        TimeHCbox->hide();
        TimeMCbox->hide();
        TimeSCbox->hide();

        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

        lbTitPic->show();
        lbTitName->show();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    m_wndMain->prepareKeyStroke();
}

void DManagerSetPage::on_caliSaveBtn_clicked()
{
    QMap<int, DISP_PARAM_CALI_ITEM_STRU> cMap;
    DISP_PARAM_CALI_ITEM_STRU values;

    float fTemp = m_pCaliS1LineEdit->text().toFloat();
    values.fk = fTemp;
    values.fc = 1;
    values.fv = 1;

    cMap.insert(m_caliId, values);

    MainSaveCalibrateParam(gGlobalParam.iMachineType, cMap);

    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_PARAMETER_CALIBRATE);

    m_wndMain->prepareKeyStroke();
    DHintDialog::getInstance(tr("Successfully saved"));
}

void DManagerSetPage::on_checkBox_changeState(int state)
{
    int iLoop;

    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
       if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
       {
           m_iSoundMask |= 1 << iLoop;
       }
       else
       {
           m_iSoundMask &= ~(1 << iLoop);
       }
    }

}

void DManagerSetPage::on_audioBtnSavebtn_clicked()
{
    if (m_iSoundMask != gGlobalParam.MiscParam.iSoundMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iSoundMask = m_iSoundMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_SOUND);

    }
    m_wndMain->prepareKeyStroke();

    DHintDialog::getInstance(tr("Successfully saved"));
}

void DManagerSetPage::on_LcdSaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

    MiscParam.iBrightness = m_iBrightness;
    MiscParam.iEnerySave  = m_iEnergySave;
    MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

    gAdditionalCfgParam.additionalParam.iScreenSleepTime = m_iSleepTime;
    MainSaveExConfigParam(gGlobalParam.iMachineType);
    MainRetriveExConfigParam(gGlobalParam.iMachineType);

    MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_DISPLAY);

    m_wndMain->prepareKeyStroke();
    DHintDialog::getInstance(tr("Successfully saved"));
}

void DManagerSetPage::on_CheckEnergySave_stateChanged(int state)
{
    if(Qt::Checked == state)
    {
        m_iEnergySave = 1;
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_iEnergySave = 0;
        m_pLcdBackWidget[2]->hide();
    }
    m_wndMain->prepareKeyStroke();
}

void DManagerSetPage::on_comboBox_currentIndexChanged(int index)
{
    m_iSleepTime = index + 1;
}

void DManagerSetPage::setValue(int value)
{
    if(0 == gAdditionalCfgParam.productInfo.iCompany)
    {
        if (value < 50)
        {
            return;
        }
    }
    else
    {
        if (value < 20)
        {
            return;
        }
    }

    m_iBrightness = value;

    Write_sys_int(PWMLCD_FILE,m_iBrightness);
}

void DManagerSetPage::on_AdditionalBtnSave_clicked()
{
    if((gGlobalParam.iMachineType == MACHINE_PURIST)
       && (gGlobalParam.iMachineType == MACHINE_ADAPT))
    {
        return;
    }

    DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;
    if(Qt::Checked == m_pAdditionalCheck[HPCIR_SETTING]->checkState())
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_HP_Water_Cir;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_HP_Water_Cir);
    }
    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();

    m_wndMain->prepareKeyStroke();

    DHintDialog::getInstance(tr("Successfully saved"));
}

void DManagerSetPage::on_HPCircheckBox_changeState(int state)
{
}

void DManagerSetPage::initTimePage()
{
    m_pageWidget[MANAGER_PAGE_TIME] = new QWidget;

    //createHeads
    QPixmap PixTit;
    PixTit.load(":/pic/time.png");

    lbTitPic = new QLabel(m_pageWidget[MANAGER_PAGE_TIME]);
    lbTitPic->setGeometry(QRect(150, 100 , 28 , 28));
    lbTitPic->setPixmap(PixTit);

    lbTitName = new QLabel(m_pageWidget[MANAGER_PAGE_TIME]);
    lbTitName->setGeometry(QRect(210, 105 , 200 , 28));
    lbTitName->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");

    //Create_Cal_Ok
    m_pBtns[TIMEPAGE_BTN_CANCEL] = new QPushButton(m_pageWidget[MANAGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setGeometry(QRect(230, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_CANCEL], SIGNAL(clicked()), this, SLOT(on_timeCancelBtn_clicked()));

    m_pBtns[TIMEPAGE_BTN_OK]     = new QPushButton(m_pageWidget[MANAGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_OK]->setGeometry(QRect(430, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_OK]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_OK], SIGNAL(clicked()), this, SLOT(on_timeOkBtn_clicked()));
    //Create_Date_Time
    int iLoop;
    int tmp;

    for(iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        ShowWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_TIME]);

        QPalette pal(ShowWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        ShowWidget[iLoop]->setAutoFillBackground(true);
        ShowWidget[iLoop]->setPalette(pal);

        ShowWidget[iLoop]->setGeometry(QRect(134 , 160 + 90 * iLoop , 530 ,60));

        lbName[iLoop] = new QLabel(ShowWidget[iLoop]);
        lbName[iLoop]->setGeometry(QRect(25, 20 , 120 , 20));
        lbName[iLoop]->setAlignment(Qt::AlignLeft);
        m_pBtns[iLoop] = new QPushButton(ShowWidget[iLoop]);
        m_pBtns[iLoop]->setGeometry(QRect(300 ,20 ,200,20));
        m_pBtns[iLoop]->setStyleSheet("background-color:transparent");

        if(DATE_SET == iLoop)
        {
            connect(m_pBtns[iLoop], SIGNAL(clicked()), this, SLOT(on_timeDateSet_clicked()));
            CalS = new QCalendarWidget(m_pageWidget[MANAGER_PAGE_TIME]);

            switch(gGlobalParam.MiscParam.iLan)
            {
            case APP_LAN_CHN:
                CalS->setLocale(QLocale::Chinese);
                break;
            case APP_LAN_SPA:
                CalS->setLocale(QLocale::Spanish);
                break;
            case APP_LAN_FRE:
                CalS->setLocale(QLocale::French);
                break;
            case APP_LAN_ITA:
                CalS->setLocale(QLocale::Italian);
                break;
            case APP_LAN_SKR:
                CalS->setLocale(QLocale::Korean);
                break;
            case APP_LAN_GER:
                CalS->setLocale(QLocale::German);
                break;
            case APP_LAN_RUS:
                CalS->setLocale(QLocale::Russian);
                break;
            case APP_LAN_POR:
                CalS->setLocale(QLocale::Portuguese);
                break;
            default:
                CalS->setLocale(QLocale::English);
                break;
            }

            CalS->setGeometry(QRect(130,70,500,300));
            CalS->hide();

        }
        else if(TIME_SET == iLoop)
        {
            connect(m_pBtns[iLoop], SIGNAL(clicked()), this, SLOT(on_timeTimeSet_clicked()));

            TimeHCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeHCbox->setGeometry(230,200,80,30);
            TimeHCbox->hide();

            for(tmp = 0 ; tmp < 24 ; tmp++ )
            {
                TimeHCbox->addItem(QString::number(tmp,10));
            }

            TimeMCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeMCbox->setGeometry(330,200,80,30);
            TimeMCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeMCbox->addItem(QString::number(tmp,10));
            }

            TimeSCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeSCbox->setGeometry(430,200,80,30);
            TimeSCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeSCbox->addItem(QString::number(tmp,10));
            }

        }

    }
    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_TIME], icon1, tr("Time & Date"));
}

void DManagerSetPage::initCalibrationPage()
{
    m_caliId = DISP_PC_COFF_S1;
    setBackColor();
    m_pageWidget[MANAGER_PAGE_CALIBRATION] = new QWidget;

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    m_pCaliS1Widget = new QWidget(m_pageWidget[MANAGER_PAGE_CALIBRATION]);

    QPalette pal(m_pCaliS1Widget->palette());
    pal.setColor(QPalette::Background, Qt::white);

    m_pCaliS1Widget->setAutoFillBackground(true);
    m_pCaliS1Widget->setPalette(pal);

    m_pCaliS1Widget->setGeometry(QRect(120 , 150 , 530 ,60));

    m_pCaliS1Label = new QLabel(m_pCaliS1Widget);
    m_pCaliS1Label->setGeometry(10, 10, 220, 40);
    m_pCaliS1Label->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
    m_pCaliS1Label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    m_pCaliS1LineEdit = new DLineEdit(m_pCaliS1Widget);
    m_pCaliS1LineEdit->setGeometry(290, 8, 120, 40);

    m_pCaliBtn = new QPushButton(m_pageWidget[MANAGER_PAGE_CALIBRATION]);
    m_pCaliBtn->move(580, 420);
    connect(m_pCaliBtn, SIGNAL(clicked()), this, SLOT(on_caliSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_CALIBRATION], icon1, tr("Calibration"));
}

void DManagerSetPage::initAudioPage()
{
    m_iSoundMask = gGlobalParam.MiscParam.iSoundMask;
    setBackColor();
    m_pageWidget[MANAGER_PAGE_AUDIO] = new QWidget;

    int iLoop ;

    m_strQss4Chk = m_wndMain->getQss4Chk();

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_pAudioBackWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_AUDIO]);

        QPalette pal(m_pAudioBackWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        m_pAudioBackWidget[iLoop]->setAutoFillBackground(true);
        m_pAudioBackWidget[iLoop]->setPalette(pal);

//        m_pAudioBackWidget[iLoop]->setGeometry(QRect(120 , 100 + 70 * iLoop , 530 ,60));
        m_pAudioBackWidget[iLoop]->setGeometry(QRect(120 , 120 + 70 * iLoop , 530 ,60));

        m_lblNames[iLoop] = new QLabel(m_pAudioBackWidget[iLoop]);
        m_lblNames[iLoop]->setPixmap(NULL);
        m_lblNames[iLoop]->setGeometry(QRect(25, 25 , 140 , 20));
        m_lblNames[iLoop]->setText("Sound");
        m_lblNames[iLoop]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_lblNames[iLoop]->show();
        m_lblNames[iLoop]->setAlignment(Qt::AlignLeft);

        m_chkSwitchs[iLoop] = new QCheckBox(m_pAudioBackWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(480 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(m_strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        if (m_iSoundMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));
        //2019.3.14 add
//        if(iLoop != 0)
//        {
//            m_pAudioBackWidget[iLoop]->hide();
//        }
    }

    m_pAudioBtnSave = new QPushButton(m_pageWidget[MANAGER_PAGE_AUDIO]);
    m_pAudioBtnSave->move(580, 420);
    connect(m_pAudioBtnSave, SIGNAL(clicked()), this, SLOT(on_audioBtnSavebtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_AUDIO], icon1, tr("Audio"));

}

void DManagerSetPage::initLcdPage()
{
    m_pageWidget[MANAGER_PAGE_LCD] = new QWidget;

    int iLoop = 0;

    QFile qss(":/app/slider.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();

    m_iEnergySave = gGlobalParam.MiscParam.iEnerySave;
    m_iBrightness = gGlobalParam.MiscParam.iBrightness;
    m_iSleepTime = gAdditionalCfgParam.additionalParam.iScreenSleepTime;

    for( iLoop = 0 ; iLoop < 3 ; iLoop++)
    {
        m_pLcdBackWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_LCD]);
        QPalette pal(m_pLcdBackWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pLcdBackWidget[iLoop]->setAutoFillBackground(true);
        m_pLcdBackWidget[iLoop]->setPalette(pal);

        m_pLcdBackWidget[iLoop]->setGeometry(QRect(120 , 100 + 70 * iLoop , 530 ,60));

        laName[iLoop]      = new QLabel(m_pLcdBackWidget[iLoop]);
        laName[iLoop]->setPixmap(NULL);
        laName[iLoop]->setGeometry(QRect(20, 15 , 150 , 20));
        laName[iLoop]->setAlignment(Qt::AlignLeft);
        laName[iLoop]->show();
    }

    pSlider[0] = new QSlider(m_pLcdBackWidget[0]);
    pSlider[0]->setOrientation(Qt::Horizontal);
    pSlider[0]->setStyleSheet(qsss);
    pSlider[0]->setGeometry(QRect(140 , 10 ,370,40));
    pSlider[0]->setMinimum(0);

    if(0 == gAdditionalCfgParam.productInfo.iCompany)
    {
        pSlider[0]->setMaximum(255);
    }
    else
    {
        pSlider[0]->setMaximum(100);
    }

    pSlider[0]->setValue(m_iBrightness);

    connect(pSlider[0], SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

    m_pCheckEnergySave = new QCheckBox(m_pLcdBackWidget[1]);
    QString strQss4Chk = m_wndMain->getQss4Chk();
    m_pCheckEnergySave->setStyleSheet(strQss4Chk);

    m_pCheckEnergySave->setGeometry(QRect(450, 15, 120, 30));

    connect(m_pCheckEnergySave, SIGNAL(stateChanged(int)), this, SLOT(on_CheckEnergySave_stateChanged(int)));

    //ex
    m_sleepLabel = new QLabel(m_pLcdBackWidget[2]);
    m_comboBox = new QComboBox(m_pLcdBackWidget[2]);
    for(iLoop = 1; iLoop < 61; iLoop++)
    {
        m_comboBox->addItem(QString("%1").arg(iLoop));
    }
    m_comboBox->setCurrentIndex(m_iSleepTime - 1);
    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));

    m_sleepLabel->setGeometry(QRect(25, 15, 140 , 20));
    m_comboBox->setGeometry(QRect(200 , 10, 80, 30));
    if(m_iEnergySave)
    {
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_pLcdBackWidget[2]->hide();
    }

    //end

    m_pLcdBtnSave = new QPushButton(m_pageWidget[MANAGER_PAGE_LCD]);
    m_pLcdBtnSave->move(580,450);

    connect(m_pLcdBtnSave, SIGNAL(clicked()), this, SLOT(on_LcdSaveBtn_clicked()));


    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_LCD], icon1, tr("LCD"));
}

void DManagerSetPage::initAdditionalSettingsPage()
{
    setBackColor();
    m_pageWidget[MANAGER_PAGE_ADDSETTINGS] = new QWidget;

    int iLoop ;

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    for(iLoop = 0 ; iLoop < ADDITIONAL_NUM ; iLoop++)
    {
        m_pAdditionalWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_ADDSETTINGS]);

        QPalette pal(m_pAdditionalWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        m_pAdditionalWidget[iLoop]->setAutoFillBackground(true);
        m_pAdditionalWidget[iLoop]->setPalette(pal);

        m_pAdditionalWidget[iLoop]->setGeometry(QRect(120 , 120 + 70 * iLoop , 530 ,60));

        m_pAdditionalLb[iLoop] = new QLabel(m_pAdditionalWidget[iLoop]);
        m_pAdditionalLb[iLoop]->setPixmap(NULL);
        m_pAdditionalLb[iLoop]->setGeometry(QRect(25, 25 , 260 , 20));
        m_pAdditionalLb[iLoop]->setText("HP Recir.");
        m_pAdditionalLb[iLoop]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_pAdditionalLb[iLoop]->show();
        m_pAdditionalLb[iLoop]->setAlignment(Qt::AlignLeft);

        m_pAdditionalCheck[iLoop] = new QCheckBox(m_pAdditionalWidget[iLoop]);

        m_pAdditionalCheck[iLoop]->setGeometry(QRect(480 , 9 ,40,40));

        QString strQss4Chk = m_wndMain->getQss4Chk();
        m_pAdditionalCheck[iLoop]->setStyleSheet(strQss4Chk);

        m_pAdditionalCheck[iLoop]->show();

        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            m_pAdditionalCheck[iLoop]->setChecked(true);
        }
        else
        {
            m_pAdditionalCheck[iLoop]->setChecked(false);
        }

        connect(m_pAdditionalCheck[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_HPCircheckBox_changeState(int)));

    }

    m_pAddBtnSave = new QPushButton(m_pageWidget[MANAGER_PAGE_ADDSETTINGS]);
    m_pAddBtnSave->move(580, 420);
    connect(m_pAddBtnSave, SIGNAL(clicked()), this, SLOT(on_AdditionalBtnSave_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_ADDSETTINGS], icon1, tr("Additional Settings"));
}

void DManagerSetPage::changeTime()
{
    //QDateTime time;
    time_t    Time_t;
    struct tm curtime;
    struct rtc_time rtctime ;

    int fb;

    QStringList timeStrList = m_pBtns[TIMEPAGE_BTN_TIME_SET]->text().split(QRegExp("[:]"));

    QStringList dateStrList = m_pBtns[TIMEPAGE_BTN_DATE_SET]->text().split(QRegExp("[-]"));

    int iYear,iMon,iDay,iHour,iMin,iSec;

    iSec  = QString(timeStrList[2]).toInt();
    iMin  = QString(timeStrList[1]).toInt();
    iHour = QString(timeStrList[0]).toInt();
    iDay  = QString(dateStrList[2]).toInt();
    iMon  = QString(dateStrList[1]).toInt();
    iYear = QString(dateStrList[0]).toInt();

    curtime.tm_sec  = iSec;
    curtime.tm_min  = iMin;
    curtime.tm_hour = iHour;
    curtime.tm_mday = iDay;
    curtime.tm_mon  = iMon  - 1;
    curtime.tm_year = iYear - 1900;
    curtime.tm_isdst= -1;

    Time_t = mktime(&curtime);

    stime(&Time_t);

    struct tm *tmp_ptr = NULL;

    tmp_ptr = gmtime(&Time_t);

    rtctime.tm_sec  = tmp_ptr->tm_sec;
    rtctime.tm_min  = tmp_ptr->tm_min;
    rtctime.tm_hour = tmp_ptr->tm_hour;
    rtctime.tm_mday = tmp_ptr->tm_mday;
    rtctime.tm_mon  = tmp_ptr->tm_mon;
    rtctime.tm_year = tmp_ptr->tm_year;
    rtctime.tm_isdst=-1;

    fb = open("/dev/rtc0", QIODevice::ReadWrite);
    if (fb != -1)
    {
        int ret = ioctl(fb , RTC_SET_TIME , &rtctime);

        if (ret)
        {
            qDebug() << " ioctl : " << ret;
        }

        ::close(fb);
    }
}

