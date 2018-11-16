#include "allocationsetpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include <QPixmap>



AllocationSetPage::AllocationSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

}

void AllocationSetPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void AllocationSetPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Dist. Control");

    setTitles(stringList);

}

void AllocationSetPage::buildTranslation()
{
    m_lbNameTitle->setText(tr("Dist. Control"));
    m_lbNameStartTime->setText(tr("Start Time"));
    m_lbNameEndTime->setText(tr("Stop Time"));
    m_lbNameWeekday->setText(tr("Working Days"));
    m_lbNameIdlePeriod->setText(tr("Recir. Period"));
    m_lbNameIdleCirTime->setText(tr("Recir. Duration"));
    m_lbNameIdlePeriodUnit->setText(tr("Min"));
    m_lbNameIdleCirTimeUnit->setText(tr("Min"));

    m_pBtns[0]->setTip(tr("Sun."));
    m_pBtns[1]->setTip(tr("Mon."));
    m_pBtns[2]->setTip(tr("Tue."));
    m_pBtns[3]->setTip(tr("Wed."));
    m_pBtns[4]->setTip(tr("Thu."));
    m_pBtns[5]->setTip(tr("Fri."));
    m_pBtns[6]->setTip(tr("Sat."));
    
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);

}

void AllocationSetPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}



void AllocationSetPage::setBackColor()
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

#define ITEM_HEIGHT (60)
#define ITEM_WIDTH (600)
void AllocationSetPage::initUi()
{
    int iIdx = 0;
    int iLoop = 0;
    setBackColor();

    m_pBackWidget[iIdx] = new QWidget(m_widget);
    
    QPalette pal(m_pBackWidget[iIdx]->palette());
    
    pal.setColor(QPalette::Background, Qt::white);
    
    m_pBackWidget[iIdx]->setAutoFillBackground(true);
    m_pBackWidget[iIdx]->setPalette(pal);
    
    m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
    
    m_lbNameTitle = new QLabel(m_pBackWidget[iIdx]);
    m_lbNameTitle->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 140 , 20));
    m_lbNameTitle->setAlignment(Qt::AlignLeft);

    m_pBtnEnable = new CBitmapButton(m_pBackWidget[iIdx],BITMAPBUTTON_STYLE_SWITCH,BITMAPBUTTON_PIC_STYLE_NORMAL,0);
    m_pBtnEnable->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SWITCH_OFF]);
    m_pBtnEnable->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_SWITCH_ON]);
    m_pBtnEnable->setGeometry(400,ITEM_HEIGHT/2 - m_pBtnEnable->getPicHeight()/2 ,m_pBtnEnable->getPicWidth(),m_pBtnEnable->getPicHeight());
    m_pBtnEnable->setStyleSheet("background-color:transparent");
    iIdx++;
    

   m_pBackWidget[iIdx] = new QWidget(m_widget);
   m_pBackWidget[iIdx]->setAutoFillBackground(true);
   m_pBackWidget[iIdx]->setPalette(pal);
   m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
   
   m_lbNameStartTime = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameStartTime->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 140 , 20));
   m_lbNameStartTime->setAlignment(Qt::AlignLeft);

   m_cmbStartTimeHour = new QComboBox(m_pBackWidget[iIdx]);
   m_cmbStartTimeHour->setGeometry(270,ITEM_HEIGHT/2 - 10 - 15,50,30);
   m_cmbStartTimeMin = new QComboBox(m_pBackWidget[iIdx]);
   m_cmbStartTimeMin->setGeometry(325,ITEM_HEIGHT/2 - 10 - 15,50,30);
   iIdx++;

   for(iLoop = 0 ; iLoop < 24 ; iLoop++ )
   {
       m_cmbStartTimeHour->addItem(QString::number(iLoop,10));
   }
   
   
   for(iLoop = 0 ; iLoop < 60 ; iLoop++ )
   {
       m_cmbStartTimeMin->addItem(QString::number(iLoop,10));
   }
   
   m_pBackWidget[iIdx] = new QWidget(m_widget);
   m_pBackWidget[iIdx]->setAutoFillBackground(true);
   m_pBackWidget[iIdx]->setPalette(pal);
   m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
   
   m_lbNameEndTime = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameEndTime->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 140 , 20));
   m_lbNameEndTime->setAlignment(Qt::AlignLeft);
   
   m_cmbEndTimeHour = new QComboBox(m_pBackWidget[iIdx]);
   m_cmbEndTimeHour->setGeometry(270,ITEM_HEIGHT/2 - 10 - 15,50,30);
   m_cmbEndTimeMin = new QComboBox(m_pBackWidget[iIdx]);
   m_cmbEndTimeMin->setGeometry(325,ITEM_HEIGHT/2 - 10 - 15,50,30);
   iIdx++;
   for(iLoop = 0 ; iLoop < 24 ; iLoop++ )
   {
       m_cmbEndTimeHour->addItem(QString::number(iLoop,10));
   }
   
   
   for(iLoop = 0 ; iLoop < 60 ; iLoop++ )
   {
       m_cmbEndTimeMin->addItem(QString::number(iLoop,10));
   }

   m_pBackWidget[iIdx] = new QWidget(m_widget);
   m_pBackWidget[iIdx]->setAutoFillBackground(true);
   m_pBackWidget[iIdx]->setPalette(pal);
   m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
   
   m_lbNameWeekday = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameWeekday->adjustSize();
   m_lbNameWeekday->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 110 , 20*2));
   m_lbNameWeekday->setWordWrap(true); //
   m_lbNameWeekday->setAlignment(Qt::AlignHCenter);
