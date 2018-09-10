#include "ex_init_tankcfgpage.h"
#include "titlebar.h"

#include "mainwindow.h"
#include <QPainter>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>


#define BACKWIDGET_START_X      10
#define BACKWIDGET_START_Y      80
#define BACKWIDGET_START_HIATUS 80
#define BACKWIDGET_HEIGHT       60
#define BACKWIDGET_WIDTH        (800 - BACKWIDGET_START_X*2)
#define BACKWIDGET_ITEM_LENGTH  120
#define BACKWIDGET_ITEM_HEIGHT  30

static  QRect   sQrectAry[2] = {
        QRect(5,  BACKWIDGET_HEIGHT/2 - BACKWIDGET_ITEM_HEIGHT/2, BACKWIDGET_ITEM_LENGTH + 20, BACKWIDGET_ITEM_HEIGHT) ,
        QRect(60, 2, 110 , 18) ,
    };

Ex_Init_Tankcfgpage::Ex_Init_Tankcfgpage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
    this->hideTitleBar();
}

void Ex_Init_Tankcfgpage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_Init_Tankcfgpage::buildTitles()
{
    QStringList stringList;

    stringList << tr("System Config");

    setTitles(stringList);

}

void Ex_Init_Tankcfgpage::buildTranslation()
{
    m_pExLbTitle->setText(tr("Tank Set"));

    m_lbPWTankName->setText(tr("Pure Water Tank"));
    m_cmbPWTankVolume->setItemText(5,tr("UDF"));
    m_cmbPWTankVolume->setItemText(6,tr("NO"));

    m_lbSWTankName->setText(tr("Feed Tank"));
    m_cmbSWTankVolume->setItemText(5,tr("UDF"));
    m_cmbSWTankVolume->setItemText(6,tr("NO"));

    m_pExNextBtn->setText(tr("Next"));
    m_pExBackBtn->setText(tr("Back"));
}

void Ex_Init_Tankcfgpage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void Ex_Init_Tankcfgpage::setBackColor()
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

#define X_MARGIN (5)
#define X_ITEM_WIDTH (60)
#define X_VALUE_WIDTH (60)

