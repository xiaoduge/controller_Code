#include "parametercalibration.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

ParameterCalibrationPage::ParameterCalibrationPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;

    switch(gGlobalParam.iMachineType)/*进水电导率*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type = PARAMETER_CALIBRATION_FORMAT0;
        aParameters[iIdx].id   = DISP_PC_COFF_SOURCE_WATER_CONDUCT;         
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*进水温度*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_SOURCE_WATER_TEMP;           
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*RO电导率*/
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
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT0;
        aParameters[iIdx].id    = DISP_PC_COFF_RO_WATER_CONDUCT;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*RO温度*/
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
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_RO_WATER_TEMP;           
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
    }

    switch(gGlobalParam.iMachineType)/*EDI电阻率*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT0;
        aParameters[iIdx].id    = DISP_PC_COFF_EDI_WATER_CONDUCT;           
        iIdx++;
        break;
    default:
        break;
    }    

    switch(gGlobalParam.iMachineType)/*EDI温度*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_EDI_WATER_TEMP;          
        iIdx++;
        break;
    default:
        break;
    } 

    switch(gGlobalParam.iMachineType)/*UP电阻率*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT0;
        aParameters[iIdx].id    = DISP_PC_COFF_UP_WATER_CONDUCT;            
        iIdx++;
        break;
    default:
        break;
    } 

    switch(gGlobalParam.iMachineType)/*UP温度*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id   = DISP_PC_COFF_UP_WATER_TEMP;             
        iIdx++;
        break;
    default:
        break;
    } 

    switch(gGlobalParam.iMachineType)/*TOC电极*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT0;
        aParameters[iIdx].id    = DISP_PC_COFF_TOC_WATER_CONDUCT;           
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*TOC温度*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_TOC_WATER_TEMP;          
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*S1*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_S1;          
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*S2*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_S2;          
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*S3*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aParameters[iIdx].type = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id   = DISP_PC_COFF_S3;           
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*S4*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_S4;          
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*纯水液位矫正*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_PW_TANK_LEVEL;           
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*原水液位矫正*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_SW_TANK_LEVEL;           
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*系统压力矫正*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aParameters[iIdx].type  = PARAMETER_CALIBRATION_FORMAT1;
        aParameters[iIdx].id    = DISP_PC_COFF_SYS_PRESSURE;            
        iIdx++;
        break;
    }


    m_iRealItems = iIdx;


    creatTitle();

    initUi();

    buildTranslation();

}

void ParameterCalibrationPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void ParameterCalibrationPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Calibration");

    setTitles(stringList);

}


void ParameterCalibrationPage::buildTranslation()
{
    int iLoop;
  
    for (iLoop = 0; iLoop < m_iRealItems; iLoop++)
    {
        switch(aParameters[iLoop].id)
        {
        case DISP_PC_COFF_SOURCE_WATER_CONDUCT:
            /*
            进水电导率     K: 1.000  C:1.000   500μS/cm  
            */
            m_aParameterlistItem[iLoop]->setName(tr("Tap Cond."));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));  //K:
            m_aParameterlistItem[iLoop]->setP2Name(tr("C:"));
            m_aParameterlistItem[iLoop]->setP3Name(tr("us/cm"));
            break;
        case DISP_PC_COFF_SOURCE_WATER_TEMP:
            /*
            进水温度    K:1.000    25.0℃
            */
            m_aParameterlistItem[iLoop]->setName(tr("Tap Temp."));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("celsius"));
            break;
        case DISP_PC_COFF_RO_WATER_CONDUCT:
            /*
            RO电导率    K: 1.000  C:0.100   20μS/cm      
            */
            m_aParameterlistItem[iLoop]->setName(tr("RO Cond."));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP2Name(tr("C:"));
            m_aParameterlistItem[iLoop]->setP3Name(tr("us/cm"));
            break;
        case DISP_PC_COFF_RO_WATER_TEMP:
            /*
            RO温度    K: 1.000    25.0℃
            */
            m_aParameterlistItem[iLoop]->setName(tr("RO Temp."));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("celsius"));
            break;
        case DISP_PC_COFF_EDI_WATER_CONDUCT:
            /*
            EDI电阻率    K:1.000 C:0.01   15.0MΩ.cm
            */
            m_aParameterlistItem[iLoop]->setName(tr("EDI Conduct"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP2Name(tr("C:"));
            m_aParameterlistItem[iLoop]->setP3Name(tr("omg"));
            break;
        case DISP_PC_COFF_EDI_WATER_TEMP:
            /*
            EDI温度     K:1.000     25.0℃
            */
            m_aParameterlistItem[iLoop]->setName(tr("EDI Temp"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("celsius"));
            break;
        case DISP_PC_COFF_UP_WATER_CONDUCT:
            /*
            UP电阻率    K:1.000 C:0.01   18.2MΩ.cm
            */
            m_aParameterlistItem[iLoop]->setName(tr("UP Conduct"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP2Name(tr("C:"));
            m_aParameterlistItem[iLoop]->setP3Name(tr("omage"));
            break;
        case DISP_PC_COFF_UP_WATER_TEMP:
            /*
            UP温度    K: 1.000    25.0℃
            */
            m_aParameterlistItem[iLoop]->setName(tr("UP Temp"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("celsius"));
            break;
        case DISP_PC_COFF_TOC_WATER_CONDUCT:
            /*
            TOC电极    K:1.000 C:0.01   5ppb
            */
            m_aParameterlistItem[iLoop]->setName(tr("TOC Conduct"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP2Name(tr("C:"));
            m_aParameterlistItem[iLoop]->setP3Name(tr("ppb"));
            break;
        case DISP_PC_COFF_TOC_WATER_TEMP:
            /*
            TOC温度    K: 1.000    25.0℃
            */
            m_aParameterlistItem[iLoop]->setName(tr("TOC Temp"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("celsius"));
            break;
        case DISP_PC_COFF_S1:
            /*
            取水流量计S1  1.000 1.8L/min
            */
            m_aParameterlistItem[iLoop]->setName(tr("UP Rate"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("L/Min"));
            break;
        case DISP_PC_COFF_S2:
            /*
            RO进水流量计S2   1.000 30.0l/min
            */
            m_aParameterlistItem[iLoop]->setName(tr("RO Feed. Rate"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("L/Min"));
            break;
        case DISP_PC_COFF_S3:
            /*
            RO产水流量计S3   1.000 20.0l/min
            */
            m_aParameterlistItem[iLoop]->setName(tr("RO Prod. Rate"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("L/Min"));
            break;
        case DISP_PC_COFF_S4:
            /*
            RO废水流量计S4   1.000 10.0l/min
            */
            m_aParameterlistItem[iLoop]->setName(tr("RO Drain Rate"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("L/Min"));
            break;
        case DISP_PC_COFF_PW_TANK_LEVEL:
            /*
            纯水液位校正    1.000  0.80m 
            */
            m_aParameterlistItem[iLoop]->setName(tr("Pure Tank Level"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("m"));
            break;
        case DISP_PC_COFF_SW_TANK_LEVEL:
            /*
            原水液位校正      1.000  0.80m 
            */
            m_aParameterlistItem[iLoop]->setName(tr("Feed Tank Level"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("m"));
            break;
        case DISP_PC_COFF_SYS_PRESSURE:
            /*
            系统压力校正       1.000  4.0bar
            */
            m_aParameterlistItem[iLoop]->setName(tr("Pres. Sensor"));
            m_aParameterlistItem[iLoop]->setP1Name(tr(""));
            m_aParameterlistItem[iLoop]->setP3Name(tr("bar"));
            break;
        }
    }
}

void ParameterCalibrationPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void ParameterCalibrationPage::Create_list()
{
    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};

    listWidget = new QListWidget((QWidget *)m_widget);

    listWidget->setStyleSheet("background-color:transparent");
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setFrameShape(QListWidget::NoFrame);

    listWidget->setGeometry(QRect(10,60 ,PC_LIST_WIDGET_WIDTH,490));

    for(int iLoop = 0 ; iLoop < m_iRealItems ; iLoop++)
    {
        listWidgetIem[iLoop]      = new QListWidgetItem;
        listWidgetIem[iLoop]->setSizeHint(QSize(PC_LIST_WIDGET_WIDTH , PC_LIST_WIDGET_HEIGHT));
        listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        m_aParameterlistItem[iLoop] = new Parameterlistwidgtitem(NULL,aParameters[iLoop].type);//

        listWidget->insertItem(iLoop,listWidgetIem[iLoop]);

        listWidget->setItemWidget(listWidgetIem[iLoop] , m_aParameterlistItem[iLoop]);

    }
    
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem *)));
}

void ParameterCalibrationPage::Set_Back_Color()
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

void ParameterCalibrationPage::initUi()
{

    Set_Back_Color();

    Create_list();
    
    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,0);

    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);

    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);

    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());

    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    m_pBtnSave->show();
}


void ParameterCalibrationPage::update()
{
    int iIdx = 0;

    for (iIdx = 0; iIdx < m_iRealItems; iIdx++) //DISP_PC_COFF_NUM  m_iRealItems
    {
        m_aParameterlistItem[iIdx]->setP1(QString::number(gGlobalParam.Caliparam.pc[iIdx].fk,'f',3));
        m_aParameterlistItem[iIdx]->setP2(QString::number(gGlobalParam.Caliparam.pc[iIdx].fc,'f',3));
        m_aParameterlistItem[iIdx]->setP3(QString::number(gGlobalParam.Caliparam.pc[iIdx].fv,'f',3));
    }
    
}

void ParameterCalibrationPage::ItemClicked(QListWidgetItem * item)
{

}

void ParameterCalibrationPage::save()
{
    int iIdx = 0;

    float fTemp;

    DISP_PARAM_CALI_STRU  pcParam = gGlobalParam.Caliparam;  

    for (iIdx = 0; iIdx < m_iRealItems; iIdx++)
    {
        fTemp = m_aParameterlistItem[iIdx]->getP1().toFloat();
        pcParam.pc[iIdx].fk = fTemp;
        fTemp = m_aParameterlistItem[iIdx]->getP2().toFloat();
        pcParam.pc[iIdx].fc = fTemp;
        fTemp = m_aParameterlistItem[iIdx]->getP3().toFloat();
        pcParam.pc[iIdx].fv = fTemp;
    }
    
    MainSaveCalibrateParam(gGlobalParam.iMachineType,pcParam);
    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_PARAMETER_CALIBRATE);

    show(false);
    m_parent->show(true);

}

void ParameterCalibrationPage::on_btn_clicked(int index)
{
    switch(index)
    {
    case 0:
        {
             /* save parameter */
             save();
        }
        break;
    }
    
    m_wndMain->prepareKeyStroke();
}

