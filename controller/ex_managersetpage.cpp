#include "ex_managersetpage.h"
#include "mainwindow.h"
#include <QPainter>
#include "Display.h"
#include "ExtraDisplay.h"
#include "ex_hintdialog.h"
#include <QRadioButton>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>
#include <time.h>
#include <QDebug>

Ex_ManagerSetPage::Ex_ManagerSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void Ex_ManagerSetPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_ManagerSetPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Manager Config");
    setTitles(stringList);

}

void Ex_ManagerSetPage::buildTranslation()
{
    m_tabWidget->setTabText(0, tr("Time & Date"));
    m_tabWidget->setTabText(1, tr("Language"));
    m_tabWidget->setTabText(2, tr("Audio"));
    m_tabWidget->setTabText(3, tr("Units"));
    m_tabWidget->setTabText(4, tr("LCD"));
    m_tabWidget->setTabText(5, tr("Additional Settings"));
//    m_tabWidget->setTabText(5, tr("Flowrate"));

//    m_flowLabel->setText(tr("Flowrate"));
//    m_flowUnit->setText(QString("L/min"));
//    m_flowSaveBtn->setText(tr("Save"));

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

    //Language
    m_pCbLan->setItemText(0, tr("English"));
    m_pCbLan->setItemText(1, tr("Chinese"));
    m_pCbLan->setItemText(2, tr("Spanish"));
    m_pCbLan->setItemText(3, tr("French"));
    m_pCbLan->setItemText(4, tr("German"));
    m_pCbLan->setItemText(5, tr("Italian"));
    m_pCbLan->setItemText(6, tr("Korean"));
    m_pCbLan->setItemText(7, tr("Russian"));

    m_pLanBtnSave->setText(tr("Save"));

    //Audio
    m_strSounds[0] = tr("Touch-tone");
    m_strSounds[1] = tr("Audio Alerts");
    m_strSounds[2] = tr("Audio Alarms");

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_lblNames[iLoop]->setText(m_strSounds[iLoop]);
    }
    m_pAudioBtnSave->setText(tr("Save"));

    //Units
    strItemName[0] = tr("Resistivity/Conductivity");
    strItemName[1] = tr("Temperature");
    strItemName[2] = tr("Pressure");
    strItemName[3] = tr("Tank Level");
    strItemName[4] = tr("Flowrate");

    strUnitChoice[0][0] = tr("omg");
    strUnitChoice[0][1] = tr("us");

    strUnitChoice[1][0] = tr("celsius");
    strUnitChoice[1][1] = tr("farenheit");

    strUnitChoice[2][0] = tr("bar");
    strUnitChoice[2][1] = tr("MPa");
    strUnitChoice[2][2] = tr("psi");

    strUnitChoice[3][0] = tr("%");
    strUnitChoice[3][1] = tr("L");

    strUnitChoice[4][0] = tr("L/min");
    strUnitChoice[4][1] = tr("gal/min");

    for(iLoop = 0 ; iLoop < UnitNum ; iLoop++)
    {
        m_laName[iLoop]->setText(strItemName[iLoop]);

        m_btnradios[iLoop][0]->setText(strUnitChoice[iLoop][0]);
        m_btnradios[iLoop][1]->setText(strUnitChoice[iLoop][1]);

        if(DISPLAY_UNIT_PRESSURE == iLoop)
        {
            m_btnradios[iLoop][2]->setText(strUnitChoice[iLoop][2]);
        }
    }
    m_pUnitsBtnSave->setText(tr("Save"));

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

void Ex_ManagerSetPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}


void Ex_ManagerSetPage::setBackColor()
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



void Ex_ManagerSetPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;
    //add page
    initTimePage();
    initLanguagePage();
    initAudioPage();
    initUnitsPage();
    initLcdPage();
    initFlowPage();

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    default:
        initAdditionalSettingsPage();
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