void Ex_Init_Tankcfgpage::createControl()
{
    QRegExp double_rx("([0-9]{0,1}[\.][0-9]{0,2})");

    QWidget *tmpWidget = NULL;
    QRect    rectTmp;

    /* line 1*/
    tmpWidget = new QWidget(m_widget);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);

    tmpWidget->setGeometry(QRect(134 , 190 , 530 ,60));

    rectTmp = sQrectAry[0];
    rectTmp.setX(25);
    m_lbPWTankName = new QLabel(tmpWidget);
    m_lbPWTankName->setGeometry(rectTmp);
    m_lbPWTankName->hide();

    rectTmp.setX(160);
    rectTmp.setWidth(60+20);
    m_cmbPWTankVolume = new QComboBox(tmpWidget);
    m_cmbPWTankVolume->setGeometry(rectTmp);

    m_cmbPWTankVolume->addItem(tr("30"));
    m_cmbPWTankVolume->addItem(tr("60"));
    m_cmbPWTankVolume->addItem(tr("100"));
    m_cmbPWTankVolume->addItem(tr("200"));
    m_cmbPWTankVolume->addItem(tr("350"));
    m_cmbPWTankVolume->addItem(tr("UDF"));

    if(MACHINE_PURIST== gGlobalParam.iMachineType) m_cmbPWTankVolume->addItem(tr("NO"));

    connect(m_cmbPWTankVolume, SIGNAL(currentIndexChanged(int)),
     this, SLOT(on_CmbIndexChange_pw(int)));

    m_cmbPWTankVolume->hide();

    rectTmp.setX(230+20);
    rectTmp.setWidth(60);
    m_lePWTankHeight = new QLineEdit(tmpWidget);
    m_lePWTankHeight->setGeometry(rectTmp);
    m_lePWTankHeight->setValidator(new QRegExpValidator(double_rx,this));
    m_lePWTankHeight->hide();

    rectTmp.setX(295+20);
    rectTmp.setWidth(20);
    m_lbPWHUnit = new QLabel(tmpWidget);
    m_lbPWHUnit->setGeometry(rectTmp);
    m_lbPWHUnit->setText("M");
    m_lbPWHUnit->hide();

    rectTmp.setX(325+20);
    rectTmp.setWidth(60);
    m_lePWTankCap = new QLineEdit(tmpWidget);
    m_lePWTankCap->setGeometry(rectTmp);
    m_lePWTankCap->setValidator(new QIntValidator(0, 9999, this));
    m_lePWTankCap->hide();

    rectTmp.setX(390+20);
    rectTmp.setWidth(20);
    m_lbPWCUnit = new QLabel(tmpWidget);
    m_lbPWCUnit->setGeometry(rectTmp);
    m_lbPWCUnit->setText("L");
    m_lbPWCUnit->hide();

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_ADAPT:
        tmpWidget->setAutoFillBackground(false);
        break;
    default:
        m_lbPWTankName->show();
        m_cmbPWTankVolume->show();
        m_lePWTankHeight->show();
        m_lbPWHUnit->show();
        m_lePWTankCap->show();
        m_lbPWCUnit->show();
        break;
    }
    /* line 2*/
    tmpWidget = new QWidget(m_widget);

    tmpWidget->setGeometry(QRect(134 , 190 + 90, 530 ,60));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);

    rectTmp.setX(25);
    rectTmp.setWidth(sQrectAry[0].width());
    m_lbSWTankName = new QLabel(tmpWidget);
    m_lbSWTankName->setGeometry(rectTmp);
    m_lbSWTankName->hide();

    rectTmp.setX(160);
    rectTmp.setWidth(60+20);
    m_cmbSWTankVolume = new QComboBox(tmpWidget);
    m_cmbSWTankVolume->setGeometry(rectTmp);

    m_cmbSWTankVolume->addItem(tr("30"));
    m_cmbSWTankVolume->addItem(tr("60"));
    m_cmbSWTankVolume->addItem(tr("100"));
    m_cmbSWTankVolume->addItem(tr("200"));
    m_cmbSWTankVolume->addItem(tr("350"));
    m_cmbSWTankVolume->addItem(tr("UDF"));
    m_cmbSWTankVolume->addItem(tr("NO"));
    connect(m_cmbSWTankVolume, SIGNAL(currentIndexChanged(int)),
     this, SLOT(on_CmbIndexChange_sw(int)));

    m_cmbSWTankVolume->hide();

    rectTmp.setX(230+20);
    rectTmp.setWidth(60);
    m_leSWTankHeight = new QLineEdit(tmpWidget);
    m_leSWTankHeight->setGeometry(rectTmp);
    m_leSWTankHeight->setValidator(new QRegExpValidator(double_rx,this));
    m_leSWTankHeight->hide();

    rectTmp.setX(295+20);
    rectTmp.setWidth(20);
    m_lbSWHUnit = new QLabel(tmpWidget);
    m_lbSWHUnit->setGeometry(rectTmp);
    m_lbSWHUnit->setText("M");
    m_lbSWHUnit->hide();

    rectTmp.setX(325+20);
    rectTmp.setWidth(60);
    m_leSWTankCap = new QLineEdit(tmpWidget);
    m_leSWTankCap->setGeometry(rectTmp);
    m_leSWTankCap->setValidator(new QIntValidator(0, 9999, this));
    m_leSWTankCap->hide();

    rectTmp.setX(390+20);
    rectTmp.setWidth(20);
    m_lbSWCUnit = new QLabel(tmpWidget);
    m_lbSWCUnit->setGeometry(rectTmp);
    m_lbSWCUnit->setText("L");
    m_lbSWCUnit->hide();

    switch(gGlobalParam.iMachineType)
    {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
         m_lbSWTankName->show();
         m_cmbSWTankVolume->show();
         m_leSWTankHeight->show();
         m_lbSWHUnit->show();
         m_leSWTankCap->show();
         m_lbSWCUnit->show();
         break;
    default:
        tmpWidget->setAutoFillBackground(false);
        break;
    }

    m_pExNextBtn = new QPushButton(m_widget);
    m_pExBackBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_ExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_ExBackBtn_clicked()));

    m_pExBackBtn->move(200, 450);
    m_pExNextBtn->move(500, 450);
}


void Ex_Init_Tankcfgpage::initUi()
{
    setBackColor();
    createHeader();
    createControl();
}

void Ex_Init_Tankcfgpage::update()
{
    connectData();
}

void Ex_Init_Tankcfgpage::createHeader()
{
    m_pExLbTitle = new QLabel(m_widget);
    m_pExLbTitle->setGeometry(QRect(134, 135 , 220 , 28));
    m_pExLbTitle->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");

}


