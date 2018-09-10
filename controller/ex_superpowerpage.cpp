#include "ex_superpowerpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include "ExtraDisplay.h"

#include <QPainter>

#include <QMessageBox>

#include <QProcess>

#define ControlNum 6

#define BACKWIDGET_START_X      10
#define BACKWIDGET_START_Y      80
#define BACKWIDGET_START_HIATUS 80
#define BACKWIDGET_HEIGHT       60
#define BACKWIDGET_WIDTH        (800 - BACKWIDGET_START_X*2)
#define BACKWIDGET_ITEM_LENGTH  120
#define BACKWIDGET_ITEM_HEIGHT  30

static  QRect   sQrectAry[2] = {
        QRect(5,  BACKWIDGET_HEIGHT/2 - BACKWIDGET_ITEM_HEIGHT/2, BACKWIDGET_ITEM_LENGTH , BACKWIDGET_ITEM_HEIGHT) ,
        QRect(60, 2, 110 , 18) ,
    };

Ex_SuperPowerPage::Ex_SuperPowerPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain)
                                   : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void Ex_SuperPowerPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_SuperPowerPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Super Power");
    setTitles(stringList);
}

void Ex_SuperPowerPage::buildTranslation()
{
    m_lbDefaultState->setText(tr("Default State"));
    m_cmbDefaultState->setItemText(0, tr("Yes"));
    m_cmbDefaultState->setItemText(1, tr("No"));

    m_lbDeviceTypeName->setText(tr("System Type"));

    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setText(tr("Catalog No"));
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setText(tr("Serial No"));
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setText(tr("Production Date"));
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setText(tr("Installation Date"));
    m_pExLabel[SYSCFGPAGE_LB_SOFTVER]->setText(tr("Software Version"));

    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
}

void Ex_SuperPowerPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void Ex_SuperPowerPage::setBackColor()
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

void Ex_SuperPowerPage::createControl()
{
    int yOffset = BACKWIDGET_START_Y ;
    QWidget *tmpWidget = NULL;
    QRect    rectTmp;
    int iLoop;
    /* line 1*/
    tmpWidget = new QWidget(m_widget);

    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);

    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    rectTmp = sQrectAry[0];
    m_lbDefaultState = new QLabel(tmpWidget);
    m_lbDefaultState->setGeometry(rectTmp);
    m_lbDefaultState->show();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 5);
    m_cmbDefaultState = new QComboBox(tmpWidget);
    m_cmbDefaultState->addItem(tr("Yes"));
    m_cmbDefaultState->addItem(tr("No"));
    m_cmbDefaultState->setCurrentIndex(1);
    m_cmbDefaultState->setGeometry(rectTmp);
    connect(m_cmbDefaultState, SIGNAL(currentIndexChanged(int)),
     this, SLOT(on_CmbIndexChange_DefaultState(int)));

    rectTmp.setX(rectTmp.x() + rectTmp.width() + 120);
    rectTmp.setWidth(120);
    m_lbDeviceTypeName = new QLabel(tmpWidget);
    m_lbDeviceTypeName->setGeometry(rectTmp);
    m_lbDeviceTypeName->show();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 30);
    m_cmbDeviceType = new QComboBox(tmpWidget);
    m_cmbDeviceType->setGeometry(rectTmp);
    for(iLoop = 0;iLoop < MACHINE_NUM; iLoop++)
    {
        m_cmbDeviceType->addItem(gaMachineType[iLoop].strName);
    }
    m_cmbDeviceType->setCurrentIndex(gGlobalParam.iMachineType);
    connect(m_cmbDeviceType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_CmbIndexChange_deviceType(int)));
    //
    tmpWidget = new QWidget(m_widget);
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT*4));

    QVBoxLayout* v1Layout = new QVBoxLayout;
    QVBoxLayout* v2Layout = new QVBoxLayout;
    QHBoxLayout* hLayout = new QHBoxLayout;

    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO] = new QLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]);

    m_pExLabel[SYSCFGPAGE_LB_SERIALNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO] = new QLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]);

    m_pExLabel[SYSCFGPAGE_LB_PRODATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE] = new QLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]);

    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE] = new QLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]);

    m_pExLabel[SYSCFGPAGE_LB_SOFTVER] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER] = new QLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_SOFTVER]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]);

    v1Layout->addWidget(m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]);
    v1Layout->addWidget(m_pExLabel[SYSCFGPAGE_LB_SERIALNO]);
    v1Layout->addWidget(m_pExLabel[SYSCFGPAGE_LB_PRODATE]);
    v1Layout->addWidget(m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]);
    v1Layout->addWidget(m_pExLabel[SYSCFGPAGE_LB_SOFTVER]);

    v2Layout->addWidget(m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]);
    v2Layout->addWidget(m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]);
    v2Layout->addWidget(m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]);
    v2Layout->addWidget(m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]);
    v2Layout->addWidget(m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]);

    hLayout->addLayout(v1Layout);
    hLayout->addLayout(v2Layout);

    tmpWidget->setLayout(hLayout);
    //

    //Save Btn
    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SYSCFGPAGE_BTN_SAVE);
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    m_pBtnSave->setStyleSheet("background-color:transparent");
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    m_pBtnSave->show();

}