void Ex_ManagerSetPage::update()
{
    float flowRate = ex_gGlobalParam.Ex_Config_Param.flowRate;
    m_flowLineEdit->setText(QString::number(flowRate, 'f', 2));

    //Time
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    m_pBtns[TIMEPAGE_BTN_DATE_SET]->setText(sysDateTime.toString("yyyy-MM-dd"));
    m_pBtns[TIMEPAGE_BTN_TIME_SET]->setText(sysDateTime.toString("hh:mm:ss"));

    //units
    int iLoop;
    for(iLoop = 0 ; iLoop < UnitNum ; iLoop++)
    {
        switch(iLoop)
        {
        case 0:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Conductivity;
            break;
        case 1:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Temperature;
            break;
        case 2:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Pressure;
            break;
        case 3:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4LiquidLevel;
            break;
        case 4:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4FlowVelocity;
            break;
        }
        if (m_btnradios[iLoop][m_aiUnit[iLoop]]) m_btnradios[iLoop][m_aiUnit[iLoop]]->setChecked(true);
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

    m_iSleepTime = ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime;
    m_comboBox->setCurrentIndex(m_iSleepTime - 1);
}

void Ex_ManagerSetPage::show(bool bShow)
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

void Ex_ManagerSetPage::timerEvent(QTimerEvent *event)
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

void Ex_ManagerSetPage::on_flowSaveBtn_clicked()
{
    float flowRate = m_flowLineEdit->text().toFloat();
    ex_gGlobalParam.Ex_Config_Param.flowRate = flowRate;

    MainSaveExConfigParam(gGlobalParam.iMachineType);
    MainRetriveExConfigParam(gGlobalParam.iMachineType);

    MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_DISPLAY);

    m_wndMain->prepareKeyStroke();

//    show(false);
//    m_parent->show(true);
}

void Ex_ManagerSetPage::on_timeDateSet_clicked()
{
    ShowWidget[DATE_SET]->hide();
    ShowWidget[TIME_SET]->hide();

    CalS->show();
    m_pBtns[TIMEPAGE_BTN_CANCEL]->show();
    m_pBtns[TIMEPAGE_BTN_OK]->show();

    m_wndMain->prepareKeyStroke();

}

void Ex_ManagerSetPage::on_timeTimeSet_clicked()
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

void Ex_ManagerSetPage::on_timeOkBtn_clicked()
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

void Ex_ManagerSetPage::on_timeCancelBtn_clicked()
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

void Ex_ManagerSetPage::on_lanSaveBtn_clicked()
{
    if (m_iLanguage != gGlobalParam.MiscParam.iLan)
    {
        DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

        MiscParam.iLan = m_iLanguage;

        MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

        MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

        gApp->installTranslators(m_iLanguage);

        m_wndMain->switchLanguage();

        m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_LANGUAGE);

//        show(false);
//        m_parent->show(true);
    }
    m_wndMain->prepareKeyStroke();
}

void Ex_ManagerSetPage::on_cbLan_currentIndexChanged(int index)
{
    m_iLanguage = index;
}

void Ex_ManagerSetPage::on_checkBox_changeState(int state)
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

void Ex_ManagerSetPage::on_audioBtnSavebtn_clicked()
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

    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_ManagerSetPage::on_unitsSaveBtn_clicked()
{
    if (m_aiUnit[0] != gGlobalParam.MiscParam.iUint4Conductivity
        || m_aiUnit[1] != gGlobalParam.MiscParam.iUint4Temperature
        || m_aiUnit[2] != gGlobalParam.MiscParam.iUint4Pressure
        || m_aiUnit[3] != gGlobalParam.MiscParam.iUint4LiquidLevel
        || m_aiUnit[4] != gGlobalParam.MiscParam.iUint4FlowVelocity)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iUint4Conductivity = m_aiUnit[0];
       MiscParam.iUint4Temperature  = m_aiUnit[1];
       MiscParam.iUint4Pressure     = m_aiUnit[2];
       MiscParam.iUint4LiquidLevel  = m_aiUnit[3];
       MiscParam.iUint4FlowVelocity = m_aiUnit[4];

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       m_wndMain->switchLanguage();
       m_wndMain->emitUnitsChanged();

       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_UNIT);

    }
    m_wndMain->prepareKeyStroke();
    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_ManagerSetPage::on_btn_clicked(QAbstractButton *button)
{
    int j;

    QList<QAbstractButton*> list[UnitNum];

    for(int iLoop = 0 ; iLoop < UnitNum ; iLoop++)
    {
        list[iLoop] = m_btngrp[iLoop]->buttons();
        j = 0;

        foreach (QAbstractButton *pButton, list[iLoop])
        {
            if (pButton->isChecked())
            {
                m_aiUnit[iLoop] = j;
            }
            j++;
        }
    }
    m_wndMain->prepareKeyStroke();
}

void Ex_ManagerSetPage::on_LcdSaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

    MiscParam.iBrightness = m_iBrightness;
    MiscParam.iEnerySave  = m_iEnergySave;
    MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

    ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime = m_iSleepTime;
    MainSaveExConfigParam(gGlobalParam.iMachineType);
    MainRetriveExConfigParam(gGlobalParam.iMachineType);

    MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_DISPLAY);

    m_wndMain->prepareKeyStroke();
    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_ManagerSetPage::on_CheckEnergySave_stateChanged(int state)
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