void Ex_Init_Tankcfgpage::connectData()
{
    int iIdx = gGlobalParam.PmParam.aiBuckType[DISP_PM_PM2];

    if (iIdx > DISP_WATER_BARREL_TYPE_NUM )
    {
        iIdx = DISP_WATER_BARREL_TYPE_030L;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_ADAPT:
        break;
    default:
      {
        if (m_cmbPWTankVolume->isVisible())
        {
            if (iIdx <= DISP_WATER_BARREL_TYPE_NO)
            {
               m_cmbPWTankVolume->setCurrentIndex(iIdx);

               if (DISP_WATER_BARREL_TYPE_UDF != iIdx)
               {
                   m_lePWTankHeight->hide();
                   m_lbPWHUnit->hide();
                   m_lePWTankCap->hide();
                   m_lbPWCUnit->hide();
               }
               else
               {
                   m_lePWTankHeight->show();
                   m_lbPWHUnit->show();
                   m_lePWTankCap->show();
                   m_lbPWCUnit->show();

                   m_lePWTankHeight->setText(QString::number(gGlobalParam.PmParam.afDepth[DISP_PM_PM2],'f',2));
                   m_lePWTankCap->setText(QString::number(gGlobalParam.PmParam.afCap[DISP_PM_PM2]));
               }
            }
        }
        break;
      }
    }

    iIdx = gGlobalParam.PmParam.aiBuckType[DISP_PM_PM3];

    if (iIdx > DISP_WATER_BARREL_TYPE_NUM )
    {
        iIdx = DISP_WATER_BARREL_TYPE_030L;
    }

    switch(gGlobalParam.iMachineType)
    {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
        {
            if (m_cmbSWTankVolume->isVisible())
            {
                if (iIdx <= DISP_WATER_BARREL_TYPE_NO)
                {
                   m_cmbSWTankVolume->setCurrentIndex(iIdx);

                   if (DISP_WATER_BARREL_TYPE_UDF != iIdx)
                   {
                       m_leSWTankHeight->hide();
                       m_lbSWHUnit->hide();
                       m_leSWTankCap->hide();
                       m_lbSWCUnit->hide();
                   }
                   else
                   {
                       m_leSWTankHeight->show();
                       m_lbSWHUnit->show();
                       m_leSWTankCap->show();
                       m_lbSWCUnit->show();

                       m_leSWTankHeight->setText(QString::number(gGlobalParam.PmParam.afDepth[DISP_PM_PM3],'f',2));
                       m_leSWTankCap->setText(QString::number(gGlobalParam.PmParam.afCap[DISP_PM_PM3]));
                   }
                }
            }
        }
        break;
    }
}