//   m_lbNameWeekday->setAlignment(Qt::AlignLeft);

   for (iLoop = 0; iLoop < 7; iLoop++)
   {
       m_pBtns[iLoop] = new CBitmapButton(m_pBackWidget[iIdx],BITMAPBUTTON_STYLE_SWITCH,0,0);
       m_pBtns[iLoop]->setGeometry(130 + iLoop*70 ,ITEM_HEIGHT/2 - 15 ,60,30);
       m_pBtns[iLoop]->setStyleSheet("background-color:transparent");
       m_pBtns[iLoop]->setColor(Qt::black,BITMAPBUTTON_STATE_UNSEL);
       m_pBtns[iLoop]->setColor(Qt::red,BITMAPBUTTON_STATE_SEL);
   
   }
   iIdx++;   

   m_pBackWidget[iIdx] = new QWidget(m_widget);
   m_pBackWidget[iIdx]->setAutoFillBackground(true);
   m_pBackWidget[iIdx]->setPalette(pal);
   m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
   
   m_lbNameIdlePeriod = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameIdlePeriod->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 140 , 20));
   m_lbNameIdlePeriod->setAlignment(Qt::AlignLeft);
   
   m_leIdlePeriod = new QLineEdit(m_pBackWidget[iIdx]);
   m_leIdlePeriod->setGeometry(270,ITEM_HEIGHT/2 - 15,50,30);
   m_leIdlePeriod->setValidator(new QIntValidator(0, 999, this));

   m_lbNameIdlePeriodUnit = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameIdlePeriodUnit->setGeometry(QRect(325, ITEM_HEIGHT/2 , 40 , 20));
   m_lbNameIdlePeriodUnit->setAlignment(Qt::AlignLeft);
   
   iIdx++;
   
   m_pBackWidget[iIdx] = new QWidget(m_widget);
   m_pBackWidget[iIdx]->setAutoFillBackground(true);
   m_pBackWidget[iIdx]->setPalette(pal);
   m_pBackWidget[iIdx]->setGeometry(QRect(100 , 70 + 70 * iIdx , ITEM_WIDTH ,ITEM_HEIGHT));
   
   m_lbNameIdleCirTime = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameIdleCirTime->setGeometry(QRect(10, ITEM_HEIGHT/2 - 10 , 140 , 20));
   m_lbNameIdleCirTime->setAlignment(Qt::AlignLeft);
   
   m_leIdleCirTime = new QLineEdit(m_pBackWidget[iIdx]);
   m_leIdleCirTime->setGeometry(270,ITEM_HEIGHT/2 - 30/2 ,50,30);
   m_leIdleCirTime->setValidator(new QIntValidator(0, 99, this));
   
   m_lbNameIdleCirTimeUnit = new QLabel(m_pBackWidget[iIdx]);
   m_lbNameIdleCirTimeUnit->setGeometry(QRect(325, ITEM_HEIGHT/2 , 40 , 20));
   m_lbNameIdleCirTimeUnit->setAlignment(Qt::AlignLeft);
   
   iIdx++;

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,ALLOCPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();
    
}