void Ex_ManagerSetPage::on_comboBox_currentIndexChanged(int index)
{
    m_iSleepTime = index + 1;
}

void Ex_ManagerSetPage::setValue(int value)
{
    if(0 == ex_gGlobalParam.Ex_System_Msg.Ex_iCompany)
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

void Ex_ManagerSetPage::on_AdditionalBtnSave_clicked()
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

    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_ManagerSetPage::on_HPCircheckBox_changeState(int state)
{
}

void Ex_ManagerSetPage::initFlowPage()
{
    m_pageWidget[MANGER_PAGE_FLOW] = new QWidget;

    m_pFlowBackWidget = new QWidget(m_pageWidget[MANGER_PAGE_FLOW]);

    QPalette pal(m_pFlowBackWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    m_pFlowBackWidget->setAutoFillBackground(true);
    m_pFlowBackWidget->setPalette(pal);

    m_pFlowBackWidget->setGeometry(QRect(120 , 120, 530 ,60));


    m_flowLabel = new QLabel(m_pFlowBackWidget);
    m_flowLabel->setGeometry(QRect(25, 20 , 120 , 20));

    m_flowLineEdit = new QLineEdit(m_pFlowBackWidget);
    m_flowLineEdit->setGeometry(QRect(150, 20 , 100 , 20));

    QDoubleValidator *dValidator = new QDoubleValidator(0.0, 1000.0, 3, this);
    m_flowLineEdit->setValidator(dValidator);

    m_flowUnit = new QLabel(m_pFlowBackWidget);
    m_flowUnit->setGeometry(QRect(255, 20 , 100 , 20));

    m_flowSaveBtn = new QPushButton(m_pageWidget[MANGER_PAGE_FLOW]);
    m_flowSaveBtn->move(580, 400);

    connect(m_flowSaveBtn, SIGNAL(clicked()), this, SLOT(on_flowSaveBtn_clicked()));

//    QIcon icon1(":/pic/unselected.png");
//    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_FLOW], icon1, tr("Flowrate"));
}

void Ex_ManagerSetPage::initTimePage()
{
    m_pageWidget[MANGER_PAGE_TIME] = new QWidget;

    //createHeads
    QPixmap PixTit;
    PixTit.load(":/pic/time.png");

    lbTitPic = new QLabel(m_pageWidget[MANGER_PAGE_TIME]);
    lbTitPic->setGeometry(QRect(150, 100 , 28 , 28));
    lbTitPic->setPixmap(PixTit);

    lbTitName = new QLabel(m_pageWidget[MANGER_PAGE_TIME]);
    lbTitName->setGeometry(QRect(210, 105 , 200 , 28));
    lbTitName->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");

    //Create_Cal_Ok
    m_pBtns[TIMEPAGE_BTN_CANCEL] = new QPushButton(m_pageWidget[MANGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setGeometry(QRect(230, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_CANCEL], SIGNAL(clicked()), this, SLOT(on_timeCancelBtn_clicked()));

    m_pBtns[TIMEPAGE_BTN_OK]     = new QPushButton(m_pageWidget[MANGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_OK]->setGeometry(QRect(430, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_OK]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_OK], SIGNAL(clicked()), this, SLOT(on_timeOkBtn_clicked()));
    //Create_Date_Time
    int iLoop;
    int tmp;

    for(iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        qDebug() << "Create_Date_Time : " << iLoop;

        ShowWidget[iLoop] = new QWidget(m_pageWidget[MANGER_PAGE_TIME]);

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
            CalS = new QCalendarWidget(m_pageWidget[MANGER_PAGE_TIME]);

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

            TimeHCbox = new QComboBox(m_pageWidget[MANGER_PAGE_TIME]);
            TimeHCbox->setGeometry(230,200,80,30);
            TimeHCbox->hide();

            for(tmp = 0 ; tmp < 24 ; tmp++ )
            {
                TimeHCbox->addItem(QString::number(tmp,10));
            }

            TimeMCbox = new QComboBox(m_pageWidget[MANGER_PAGE_TIME]);
            TimeMCbox->setGeometry(330,200,80,30);
            TimeMCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeMCbox->addItem(QString::number(tmp,10));
            }

            TimeSCbox = new QComboBox(m_pageWidget[MANGER_PAGE_TIME]);
            TimeSCbox->setGeometry(430,200,80,30);
            TimeSCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeSCbox->addItem(QString::number(tmp,10));
            }

        }

    }
    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_TIME], icon1, tr("Time & Date"));
}

