#include "systemcfgpage.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include "dlineedit.h"
#include "cbitmapbutton.h"

#define ControlNum 6

#define BACKWIDGET_START_X      10
#define BACKWIDGET_START_Y      70
#define BACKWIDGET_START_HIATUS 70
#define BACKWIDGET_HEIGHT       60
#define BACKWIDGET_WIDTH        (800 - BACKWIDGET_START_X*2)
#define BACKWIDGET_ITEM_LENGTH  120
#define BACKWIDGET_ITEM_HEIGHT  30

static  QRect   sQrectAry[2] = {
        QRect(5,  BACKWIDGET_HEIGHT/2 - BACKWIDGET_ITEM_HEIGHT/2, BACKWIDGET_ITEM_LENGTH , BACKWIDGET_ITEM_HEIGHT) ,
        QRect(60, 2, 110 , 18) ,
    };

SystemCfgPage::SystemCfgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;

    /*
    aCHKsIds[iIdx].iId = DISP_SM_HaveSWValve;
    iIdx++;
    */

    aCHKsIds[iIdx].iId = DISP_SM_ElecLeakProtector;
    iIdx++;

    aCHKsIds[iIdx].iId = DISP_SM_Printer;
    iIdx++;
   /*
    aCHKsIds[iIdx].iId = DISP_SM_RFID_Authorization;
    iIdx++;
  */
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    default:
//        aCHKsIds[iIdx].iId = DISP_SM_HP_Water_Cir;
//        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aCHKsIds[iIdx].iId = DISP_SM_TubeUV;
        iIdx++;
        aCHKsIds[iIdx].iId = DISP_SM_TubeDI;
        iIdx++;
        aCHKsIds[iIdx].iId = DISP_SM_HaveTubeFilter;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
        break;
    default:
        aCHKsIds[iIdx].iId = DISP_SM_Pre_Filter;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aCHKsIds[iIdx].iId = DISP_SM_H_PACK;
        iIdx++;
        aCHKsIds[iIdx].iId = DISP_SM_HP_Electrode;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aCHKsIds[iIdx].iId = DISP_SM_SW_PUMP;
        iIdx++;
        break;
    }

    m_iRealChkNum = iIdx;
    
    creatTitle();

    initUi();

    buildTranslation();
}

void SystemCfgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SystemCfgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("System Config");

    setTitles(stringList);

}

void SystemCfgPage::buildTranslation()
{
    m_lbPWTankName->setText(tr("Pure Water Tank"));
    m_cmbPWTankVolume->setItemText(5,tr("UDF"));
    m_cmbPWTankVolume->setItemText(6,tr("NO"));
    
    m_lbSWTankName->setText(tr("Feed Tank"));
    m_cmbSWTankVolume->setItemText(5,tr("UDF"));
    m_cmbSWTankVolume->setItemText(6,tr("NO"));

    m_lbPWHUnit->setText(tr("Height(M)"));
    m_lbPWCUnit->setText(tr("Volume(L)"));
    m_lbSWHUnit->setText(tr("Height(M)"));
    m_lbSWCUnit->setText(tr("Volume(L)"));

    
    for (int iLoop = 0; iLoop < m_iRealChkNum; iLoop++)
    {
        switch (aCHKsIds[iLoop].iId)
        {
        case DISP_SM_HaveSWValve:
            m_aChks[iLoop]->setText(tr("Feed Valve"));
            break;
        case DISP_SM_ElecLeakProtector:
            m_aChks[iLoop]->setText(tr("Leak Protector"));
            break;
        case DISP_SM_Printer:
            m_aChks[iLoop]->setText(tr("Printer"));
            break;
        case DISP_SM_TubeUV:
            m_aChks[iLoop]->setText(tr("Loop UV"));
            break;
        case DISP_SM_TubeDI:
            m_aChks[iLoop]->setText(tr("Loop DI"));
            break;
        case DISP_SM_HaveTubeFilter:
            m_aChks[iLoop]->setText(tr("Loop Filter"));
            break;
        case DISP_SM_HP_Water_Cir:
            m_aChks[iLoop]->setText(tr("HP Recir."));
            break;
        case DISP_SM_Pre_Filter:
            m_aChks[iLoop]->setText(tr("Prefilter"));
            break;
        case DISP_SM_RFID_Authorization:
            m_aChks[iLoop]->setText(tr("Disable RFID"));
            break;
        case DISP_SM_H_PACK:
            m_aChks[iLoop]->setText(tr("H PACK"));
            break;
        case DISP_SM_HP_Electrode:
            m_aChks[iLoop]->setText(tr("HP ELECTRODE"));
            break;
        case DISP_SM_SW_PUMP:
            m_aChks[iLoop]->setText(tr("Feed PUMP"));
            break;
        }
    }

    m_chkPWTankUV->setText(tr("Tank UV"));
    m_lbPWTankUVName->setText(tr("Exposure Time"));
    m_lbPWTankUVUnit->setText(tr("min/H"));

    m_lbPOweronFlushName->setText(tr("Flush Time"));
    m_lbPOweronFlushUnit->setText(tr("min"));

    m_lbLoginLingerName->setText(tr("Auto. Logout"));
    m_lbLoginLingerUnit->setText(tr("min"));

    m_lbDeviceTypeName->setText(tr("System Type"));

    m_chkDeviceTypeTOC->setText(tr("TOC"));

    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);

}

void SystemCfgPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void SystemCfgPage::setBackColor()
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

void SystemCfgPage::createControl()
{
    QString strQss4Chk = m_wndMain->getQss4Chk();

    QRegExp double_rx("([0-9]{0,1}[\.][0-9]{0,2})"); 

    int yOffset = BACKWIDGET_START_Y ;
    QWidget *tmpWidget = NULL;
    QRect    rectTmp;

    /* line 1*/
    tmpWidget = new QWidget(m_widget);
    
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));
    
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    rectTmp = sQrectAry[0];

    m_lbDeviceTypeName = new QLabel(tmpWidget);
    m_lbDeviceTypeName->setGeometry(rectTmp);
    m_lbDeviceTypeName->show();
    m_lbDeviceTypeName->setAlignment(Qt::AlignCenter);

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(X_VALUE_WIDTH*2 + 30); //+5

    m_lbDeviceType = new QLabel(tmpWidget);
    m_lbDeviceType->setGeometry(rectTmp);
    m_lbDeviceType->show();
    m_lbDeviceType->setLineWidth(0);
    m_lbDeviceType->setMidLineWidth(2);
    m_lbDeviceType->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_lbDeviceType->setAlignment(Qt::AlignCenter);
    setDeviceType();

    rectTmp.setX(BACKWIDGET_WIDTH/2 + X_MARGIN - 5);
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH);
    m_chkDeviceTypeTOC = new QCheckBox(tmpWidget);
    m_chkDeviceTypeTOC->setGeometry(rectTmp);
    m_chkDeviceTypeTOC->setStyleSheet(strQss4Chk);
    m_chkDeviceTypeTOC->hide();

    m_bHaveToc = false;
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        m_bHaveToc = true;
        m_chkDeviceTypeTOC->show();
        break;
    }

    int iCols = (m_iRealChkNum + 1) / 2 ;
    int iRows = (m_iRealChkNum + iCols - 1)/ iCols;
    int iRow,iCol;
    
    for (iRow = 0;iRow < iRows; iRow++)
    {
        /* line 2*/
        tmpWidget = new QWidget(m_widget);
        
        tmpWidget->setAutoFillBackground(true);
        tmpWidget->setPalette(pal);
        
        tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
        yOffset += BACKWIDGET_START_HIATUS;

        rectTmp = sQrectAry[0];
        rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 60);

        for (iCol = 0; iCol < iCols; iCol++)
        {
            if (iRow * iCols + iCol < m_iRealChkNum)
            {
                m_aChks[iRow * iCols + iCol] = new QCheckBox(tmpWidget);
                m_aChks[iRow * iCols + iCol]->setGeometry(rectTmp);
                m_aChks[iRow * iCols + iCol]->setStyleSheet(strQss4Chk);
            
                rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
                rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 60);
            }
        }
        
    }

    /* line 4*/
    tmpWidget = new QWidget(m_widget);

    switch(gGlobalParam.iMachineType)
    {
   // case MACHINE_PURIST:
    case MACHINE_ADAPT:
        tmpWidget->setAutoFillBackground(false);
        tmpWidget->setPalette(pal);
        break;
    default:
        tmpWidget->setAutoFillBackground(true);
        tmpWidget->setPalette(pal);
        break;
    }
    
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    rectTmp = sQrectAry[0];
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH);
    m_chkPWTankUV = new QCheckBox(tmpWidget);
    m_chkPWTankUV->setGeometry(rectTmp);
    m_chkPWTankUV->setStyleSheet(strQss4Chk);
    m_chkPWTankUV->hide();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 20);
    m_lbPWTankUVName = new QLabel(tmpWidget);
    m_lbPWTankUVName->setGeometry(rectTmp);
    m_lbPWTankUVName->hide();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(X_ITEM_WIDTH);
    m_lePWTankUVValue = new DLineEdit(tmpWidget);
    m_lePWTankUVValue->setGeometry(rectTmp);
    m_lePWTankUVValue->setValidator(new QIntValidator(0, 100, this));
    m_lePWTankUVValue->hide();

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 10);
    rectTmp.setWidth(X_ITEM_WIDTH+30);
    m_lbPWTankUVUnit = new QLabel(tmpWidget);
    m_lbPWTankUVUnit->setGeometry(rectTmp);
    m_lbPWTankUVUnit->hide();

    switch(gGlobalParam.iMachineType)
    {
//    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        yOffset -= BACKWIDGET_START_HIATUS;
        break;
    default:
        m_chkPWTankUV->show();
        m_lbPWTankUVName->show();    
        m_lePWTankUVValue->show();
        m_lbPWTankUVUnit->show();
        break;
    }

    /* line 5*/
    tmpWidget = new QWidget(m_widget);
    
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
        rectTmp.setX(BACKWIDGET_WIDTH/2 - 20);
        rectTmp.setWidth(sQrectAry[0].width() + 40);
        m_lbPOweronFlushName = new QLabel(tmpWidget);
        m_lbPOweronFlushName->setGeometry(rectTmp);
        m_lbPOweronFlushName->hide();
    
        rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 25);
        rectTmp.setWidth(X_ITEM_WIDTH);
        m_lePOweronFlushValue = new DLineEdit(tmpWidget);
        m_lePOweronFlushValue->setGeometry(rectTmp);
        m_lePOweronFlushValue->setValidator(new QIntValidator(0, 100, this));
        m_lePOweronFlushValue->hide();
            
        rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 25);
        rectTmp.setWidth(X_ITEM_WIDTH);
        m_lbPOweronFlushUnit = new QLabel(tmpWidget);
        m_lbPOweronFlushUnit->setGeometry(rectTmp);
        m_lbPOweronFlushUnit->hide();
        break;
    default:
        rectTmp.setX(BACKWIDGET_WIDTH/2 -20);
        rectTmp.setWidth(sQrectAry[0].width() + 40);
        m_lbPOweronFlushName = new QLabel(tmpWidget);
        m_lbPOweronFlushName->setGeometry(rectTmp);
    
        rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 25);
        rectTmp.setWidth(X_ITEM_WIDTH);
        m_lePOweronFlushValue = new DLineEdit(tmpWidget);
        m_lePOweronFlushValue->setGeometry(rectTmp);
        m_lePOweronFlushValue->setValidator(new QIntValidator(0, 100, this));
            
        rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 25);
        rectTmp.setWidth(X_ITEM_WIDTH);
        m_lbPOweronFlushUnit = new QLabel(tmpWidget);
        m_lbPOweronFlushUnit->setGeometry(rectTmp);
        break;
    }
    
    rectTmp = sQrectAry[0];
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 30);
    m_lbLoginLingerName = new QLabel(tmpWidget);
    m_lbLoginLingerName->setGeometry(rectTmp);

        
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 20);
    rectTmp.setWidth(X_ITEM_WIDTH);
    m_leLoginLingerValue = new DLineEdit(tmpWidget);
    m_leLoginLingerValue->setGeometry(rectTmp);
    m_leLoginLingerValue->setValidator(new QIntValidator(0, 100, this));

    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_ITEM_WIDTH);
    m_lbLoginLingerUnit = new QLabel(tmpWidget);
    m_lbLoginLingerUnit->setGeometry(rectTmp);

    /* line 6*/
    tmpWidget = new QWidget(m_widget);
    
    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    
    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    rectTmp = sQrectAry[0];
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 60);
    m_lbPWTankName = new QLabel(tmpWidget);
    m_lbPWTankName->setGeometry(rectTmp);
    m_lbPWTankName->hide();
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_ITEM_WIDTH + 60);
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
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 20);
    rectTmp.setWidth(X_VALUE_WIDTH);
    m_lePWTankHeight = new DLineEdit(tmpWidget);
    m_lePWTankHeight->setGeometry(rectTmp);
    m_lePWTankHeight->setValidator(new QRegExpValidator(double_rx,this));
    m_lePWTankHeight->hide();
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + 10);
    rectTmp.setWidth(90);
    m_lbPWHUnit = new QLabel(tmpWidget);
    m_lbPWHUnit->setGeometry(rectTmp);