void Ex_Init_Tankcfgpage::save()
{
   DISP_PM_SETTING_STRU          pmParam = gGlobalParam.PmParam;
   DISP_SUB_MODULE_SETTING_STRU  smParam = gGlobalParam.SubModSetting;

   pmParam.aiBuckType[DISP_PM_PM2] = m_cmbPWTankVolume->currentIndex();

   if (m_cmbPWTankVolume->isVisible())
   {
       smParam.ulFlags |= 1 << DISP_SM_HaveB2;

       switch (pmParam.aiBuckType[DISP_PM_PM2])
       {
       case DISP_WATER_BARREL_TYPE_030L:
           pmParam.afCap[DISP_PM_PM2] = 30;
           pmParam.afDepth[DISP_PM_PM2] = 0.30;
           break;
       case DISP_WATER_BARREL_TYPE_060L:
           pmParam.afCap[DISP_PM_PM2] = 60;
           pmParam.afDepth[DISP_PM_PM2] = 0.60;
           break;
       case DISP_WATER_BARREL_TYPE_100L:
           pmParam.afCap[DISP_PM_PM2] = 100;
           pmParam.afDepth[DISP_PM_PM2] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_200L:
           pmParam.afCap[DISP_PM_PM2] = 200;
           pmParam.afDepth[DISP_PM_PM2] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_350L:
           pmParam.afCap[DISP_PM_PM2] = 350;
           pmParam.afDepth[DISP_PM_PM2] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_UDF:
           pmParam.afCap[DISP_PM_PM2] = m_lePWTankCap->text().toInt();
           pmParam.afDepth[DISP_PM_PM2] = m_lePWTankHeight->text().toFloat();
           break;
       case DISP_WATER_BARREL_TYPE_NO:
           smParam.ulFlags &= ~(1 << DISP_SM_HaveB2);
           break;
       }
   }
   else
   {
       smParam.ulFlags &= ~(1 << DISP_SM_HaveB2);
   }

   pmParam.aiBuckType[DISP_PM_PM3] = m_cmbSWTankVolume->currentIndex();

   if (m_cmbSWTankVolume->isVisible())
   {
       smParam.ulFlags |= 1 << DISP_SM_HaveB3;

       switch (pmParam.aiBuckType[DISP_PM_PM3])
       {
       case DISP_WATER_BARREL_TYPE_030L:
           pmParam.afCap[DISP_PM_PM3] = 30;
           pmParam.afDepth[DISP_PM_PM3] = 0.30;
           break;
       case DISP_WATER_BARREL_TYPE_060L:
           pmParam.afCap[DISP_PM_PM3] = 60;
           pmParam.afDepth[DISP_PM_PM3] = 0.60;
           break;
       case DISP_WATER_BARREL_TYPE_100L:
           pmParam.afCap[DISP_PM_PM3] = 100;
           pmParam.afDepth[DISP_PM_PM3] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_200L:
           pmParam.afCap[DISP_PM_PM3] = 200;
           pmParam.afDepth[DISP_PM_PM3] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_350L:
           pmParam.afCap[DISP_PM_PM3] = 350;
           pmParam.afDepth[DISP_PM_PM3] = 1.00;
           break;
       case DISP_WATER_BARREL_TYPE_UDF:
           pmParam.afCap[DISP_PM_PM3] = m_leSWTankCap->text().toInt();
           pmParam.afDepth[DISP_PM_PM3] = m_leSWTankHeight->text().toFloat();
           break;
       case DISP_WATER_BARREL_TYPE_NO:
           smParam.ulFlags &= ~(1 << DISP_SM_HaveB3);
           break;
       }
   }
   else
   {
       smParam.ulFlags &= ~(1 << DISP_SM_HaveB3);
   }

   MainSavePMParam(gGlobalParam.iMachineType,pmParam);
   MainSaveSubModuleSetting(gGlobalParam.iMachineType,smParam);

   MainUpdateGlobalParam();

}


void Ex_Init_Tankcfgpage::on_CmbIndexChange_pw(int index)
{
    int iIdx = m_cmbPWTankVolume->currentIndex();

    (void)index;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_ADAPT:
        break;
    default:
      {
          if (m_cmbPWTankVolume->isVisible())
          {
              if (DISP_WATER_BARREL_TYPE_UDF == iIdx)
              {
                  m_lePWTankHeight->show();
                  m_lbPWHUnit->show();
                  m_lePWTankCap->show();
                  m_lbPWCUnit->show();

                  m_lePWTankHeight->setText(QString::number(gGlobalParam.PmParam.afDepth[DISP_PM_PM2],'f',2));
                  m_lePWTankCap->setText(QString::number(gGlobalParam.PmParam.afCap[DISP_PM_PM2]));

              }
              else
              {
                  m_lePWTankHeight->hide();
                  m_lbPWHUnit->hide();
                  m_lePWTankCap->hide();
                  m_lbPWCUnit->hide();

              }
          }
        break;
      }
    }

}

void Ex_Init_Tankcfgpage::on_CmbIndexChange_sw(int index)
{
    int iIdx = m_cmbSWTankVolume->currentIndex();
    (void)index;

    switch(gGlobalParam.iMachineType)
    {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
        {
            if (m_cmbSWTankVolume->isVisible())
            {
                if (DISP_WATER_BARREL_TYPE_UDF == iIdx)
                {
                    m_leSWTankHeight->show();
                    m_lbSWHUnit->show();
                    m_leSWTankCap->show();
                    m_lbSWCUnit->show();

                    m_leSWTankHeight->setText(QString::number(gGlobalParam.PmParam.afDepth[DISP_PM_PM3],'f',2));
                    m_leSWTankCap->setText(QString::number(gGlobalParam.PmParam.afCap[DISP_PM_PM3]));
                }
                else
                {
                    m_leSWTankHeight->hide();
                    m_lbSWHUnit->hide();
                    m_leSWTankCap->hide();
                    m_lbSWCUnit->hide();
                }
            }
        }
        break;
    }
}

void Ex_Init_Tankcfgpage::on_ExNextBtn_clicked()
{
    save();
    emit tankcfgSwitchBtnClicked(1);
}

void Ex_Init_Tankcfgpage::on_ExBackBtn_clicked()
{
    emit tankcfgSwitchBtnClicked(0);
}