void Ex_ManagerSetPage::initLanguagePage()
{
    m_iLanguage = gGlobalParam.MiscParam.iLan;
//    setBackColor();

    m_pageWidget[MANGER_PAGE_LAN] = new QWidget;

    QPixmap worldPix = QPixmap(":/pic/blueWorld.png");

    m_pLbWorld = new QLabel(m_pageWidget[MANGER_PAGE_LAN]);
    m_pLbWorld->setPixmap(worldPix);
    m_pLbWorld->setGeometry(20, 50, worldPix.width(), worldPix.height());

    m_pCbLan = new QComboBox(m_pageWidget[MANGER_PAGE_LAN]);
    QStringList strList;
    strList << tr("English")
            << tr("Chinese")
            << tr("Spanish")
            << tr("French")
            << tr("German")
            << tr("Italian")
            << tr("Korean")
            << tr("Russian");
    m_pCbLan->addItems(strList);
    m_pCbLan->setCurrentIndex(m_iLanguage);
    m_pCbLan->setGeometry(570, 55, 160, 40);
    connect(m_pCbLan, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbLan_currentIndexChanged(int)));

    m_pLanBtnSave = new QPushButton(m_pageWidget[MANGER_PAGE_LAN]);
    m_pLanBtnSave->setGeometry(570, 130, 160, 40);
    connect(m_pLanBtnSave, SIGNAL(clicked()), this, SLOT(on_lanSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_LAN], icon1, tr("Language"));
}

void Ex_ManagerSetPage::initAudioPage()
{
    m_iSoundMask = gGlobalParam.MiscParam.iSoundMask;
    setBackColor();
    m_pageWidget[MANGER_PAGE_AUDIO] = new QWidget;

    int iLoop ;

    m_strQss4Chk = m_wndMain->getQss4Chk();

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_pAudioBackWidget[iLoop] = new QWidget(m_pageWidget[MANGER_PAGE_AUDIO]);

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
        if(iLoop != 0)
        {
            m_pAudioBackWidget[iLoop]->hide();
        }
    }

    m_pAudioBtnSave = new QPushButton(m_pageWidget[MANGER_PAGE_AUDIO]);
    m_pAudioBtnSave->move(580, 420);
    connect(m_pAudioBtnSave, SIGNAL(clicked()), this, SLOT(on_audioBtnSavebtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_AUDIO], icon1, tr("Audio"));

}