void Ex_SuperPowerPage::initUi()
{
    setBackColor();
    createControl();
}

void Ex_SuperPowerPage::update()
{
    connectData();
    m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->setText(ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo);
    m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]->setText(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->setText(ex_gGlobalParam.Ex_System_Msg.Ex_ProDate);
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->setText(ex_gGlobalParam.Ex_System_Msg.Ex_InsDate);
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->setText(ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer);

    m_cmbDeviceType->setCurrentIndex(gGlobalParam.iMachineType);
}

void Ex_SuperPowerPage::connectData()
{
    switch(ex_gGlobalParam.Ex_Default)
    {
    case 0:
        m_cmbDefaultState->setCurrentIndex(0);
        break;
    case 1:
        m_cmbDefaultState->setCurrentIndex(1);
        break;
    default:
        break;
    }
}

void Ex_SuperPowerPage::save()
{
    ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo = m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo = m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_ProDate = m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_InsDate = m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer = m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->text();

    MainSaveProductMsg(gGlobalParam.iMachineType);
    MainSaveInstallMsg(gGlobalParam.iMachineType);

    ex_gGlobalParam.Ex_Default = m_cmbDefaultState->currentIndex();
    MainSaveDefaultState(gGlobalParam.iMachineType);

    MainUpdateGlobalParam();

    show(false);
    m_parent->show(true);
}


void Ex_SuperPowerPage :: on_CmbIndexChange_DefaultState(int index)
{
    int iIdx = m_cmbDefaultState->currentIndex();
    if (iIdx == 0)
    {
         QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("NOTIFY"), tr("Whether to restart the device immediately?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

         if(rb != QMessageBox::Yes)
         {
             return;
         }
         ex_gGlobalParam.Ex_Default = m_cmbDefaultState->currentIndex();
         MainSaveDefaultState(gGlobalParam.iMachineType);
         MainUpdateGlobalParam();
         update();
         /**/
         Restart();
     }
}

void Ex_SuperPowerPage::Restart(void)
{
    QStringList  list;
    list<<"-qws";

    //gApp->quit();
    //gApp->closeAllWindows();

    QProcess::startDetached(gApp->applicationFilePath(),list);

    *((int *)(0)) = 0;

}

void Ex_SuperPowerPage::on_CmbIndexChange_deviceType(int index)
{
    if (index != gGlobalParam.iMachineType)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("NOTIFY"), tr("Change Device Type?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb != QMessageBox::Yes)
        {
            return;
        }
        MainSaveMachineType(index);
//        MainUpdateGlobalParam();
        update();
         /**/
        Restart();
     }
}

void Ex_SuperPowerPage::on_btn_clicked(int index)
{

   switch(index)
   {
   case SYSCFGPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }

   m_wndMain->prepareKeyStroke();//蜂鸣器报警
}