//    m_lbPWHUnit->setText("M");
    m_lbPWHUnit->hide();
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 20);
    rectTmp.setWidth(X_VALUE_WIDTH);
    m_lePWTankCap = new DLineEdit(tmpWidget);
    m_lePWTankCap->setGeometry(rectTmp);
    m_lePWTankCap->setValidator(new QIntValidator(0, 9999, this));
    m_lePWTankCap->hide();
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + 10);
    rectTmp.setWidth(90);
    m_lbPWCUnit = new QLabel(tmpWidget);
    m_lbPWCUnit->setGeometry(rectTmp);
//    m_lbPWCUnit->setText("L");
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

    /* line 7*/
    tmpWidget = new QWidget(m_widget);

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);

    tmpWidget->setGeometry(QRect(BACKWIDGET_START_X , yOffset, BACKWIDGET_WIDTH ,BACKWIDGET_HEIGHT));
    yOffset += BACKWIDGET_START_HIATUS;

    rectTmp = sQrectAry[0];
    rectTmp.setWidth(BACKWIDGET_ITEM_LENGTH + 60);

    m_lbSWTankName = new QLabel(tmpWidget);
    m_lbSWTankName->setGeometry(rectTmp);
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN);
    rectTmp.setWidth(X_ITEM_WIDTH + 60);
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
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN  + 20);
    rectTmp.setWidth(X_ITEM_WIDTH);
    m_leSWTankHeight = new DLineEdit(tmpWidget);
    m_leSWTankHeight->setGeometry(rectTmp);
    m_leSWTankHeight->setValidator(new QRegExpValidator(double_rx,this));
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + 10);
    rectTmp.setWidth(90);
    m_lbSWHUnit = new QLabel(tmpWidget);
    m_lbSWHUnit->setGeometry(rectTmp);
