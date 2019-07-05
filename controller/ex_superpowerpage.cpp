#include "ex_superpowerpage.h"
#include "titlebar.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include <QPainter>
#include <QMessageBox>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "dlineedit.h"

#define ControlNum 6

#define BACKWIDGET_START_X      10
#define BACKWIDGET_START_Y      80
#define BACKWIDGET_START_HIATUS 80
#define BACKWIDGET_HEIGHT       60
#define BACKWIDGET_WIDTH        (800 - BACKWIDGET_START_X*2)
#define BACKWIDGET_ITEM_LENGTH  120
#define BACKWIDGET_ITEM_HEIGHT  30

static  QRect   sQrectAry[2] = {
        QRect(5,  BACKWIDGET_HEIGHT/2 - BACKWIDGET_ITEM_HEIGHT/2, BACKWIDGET_ITEM_LENGTH + 10, BACKWIDGET_ITEM_HEIGHT) ,
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
    m_lbDefaultState->setText(tr("Initialize"));
    m_cmbDefaultState->setItemText(0, tr("Yes"));
    m_cmbDefaultState->setItemText(1, tr("No"));

    m_lbDeviceTypeName->setText(tr("System Type"));

    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setText(tr("Catalog No"));
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setText(tr("Serial No"));
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setText(tr("Production Date"));
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setText(tr("Installation Date"));
    m_pExLabel[SYSCFGPAGE_LB_SOFTVER]->setText(tr("Software Version"));

    m_pBtnDbDel->setText(tr("Delete"));
    m_pLbDbDel->setText(tr("Delete Data"));

    m_pBtnDelCfg->setText(tr("Delete Cfg"));

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
    m_lbDefaultState->setAlignment(Qt::AlignCenter);

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 5);
    m_cmbDefaultState = new QComboBox(tmpWidget);
    m_cmbDefaultState->addItem(tr("Yes"));
    m_cmbDefaultState->addItem(tr("No"));
    m_cmbDefaultState->setCurrentIndex(1);
    m_cmbDefaultState->setGeometry(rectTmp);
    connect(m_cmbDefaultState, SIGNAL(currentIndexChanged(int)),
     this, SLOT(on_CmbIndexChange_DefaultState(int)));

    rectTmp.setX(rectTmp.x() + rectTmp.width() + 80); //120
    rectTmp.setWidth(120);
    m_lbDeviceTypeName = new QLabel(tmpWidget);
    m_lbDeviceTypeName->setGeometry(rectTmp);
    m_lbDeviceTypeName->show();

    //系统流速
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN - 10);
    rectTmp.setWidth(X_VALUE_WIDTH + 30);
    m_cmbDeviceFlow = new QComboBox(tmpWidget);
    m_cmbDeviceFlow->setGeometry(rectTmp);
    QStringList flowList;
    flowList << "5" << "10" << "12" << "15"<< "24" << "32"
             << "30" << "50" << "60" << "125" << "150" << "250" << "300" << "500" << "600";
    m_cmbDeviceFlow->addItems(flowList);

    //系统机型
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 30);
    m_cmbDeviceType = new QComboBox(tmpWidget);
    m_cmbDeviceType->setGeometry(rectTmp);
    for(iLoop = 0;iLoop < MACHINE_NUM; iLoop++)
    {
        m_cmbDeviceType->addItem(gaMachineType[iLoop].strName);
    }
    m_cmbDeviceType->setCurrentIndex(gGlobalParam.iMachineType);
    connect(m_cmbDeviceType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_CmbIndexChange_deviceType(int)));

    //line2
    tmpWidget = new QWidget(m_widget);
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT*4));
    yOffset += (BACKWIDGET_HEIGHT*4 + 20);

    QVBoxLayout* v1Layout = new QVBoxLayout;
    QVBoxLayout* v2Layout = new QVBoxLayout;
    QHBoxLayout* hLayout = new QHBoxLayout;

    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO] = new DLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]);

    m_pExLabel[SYSCFGPAGE_LB_SERIALNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO] = new DLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]);

    m_pExLabel[SYSCFGPAGE_LB_PRODATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]);

    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]);

    m_pExLabel[SYSCFGPAGE_LB_SOFTVER] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->setReadOnly(true);
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

    //Database Delete
    tmpWidget = new QWidget(m_widget);
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));

    rectTmp = QRect(5,  15, 120, 30);;
    m_pLbDbDel = new QLabel(tmpWidget);
    m_pLbDbDel->setGeometry(rectTmp);
    m_pLbDbDel->show();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 5);
    m_pCmDbDel = new QComboBox(tmpWidget);
    QStringList cbList;
    cbList << tr("All") << tr("Alarm") << tr("GetW") << tr("PWater") << tr("Log") << tr("Consumables");//  << tr("Water");
    m_pCmDbDel->addItems(cbList);
    m_pCmDbDel->setCurrentIndex(0);
    m_pCmDbDel->setGeometry(rectTmp);

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN*2);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 5);
    m_pBtnDbDel = new QPushButton(tmpWidget);
    m_pBtnDbDel->setGeometry(rectTmp);
    connect(m_pBtnDbDel, SIGNAL(clicked()), this, SLOT(on_btnDbDel_clicked()));

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN*2);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 5);
    m_pBtnDelCfg = new QPushButton(tmpWidget);
    m_pBtnDelCfg->setGeometry(rectTmp);
    connect(m_pBtnDelCfg, SIGNAL(clicked()), this, SLOT(on_btnDelCfg_clicked()));

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