void Ex_ManagerSetPage::initUnitsPage()
{
    m_pageWidget[MANGER_PAGE_UNITS] = new QWidget;

    int iLoop;

//    setBackColor();

    QFile qss(":/app/RadioButton.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();

    for(iLoop = 0 ; iLoop < UnitNum ; iLoop++)
    {
        m_pUnitsBackWidget[iLoop] = new QWidget(m_pageWidget[MANGER_PAGE_UNITS]);

        QPalette pal(m_pUnitsBackWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pUnitsBackWidget[iLoop]->setAutoFillBackground(true);
        m_pUnitsBackWidget[iLoop]->setPalette(pal);

        m_pUnitsBackWidget[iLoop]->setGeometry(QRect(120 , 50 + 80 * iLoop , 530 ,60));

        if(gGlobalParam.iMachineType == MACHINE_ADAPT)
        {
            if(3 == iLoop)
                m_pUnitsBackWidget[iLoop]->hide();
            if(4 == iLoop)
                m_pUnitsBackWidget[iLoop]->setGeometry(QRect(120 , 50 + 80 * (iLoop - 1) , 530 ,60));
        }

        m_laName[iLoop] = new QLabel(m_pUnitsBackWidget[iLoop]);
        m_laName[iLoop]->setPixmap(NULL);
        m_laName[iLoop]->setGeometry(QRect(25, 30 , 250 , 20));
        m_laName[iLoop]->show();
        m_laName[iLoop]->setAlignment(Qt::AlignLeft);

        m_btngrp[iLoop] = new QButtonGroup(m_pUnitsBackWidget[iLoop]);
        m_btngrp[iLoop]->setExclusive(true);

        m_pLayout[iLoop] = new QHBoxLayout(m_pUnitsBackWidget[iLoop]);

        m_btnradios[iLoop][0] = new QRadioButton(m_pUnitsBackWidget[iLoop]);
        m_btnradios[iLoop][0]->setStyleSheet(qsss);
        m_btnradios[iLoop][0]->show();

        m_btnradios[iLoop][1] = new QRadioButton(m_pUnitsBackWidget[iLoop]);
        m_btnradios[iLoop][1]->setStyleSheet(qsss);
        m_btnradios[iLoop][1]->show();


        m_btngrp[iLoop]->addButton(m_btnradios[iLoop][0] , 0);
        m_btngrp[iLoop]->addButton(m_btnradios[iLoop][1] , 1);

        m_pLayout[iLoop]->addStretch();

        m_pLayout[iLoop]->addWidget(m_btnradios[iLoop][0]);
        m_pLayout[iLoop]->addWidget(m_btnradios[iLoop][1]);

        m_pLayout[iLoop]->setSpacing(10);
        m_pLayout[iLoop]->setContentsMargins(200, 10, 10, 10);

        if(iLoop == 2)
        {
            m_btnradios[iLoop][2] = new QRadioButton(m_pUnitsBackWidget[iLoop]);
            m_btnradios[iLoop][2]->setStyleSheet(qsss);
            m_btnradios[iLoop][2]->show();

            m_btngrp[iLoop]->addButton(m_btnradios[iLoop][2] , 2);
            m_pLayout[iLoop]->addWidget(m_btnradios[iLoop][2]);
        }


        switch(iLoop)
        {
        case 0:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Conductivity;
            break;
        case 1:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Temperature;
            break;
        case 2:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4Pressure;
            break;
        case 3:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4LiquidLevel;
            break;
        case 4:
            m_aiUnit[iLoop] = gGlobalParam.MiscParam.iUint4FlowVelocity;
            break;
        }

        if (m_btnradios[iLoop][m_aiUnit[iLoop]]) m_btnradios[iLoop][m_aiUnit[iLoop]]->setChecked(true);

        connect(m_btngrp[iLoop], SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_btn_clicked(QAbstractButton*)));

    }

    m_pUnitsBtnSave = new QPushButton(m_pageWidget[MANGER_PAGE_UNITS]);
    m_pUnitsBtnSave->move(580,450);
    connect(m_pUnitsBtnSave, SIGNAL(clicked()), this, SLOT(on_unitsSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_UNITS], icon1, tr("Units"));
}

void Ex_ManagerSetPage::initLcdPage()
{
    m_pageWidget[MANGER_PAGE_LCD] = new QWidget;

    int iLoop = 0;

    QFile qss(":/app/slider.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();

    m_iEnergySave = gGlobalParam.MiscParam.iEnerySave;
    m_iBrightness = gGlobalParam.MiscParam.iBrightness;
    m_iSleepTime = ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime;

    for( iLoop = 0 ; iLoop < 3 ; iLoop++)
    {
        m_pLcdBackWidget[iLoop] = new QWidget(m_pageWidget[MANGER_PAGE_LCD]);
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

    if(0 == ex_gGlobalParam.Ex_System_Msg.Ex_iCompany)
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

    m_pLcdBtnSave = new QPushButton(m_pageWidget[MANGER_PAGE_LCD]);
    m_pLcdBtnSave->move(580,450);

    connect(m_pLcdBtnSave, SIGNAL(clicked()), this, SLOT(on_LcdSaveBtn_clicked()));


    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_LCD], icon1, tr("LCD"));
}

void Ex_ManagerSetPage::initAdditionalSettingsPage()
{
    setBackColor();
    m_pageWidget[MANGER_PAGE_ADDSETTINGS] = new QWidget;

    int iLoop ;

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    for(iLoop = 0 ; iLoop < ADDITIONAL_NUM ; iLoop++)
    {
        m_pAdditionalWidget[iLoop] = new QWidget(m_pageWidget[MANGER_PAGE_ADDSETTINGS]);

        QPalette pal(m_pAdditionalWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        m_pAdditionalWidget[iLoop]->setAutoFillBackground(true);
        m_pAdditionalWidget[iLoop]->setPalette(pal);

        m_pAdditionalWidget[iLoop]->setGeometry(QRect(120 , 120 + 70 * iLoop , 530 ,60));

        m_pAdditionalLb[iLoop] = new QLabel(m_pAdditionalWidget[iLoop]);
        m_pAdditionalLb[iLoop]->setPixmap(NULL);
        m_pAdditionalLb[iLoop]->setGeometry(QRect(25, 25 , 140 , 20));
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

    m_pAddBtnSave = new QPushButton(m_pageWidget[MANGER_PAGE_ADDSETTINGS]);
    m_pAddBtnSave->move(580, 420);
    connect(m_pAddBtnSave, SIGNAL(clicked()), this, SLOT(on_AdditionalBtnSave_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANGER_PAGE_ADDSETTINGS], icon1, tr("Additional Settings"));
}

void Ex_ManagerSetPage::changeTime()
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