//    m_lbSWHUnit->setText("M");
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + X_MARGIN + 20);
    rectTmp.setWidth(X_VALUE_WIDTH);
    m_leSWTankCap = new DLineEdit(tmpWidget);
    m_leSWTankCap->setGeometry(rectTmp);
    m_leSWTankCap->setValidator(new QIntValidator(0, 9999, this));
    
    rectTmp.setX(rectTmp.x() + rectTmp.width() + 10);
    rectTmp.setWidth(90);
    m_lbSWCUnit = new QLabel(tmpWidget);
    m_lbSWCUnit->setGeometry(rectTmp);
//    m_lbSWCUnit->setText("L");

    tmpWidget->hide();
    switch(gGlobalParam.iMachineType)
    {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
        tmpWidget->show();
        break;
    }

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SYSCFGPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}


void SystemCfgPage::initUi()
{
    setBackColor();

    createControl();

}

void SystemCfgPage::update()
{
    connectData();
}

void SystemCfgPage::setDeviceType()
{
    QString strTypeName;
    switch(ex_gGlobalParam.Ex_System_Msg.Ex_iCompany)
    {
    case 0:
        strTypeName = setDeviceTypeRephile();
        break;
    case 1:
        strTypeName = setDeviceTypeTypeVWR();
        break;
    default:
        strTypeName = "Unknow";
        break;
    }
    m_lbDeviceType->setText(strTypeName);
}