bool Ex_SuperPowerPage::deleteDbAll()
{
    //water
    bool ret;
#if 0
    ret = deleteDbWater();
    if(!ret)
    {
        return false;
    }
#endif
    //alarm
    ret = deleteDbAlarm();
    if(!ret)
    {
        return false;
    }
    //getWater
    ret = deleteDbGetWater();
    if(!ret)
    {
        return false;
    }
    //product water
    ret = deleteDbPWater();
    if(!ret)
    {
        return false;
    }
    //log
    ret = deleteDbLog();
    if(!ret)
    {
        return false;
    }
    //consumables
    ret = deleteDbConsumables();
    if(!ret)
    {
        return false;
    }

    return true;
}

bool Ex_SuperPowerPage::deleteDbWater()
{
    QString strSql = "Delete from Water";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Water"), tr("Deleting table failed: Water"), QMessageBox::Ok);
        return false;
    }

    return true;
}
bool Ex_SuperPowerPage::deleteDbAlarm()
{
    QString strSql = "Delete from Alarm";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Alarm"), tr("Deleting table failed: Alarm"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool Ex_SuperPowerPage::deleteDbGetWater()
{
    QString strSql = "Delete from GetW";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("GetWater"), tr("Deleting table failed: GetW"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool Ex_SuperPowerPage::deleteDbPWater()
{
    QString strSql = "Delete from PWater";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Product Water"), tr("Deleting table failed: pWater"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool Ex_SuperPowerPage::deleteDbLog()
{
    QString strSql = "Delete from Log";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Log"), tr("Deleting table failed: Log"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool Ex_SuperPowerPage::deleteDbConsumables()
{
    QString strSql = "Delete from Consumable";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Consumable"), tr("Deleting table failed: Consumable"), QMessageBox::Ok);
        return false;
    }
    return true;
}

void Ex_SuperPowerPage::save()
{
    ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo = m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo = m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_ProDate = m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->text();
    ex_gGlobalParam.Ex_System_Msg.Ex_InsDate = m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->text();
//    ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer = m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->text();

    ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow = m_cmbDeviceFlow->currentText().toInt();

    MainSaveExMachineMsg(gGlobalParam.iMachineType);
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
         ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow = m_cmbDeviceFlow->currentText().toInt();
         MainSaveExMachineMsg(gGlobalParam.iMachineType);

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

void Ex_SuperPowerPage::on_btnDbDel_clicked()
{
    int index = m_pCmDbDel->currentIndex();
    switch(index)
    {
    case 0:
        deleteDbAll();
        break;
    case 1:
        deleteDbAlarm();
        break;
    case 2:
        deleteDbGetWater();
        break;
    case 3:
        deleteDbPWater();
        break;
    case 4:
        deleteDbLog();
        break;
    case 5:
        deleteDbConsumables();
        break;
    default:
        break;
    }
}

void Ex_SuperPowerPage::on_btnDelCfg_clicked()
{
    QString strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += "_info.ini";

    QFile infoFile(strCfgName);
    if(!infoFile.exists())
    {
        qDebug() <<" not existe";
        QMessageBox::warning(m_widget, tr("DeleteInfoCfg"), tr("info File not existe"), QMessageBox::Ok);
    }
    else
    {
        if(infoFile.remove())
        {
            qDebug() << "Delete success";
        }
        else
        {
            qDebug() << "Delete failed";
            QMessageBox::warning(m_widget, tr("DeleteInfoCfg"), tr("info File delete failed"), QMessageBox::Ok);
        }
    }

    strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += ".ini";

    QFile cfgFile(strCfgName);
    if(!cfgFile.exists())
    {
        qDebug() <<" not existe";
        QMessageBox::warning(m_widget, tr("DeleteCfg"), tr("Cfg File not existe"), QMessageBox::Ok);
    }
    else
    {
        if(cfgFile.remove())
        {
            qDebug() << "Delete success";
        }
        else
        {
            qDebug() << "Delete failed";
            QMessageBox::warning(m_widget, tr("DeleteCfg"), tr("Cfg File delete failed"), QMessageBox::Ok);
        }
    }

    strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += "_CaliParam.ini";

    QFile cailFile(strCfgName);
    if(!cailFile.exists())
    {
        qDebug() <<" not existe";
        QMessageBox::warning(m_widget, tr("Delete Calibrate"), tr("Calibrate File not existe"), QMessageBox::Ok);
        return;
    }
    else
    {
        if(cailFile.remove())
        {
            qDebug() << "Delete success";
        }
        else
        {
            qDebug() << "Delete failed";
            QMessageBox::warning(m_widget, tr("Delete Calibrate"), tr("Calibrate File delete failed"), QMessageBox::Ok);
        }
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