void AllocationSetPage::update()
{
    int iLoop;

    int iTmp;

    if (gGlobalParam.MiscParam.ulAllocMask & (1 << DISPLAY_ALLOC_FLAG_SWITCHON))
    {
        m_pBtnEnable->setState(BITMAPBUTTON_STATE_SEL);
    }
    else
    {
        m_pBtnEnable->setState(BITMAPBUTTON_STATE_UNSEL);
    }
    
    for (iLoop = 0; iLoop <= ALLOCPAGE_BTN_SATURDAY; iLoop++)
    {
        if (gGlobalParam.MiscParam.ulAllocMask & (1 << iLoop))
        {
            m_pBtns[iLoop]->setState(BITMAPBUTTON_STATE_SEL);
        }
        else
        {
            m_pBtns[iLoop]->setState(BITMAPBUTTON_STATE_UNSEL);
        }
    }

    iTmp = (gGlobalParam.MiscParam.aiTubeCirTimeInfo[0] >> 8) & 0XFF;

    if (iTmp < 24)
    {
        m_cmbStartTimeHour->setCurrentIndex(iTmp);
    }
    
    iTmp = (gGlobalParam.MiscParam.aiTubeCirTimeInfo[0] >> 0) & 0XFF;

    if (iTmp < 60)
    {
        m_cmbStartTimeMin->setCurrentIndex(iTmp);
    }


    iTmp = (gGlobalParam.MiscParam.aiTubeCirTimeInfo[1] >> 8) & 0XFF;
    
    if (iTmp < 24)
    {
        m_cmbEndTimeHour->setCurrentIndex(iTmp);
    }
    
    iTmp = (gGlobalParam.MiscParam.aiTubeCirTimeInfo[1] >> 0) & 0XFF;
    
    if (iTmp < 60)
    {
        m_cmbEndTimeMin->setCurrentIndex(iTmp);
    }

    m_leIdlePeriod->setText(QString::number(gGlobalParam.MiscParam.iTubeCirCycle));
    m_leIdleCirTime->setText(QString::number(gGlobalParam.MiscParam.iTubeCirDuration));
}

void AllocationSetPage::save()
{
    int iMask = 0;
    int iTubeCirCycle;
    int iTubeCirDuration;
    int aiTubeCirTimeInfo[2]; 
    int iLoop;

    if (BITMAPBUTTON_STATE_SEL == m_pBtnEnable->getState())
    {
        iMask |= 1 << DISPLAY_ALLOC_FLAG_SWITCHON;
    }

    for (iLoop = 0; iLoop <= ALLOCPAGE_BTN_SATURDAY; iLoop++)
    {
        if (BITMAPBUTTON_STATE_SEL == m_pBtns[iLoop]->getState())
        {
            iMask |= 1 << iLoop;
        }
        
    }

    iTubeCirCycle    = m_leIdlePeriod->text().toInt();
    iTubeCirDuration = m_leIdleCirTime->text().toInt();

    aiTubeCirTimeInfo[0] = (m_cmbStartTimeHour->currentIndex() << 8) | (m_cmbStartTimeMin->currentIndex() << 0);
    aiTubeCirTimeInfo[1] = (m_cmbEndTimeHour->currentIndex()   << 8) | (m_cmbEndTimeMin->currentIndex() << 0);

    if (iMask != gGlobalParam.MiscParam.ulAllocMask
        || iTubeCirCycle != gGlobalParam.MiscParam.iTubeCirCycle
        || iTubeCirDuration != gGlobalParam.MiscParam.iTubeCirDuration
        || aiTubeCirTimeInfo[0] != gGlobalParam.MiscParam.aiTubeCirTimeInfo[0]
        || aiTubeCirTimeInfo[1] != gGlobalParam.MiscParam.aiTubeCirTimeInfo[1]
        )
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.ulAllocMask      = iMask;
       MiscParam.iTubeCirCycle    = iTubeCirCycle;
       MiscParam.iTubeCirDuration = iTubeCirDuration;
       MiscParam.aiTubeCirTimeInfo[0] = aiTubeCirTimeInfo[0];
       MiscParam.aiTubeCirTimeInfo[1] = aiTubeCirTimeInfo[1];

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       m_wndMain->updateTubeCirCfg();
       
       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_ALLOCATION);
       
    }
    
    show(false);
    m_parent->show(true);

}

void AllocationSetPage::on_btn_clicked(int index)
{
    switch(index)
    {
    case ALLOCPAGE_BTN_SAVE:
        save();
        break;
    default:
        break;
    }
    
    m_wndMain->prepareKeyStroke();
}

void AllocationSetPage::leaveSubPage()
{    
    CSubPage::leaveSubPage();
}