const QString SystemCfgPage::setDeviceTypeRephile()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        return QString("Super-Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_UP:
        return QString("Super-Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_EDI_LOOP:
        return QString("Super-Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_RO_LOOP:
        return QString("Super-Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_Genie:
        return QString("Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_UP:
        return QString("Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_EDI:
        return QString("Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_RO:
        return QString("Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_PURIST:
        return QString("Genie PURIST");
    case MACHINE_ADAPT:
        return QString("Genie A ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    default:
        return QString("Unknow");
    }
}

const QString SystemCfgPage::setDeviceTypeTypeVWR()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        return QString("Super-Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_UP:
        return QString("Super-Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_EDI_LOOP:
        return QString("Super-Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_L_RO_LOOP:
        return QString("Super-Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_Genie:
        return QString("VWR G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_UP:
        return QString("VWR U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_EDI:
        return QString("VWR E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_RO:
        return QString("VWR R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    case MACHINE_PURIST:
        return QString("VWR P");
    case MACHINE_ADAPT:
        return QString("VWR A ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
    default:
        return QString("Unknow");
    }
}


void SystemCfgPage::connectData()
{
    int iIdx = gGlobalParam.PmParam.aiBuckType[DISP_PM_PM2];

    int iLoop;

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
    
    for (iLoop = 0; iLoop < m_iRealChkNum; iLoop++)
    {
        switch (aCHKsIds[iLoop].iId)
        {
        case DISP_SM_HaveSWValve:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveSWValve))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            break;
        case DISP_SM_ElecLeakProtector:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_ElecLeakProtector))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            break;
        case DISP_SM_Printer:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Printer))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            break;
        case DISP_SM_TubeUV:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            
            break;
        case DISP_SM_TubeDI:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeDI))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            
            break;
        case DISP_SM_HaveTubeFilter:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTubeFilter))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            break;
        case DISP_SM_PreFilterColumn:
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_PreFilterColumn))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }
            break;
        case DISP_SM_HP_Water_Cir:
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }            
            break;
        case DISP_SM_RFID_Authorization:
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }            
            break;
        case DISP_SM_Pre_Filter:
//            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_Pre_Filter))
            if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }  
            break;
        case DISP_SM_H_PACK:
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_H_PACK))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }  
            break;
        case DISP_SM_HP_Electrode:
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Electrode))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }  
            break;
        case DISP_SM_SW_PUMP:
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_SW_PUMP))
            {
                m_aChks[iLoop]->setChecked(true);
            }
            else
            {
                m_aChks[iLoop]->setChecked(false);
            }  
            break;
        }
    }

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        m_chkDeviceTypeTOC->setChecked(true);
    }
    else
    {
        m_chkDeviceTypeTOC->setChecked(false);
    }

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TankUV))
    {
        m_chkPWTankUV->setChecked(true);
    }
    else
    {
        m_chkPWTankUV->setChecked(false);
    }

    m_lePWTankUVValue->setText(QString::number(gGlobalParam.MiscParam.iTankUvOnTime));
    m_leLoginLingerValue->setText(QString::number(gGlobalParam.MiscParam.iAutoLogoutTime));
    m_lePOweronFlushValue->setText(QString::number(gGlobalParam.MiscParam.iPowerOnFlushTime));
}

