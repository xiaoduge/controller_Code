#include "unitpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include <QRect>

#include <QList>

QString strItemName[UnitNum] ;

QString strUnitChoice[UnitNum][UnitChoice_NUM];


UnitPage::UnitPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int i,j;
    for (i =0 ; i < UnitNum; i++)
    {
        m_laName[i] = NULL;
        m_btngrp[i] = NULL;
        m_pLayout[i] = NULL;
        m_pBackWidget[i] = NULL;
        
        for (j = 0; j < UnitChoice_NUM; j++)
        {
            m_btnradios[i][j] = NULL;
        }
    }
    creatTitle();

    initUi();

    buildTranslation();

}

void UnitPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void UnitPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Units");

    setTitles(stringList);
}

void UnitPage::buildTranslation()
{
    int iLoop;

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
    
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);

}

void UnitPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void UnitPage::setBackColor()
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



void UnitPage::initUi()
{
    int iLoop;

    setBackColor();

    QFile qss(":/app/RadioButton.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();    

    for(iLoop = 0 ; iLoop < UnitNum ; iLoop++)
    {
        m_pBackWidget[iLoop] = new QWidget(m_widget);

        QPalette pal(m_pBackWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        m_pBackWidget[iLoop]->setAutoFillBackground(true);
        m_pBackWidget[iLoop]->setPalette(pal);

        m_pBackWidget[iLoop]->setGeometry(QRect(134 , 100 + 80 * iLoop , 530 ,60));

        m_laName[iLoop] = new QLabel(m_pBackWidget[iLoop]);
        m_laName[iLoop]->setPixmap(NULL);
        m_laName[iLoop]->setGeometry(QRect(25, 30 , 250 , 20));
        m_laName[iLoop]->show();
        m_laName[iLoop]->setAlignment(Qt::AlignLeft);

        m_btngrp[iLoop] = new QButtonGroup(m_pBackWidget[iLoop]);
        m_btngrp[iLoop]->setExclusive(true);

        m_pLayout[iLoop] = new QHBoxLayout(m_pBackWidget[iLoop]);

        m_btnradios[iLoop][0] = new QRadioButton(m_pBackWidget[iLoop]);
        m_btnradios[iLoop][0]->setStyleSheet(qsss);
        m_btnradios[iLoop][0]->show();

        m_btnradios[iLoop][1] = new QRadioButton(m_pBackWidget[iLoop]);
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
            m_btnradios[iLoop][2] = new QRadioButton(m_pBackWidget[iLoop]);
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

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,UNITPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,540,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}


void UnitPage::update()
{
    
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
}

void UnitPage::leaveSubPage()
{   
    CSubPage::leaveSubPage();
}
void UnitPage::save()
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

       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_UNIT);
       
    }
    
    show(false);
    m_parent->show(true);

}

void UnitPage::on_btn_clicked(int index)
{

   switch(index)
   {
   case UNITPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }
   
   m_wndMain->prepareKeyStroke();
}

void UnitPage::on_btn_clicked(QAbstractButton *button)
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