void SystemCfgPage::save()
{
   DISP_PM_SETTING_STRU          pmParam = gGlobalParam.PmParam;
   DISP_SUB_MODULE_SETTING_STRU  smParam = gGlobalParam.SubModSetting;
   DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;


   pmParam.aiBuckType[DISP_PM_PM2] = m_cmbPWTankVolume->currentIndex();

   if (m_cmbPWTankVolume->isVisible())
   {
       smParam.ulFlags = 1 << DISP_SM_HaveB2;
   
       switch (pmParam.aiBuckType[DISP_PM_PM2])
       {
       case DISP_WATER_BARREL_TYPE_030L:
           pmParam.afCap[DISP_PM_PM2] = 30;
           pmParam.afDepth[DISP_PM_PM2] = 0.3;
           break;
       case DISP_WATER_BARREL_TYPE_060L:
           pmParam.afCap[DISP_PM_PM2] = 60;
           pmParam.afDepth[DISP_PM_PM2] = 0.6;
           break;
       case DISP_WATER_BARREL_TYPE_100L:
           pmParam.afCap[DISP_PM_PM2] = 100;
           pmParam.afDepth[DISP_PM_PM2] = 0.95;
           break;
       case DISP_WATER_BARREL_TYPE_200L:
           pmParam.afCap[DISP_PM_PM2] = 200;
           pmParam.afDepth[DISP_PM_PM2] = 0.95;
           break;
       case DISP_WATER_BARREL_TYPE_350L:
           pmParam.afCap[DISP_PM_PM2] = 350;
           pmParam.afDepth[DISP_PM_PM2] = 0.95;
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
           pmParam.afDepth[DISP_PM_PM3] = 0.3;
           break;
       case DISP_WATER_BARREL_TYPE_060L:
           pmParam.afCap[DISP_PM_PM3] = 60;
           pmParam.afDepth[DISP_PM_PM3] = 0.6;
           break;
       case DISP_WATER_BARREL_TYPE_100L:
           pmParam.afCap[DISP_PM_PM3] = 100;
           pmParam.afDepth[DISP_PM_PM3] = 0.95;
           break;
       case DISP_WATER_BARREL_TYPE_200L:
           pmParam.afCap[DISP_PM_PM3] = 200;
           pmParam.afDepth[DISP_PM_PM3] = 0.95;
           break;
       case DISP_WATER_BARREL_TYPE_350L:
           pmParam.afCap[DISP_PM_PM3] = 350;
           pmParam.afDepth[DISP_PM_PM3] = 0.95;
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
   
   int iLoop;
   for (iLoop = 0; iLoop < m_iRealChkNum; iLoop++)
   {
       if (!m_aChks[iLoop]->isVisible())
        {
           smParam.ulFlags &= ~(1 << aCHKsIds[iLoop].iId);
           continue;
        }
   
       switch (aCHKsIds[iLoop].iId)
       {
       case DISP_SM_HaveSWValve:
           /* other modules */
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_HaveSWValve;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_HaveSWValve);
           }

           break;
       case DISP_SM_ElecLeakProtector:
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_ElecLeakProtector;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_ElecLeakProtector);
           }

           break;
       case DISP_SM_Printer:
           
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_Printer;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_Printer);
           }
           break;
       case DISP_SM_TubeUV:
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_TubeUV;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_TubeUV);
           }
           break;
       case DISP_SM_TubeDI:
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_TubeDI;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_TubeDI);
           }
           break;
       case DISP_SM_HaveTubeFilter:
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_HaveTubeFilter;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_HaveTubeFilter);
           }
           break;
       case DISP_SM_PreFilterColumn:
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               smParam.ulFlags |= 1 << DISP_SM_PreFilterColumn;
           }
           else
           {
               smParam.ulFlags &= ~(1 << DISP_SM_PreFilterColumn);
           }
           break;
       case DISP_SM_HP_Water_Cir:      
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               miscParam.ulMisFlags |= 1 << DISP_SM_HP_Water_Cir;
           }
           else
           {
               miscParam.ulMisFlags &= ~(1 << DISP_SM_HP_Water_Cir);
           }
           break;
       case DISP_SM_RFID_Authorization:          
           if ((Qt::Checked == m_aChks[iLoop]->checkState()))
           {
               miscParam.ulMisFlags |= 1 << DISP_SM_RFID_Authorization;
           }
           else
           {
               miscParam.ulMisFlags &= ~(1 << DISP_SM_RFID_Authorization);
           }
           break; 
       case DISP_SM_Pre_Filter:
            if ((Qt::Checked == m_aChks[iLoop]->checkState()))
            {
//                miscParam.ulMisFlags |= 1 << DISP_SM_Pre_Filter;
                smParam.ulFlags |= 1 << DISP_SM_Pre_Filter;
            }
            else
            {
//                miscParam.ulMisFlags &= ~(1 << DISP_SM_Pre_Filter);
                smParam.ulFlags &= ~(1 << DISP_SM_Pre_Filter);
            }  
            break;
        case DISP_SM_H_PACK:
            if ((Qt::Checked == m_aChks[iLoop]->checkState()))
            {
                miscParam.ulMisFlags |= 1 << DISP_SM_H_PACK;
            }
            else
            {
                miscParam.ulMisFlags &= ~(1 << DISP_SM_H_PACK);
            }  
            break;
        case DISP_SM_HP_Electrode:
            if ((Qt::Checked == m_aChks[iLoop]->checkState()))
            {
                miscParam.ulMisFlags |= 1 << DISP_SM_HP_Electrode;
            }
            else
            {
                miscParam.ulMisFlags &= ~(1 << DISP_SM_HP_Electrode);
            }  
            break;
        case DISP_SM_SW_PUMP:
            if ((Qt::Checked == m_aChks[iLoop]->checkState()))
            {
                miscParam.ulMisFlags |= 1 << DISP_SM_SW_PUMP;
            }
            else
            {
                miscParam.ulMisFlags &= ~(1 << DISP_SM_SW_PUMP);
            }  
            break;
       }
   }

   if (!m_chkDeviceTypeTOC->isVisible())
   {
       smParam.ulFlags &= ~(1 << DISP_SM_HaveTOC);
   }
   else
   {
       if ((Qt::Checked == m_chkDeviceTypeTOC->checkState()))
       {
           smParam.ulFlags |= 1 << DISP_SM_HaveTOC;
       }
       else
       {
           smParam.ulFlags &= ~(1 << DISP_SM_HaveTOC);
       }
   }

   if (!m_chkPWTankUV->isVisible())
   {
       smParam.ulFlags &= ~(1 << DISP_SM_TankUV);
   }
   else
   {
       if ((Qt::Checked == m_chkPWTankUV->checkState()))
       {
           smParam.ulFlags |= 1 << DISP_SM_TankUV;
       }
       else
       {
           smParam.ulFlags &= ~(1 << DISP_SM_TankUV);
       }
   }
   
   miscParam.iTankUvOnTime     = m_lePWTankUVValue->text().toInt();
   miscParam.iAutoLogoutTime   = m_leLoginLingerValue->text().toInt();
   miscParam.iPowerOnFlushTime = m_lePOweronFlushValue->text().toInt();
   
   MainSavePMParam(gGlobalParam.iMachineType,pmParam);
   MainSaveSubModuleSetting(gGlobalParam.iMachineType,smParam);
   MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
   MainUpdateGlobalParam();

   m_wndMain->ClearToc();
   m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_DEVICE_CONFIG);

   show(false);
   m_parent->show(true);
}

void SystemCfgPage::on_CmbIndexChange_pw(int index)
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

void SystemCfgPage::on_CmbIndexChange_sw(int index)
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

void SystemCfgPage::on_btn_clicked(int index)
{

   switch(index)
   {
   case SYSCFGPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }
   
   m_wndMain->prepareKeyStroke();
}






