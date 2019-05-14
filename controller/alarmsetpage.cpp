#include "alarmsetpage.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>
#include "Display.h"
#include "notify.h"

#define MAKEID(major,minor) (((major)<<16) + (minor))

AlarmSetPage::AlarmSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;
    int iLoop;
    int iMask;

    m_iRealNum = 0;
    
    iMask = m_wndMain->getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART0);
    
    for (iLoop = 0; iLoop < DISP_ALARM_PART0_NUM; iLoop++)
    {
       if (iMask & (1 << iLoop))
       {
            aIds[iIdx++].iId = MAKEID(DISP_ALARM_PART0,iLoop);
       }
    }
    
    iMask = m_wndMain->getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART1);
    
    for (iLoop = 0; iLoop < DISP_ALARM_PART1_NUM; iLoop++)
    {
       if (iMask & (1 << iLoop))
       {
            aIds[iIdx++].iId = MAKEID(DISP_ALARM_PART1,iLoop);
       }
    }
    
    m_iRealNum = iIdx;
    
    creatTitle();

    initUi();

    buildTranslation();

}

void AlarmSetPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void AlarmSetPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Alarm Config");

    setTitles(stringList);

}

void AlarmSetPage::buildTranslation()
{
    int iIdx = 0;

    /* refer : QString gastrAlarmName */

    m_pBtnSave->setTip(tr("save"));

    for (iIdx = 0; iIdx < m_iRealNum; iIdx++)
    {
        switch(aIds[iIdx].iId)
        {
        
        /* Part One */
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP):
            /*
            检查254UV
            */
            m_plistItem[iIdx]->setName(tr("Check 254 UV"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP):
            /*
            检查185UV
            */
            m_plistItem[iIdx]->setName(tr("Check 185 UV"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP):
            /*
            检查水箱UV
            */
            m_plistItem[iIdx]->setName(tr("Check Tank UV"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TUBEUV_OOP):
            /*
            检查管路UV
            */
            m_plistItem[iIdx]->setName(tr("Check Loop UV"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PREPACK_OOP):
            /*
            PRE-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("PRE Pack Not Detected"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ACPACK_OOP):
            /*
            AC-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("AC Pack Not Detected"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP):
            /*
            P-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("P Pack Not Detected"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP):
            /*
            AT-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("AT Pack Not Detected"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP):
            /*
            H-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("H Pack Not Detected"));
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP):
            /*
            U-Pack脱落
            */
            m_plistItem[iIdx]->setName(tr("U Pack Not Detected"));
            break;
            /* Part Two */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE):
            /*
            自来水压力低
            */
            m_plistItem[iIdx]->setName(tr("Low Tap Pressure"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY):
            /*
            自来水电导率>设定值
            */
            m_plistItem[iIdx]->setName(tr("High Tap Conductivity"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY):
            /*
            RO产水>设定值
            */
            
            if (MACHINE_PURIST == gGlobalParam.iMachineType)
            {
                m_plistItem[iIdx]->setName(tr("High UP Feed Cond."));
            }
            else
            {
                //High RO Conductivity
                m_plistItem[iIdx]->setName(tr("High RO Conductivity"));
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO):
            
            /*
            RO截留率<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low RO Rejection")); //ex
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE):
            /*
            EDI产水<设定值
            */
            //Low EDI Resistivity
            m_plistItem[iIdx]->setName(tr("Low EDI Resistivity"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE):
            /*
            UP取水<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low UP Resistivity"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE):
            /*
            管路水质<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low Loop Resistivity"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL):
            /*
            纯水箱液位<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low Pure Tank Level"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL):
            /*
            源水箱液位<设定值 
            */
            m_plistItem[iIdx]->setName(tr("Low Feed Tank Level"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY):
            /*
            RO产水流速<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low RO Product Rate"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY):
            /*
            RO弃水流速<设定值
            */
            m_plistItem[iIdx]->setName(tr("Low RO Reject Rate"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY):
             /*
             循环水质<设定值 
             */
             m_plistItem[iIdx]->setName(tr("Low Recir. Resis."));
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY):
             /*
             HP产水<设定值 
             */
             m_plistItem[iIdx]->setName(tr("Low HP Prouduce Cond."));
             break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE):
            /*
            水箱水质 <设定值
            */
            m_plistItem[iIdx]->setName(tr("Low Pure Tank Water Resis."));
            break;
            /*
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE):
            //纯水取水<设定值
            m_plistItem[iIdx]->setName(tr("Low Feed Product Resis."));
            break;
            */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE):
            /*
            进水温度>45℃
            */
            m_plistItem[iIdx]->setName(tr("High RO Feed Temp."));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE):
           /*
           进水温度<5℃
           */
           m_plistItem[iIdx]->setName(tr("Low RO Feed Temp."));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE):
             /*
             RO产水温度>45℃
             */
             
             if (MACHINE_PURIST == gGlobalParam.iMachineType)
             {
                  m_plistItem[iIdx]->setName(tr("High UP Feed Temp."));
             }
             else
             {
                 m_plistItem[iIdx]->setName(tr("High RO Temperature"));
             }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE):
            /*
            RO产水温度<5℃
            */
            if (MACHINE_PURIST == gGlobalParam.iMachineType)
            {
                m_plistItem[iIdx]->setName(tr("Low UP Feed Temp."));
            }
            else
            {
                m_plistItem[iIdx]->setName(tr("Low RO Temperature"));
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE):
            /*
            EDI产水温度>45℃
            */
            m_plistItem[iIdx]->setName(tr("High EDI Temperature"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE):
            /*
            EDI产水温度<5℃
            */
            m_plistItem[iIdx]->setName(tr("Low EDI Temperature"));
            
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE):
            /*
            UP产水温度>45℃
            */
            m_plistItem[iIdx]->setName(tr("High UP Temperature"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE):
            /*
            UP产水温度<5℃
            */
            m_plistItem[iIdx]->setName(tr("Low UP Temperature"));
            
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE):
            /*
            管路温度>45℃
            */
            m_plistItem[iIdx]->setName(tr("High Loop Temperature"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE):
            /*
            管路温度<5℃
            */
            m_plistItem[iIdx]->setName(tr("Low Loop Temperature"));
            
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE):
            /*
            TOC温度>45℃
            */
            m_plistItem[iIdx]->setName(tr("High TOC Temperature"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE):
            /*
            TOC温度<5℃
            */
            m_plistItem[iIdx]->setName(tr("Low TOC Temperature"));
            
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE):            
            /*
            TOC进水水质<15MΩ。cm
            */
            m_plistItem[iIdx]->setName(tr("High TOC Feed Conductivity"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW):
            /*
            Leak or Tank Overflow
            */
            m_plistItem[iIdx]->setName(tr("Leak or Tank Overflow"));
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGH_WORK_PRESSURE):
            /*
            Work pressure max
            */
            m_plistItem[iIdx]->setName(tr("High Work Pressure"));
            break;
        }
    }

}

void AlarmSetPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void AlarmSetPage::createList()
{

    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};

    m_plistWidget = new QListWidget((QWidget *)m_widget);

    m_plistWidget->setStyleSheet("background-color:transparent");
    m_plistWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_plistWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_plistWidget->setFrameShape(QListWidget::NoFrame);

    m_plistWidget->setGeometry(QRect(10,55 ,ALARM_SET_LIST_WIDGET_WIDTH,500));

    for(int iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        m_listWidgetIem[iLoop] = new QListWidgetItem;
        m_listWidgetIem[iLoop]->setSizeHint(QSize(600 , ALARM_SET_LIST_WIDGET_HEIGHT));

        m_plistItem[iLoop] = new AlarmSetListwidgtItem(m_widget,this,0,iLoop);

        m_listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        m_plistWidget->insertItem(iLoop,m_listWidgetIem[iLoop]);

        m_plistWidget->setItemWidget(m_listWidgetIem[iLoop] , m_plistItem[iLoop]);

    }

    m_plistWidget->setStyleSheet(
                 "QListWidget{background-color:transparent;}"  
                 "QListWidget::item:!selected{}"  
                 "QListWidget::item:selected:active{background:#F2F2F2;color:#19649F;}"  
                 "QListWidget::item:selected{background:#F2F2F2;color:#19649F;}" 
                 );
    
    connect(m_plistWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem*)));

}

void AlarmSetPage::setBackColor()
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



void AlarmSetPage::initUi()
{

    setBackColor();

    createList();

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,0XFF);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();
    
}


void AlarmSetPage::ItemClicked(QListWidgetItem * item)
{
#if 0
    QListWidget *pTmpListWidget = (QListWidget *)this->sender();
    
    int iIndex = pTmpListWidget->row(item);

    switch (m_plistItem[iIndex]->getType())
    {
    case ALARMSET_ITEM_TYPE_SWITCH:
        {
            //int state ;
            
            m_plistItem[iIndex]->toggleSwitchState();
            //state = m_plistItem[iIndex]->getSwitchState();

        }
        break;
    default:
        break;
    }

    //m_wndMain->showOptDlg(true);

    printf("list index %d\r\n",iIndex);
#endif
}

void AlarmSetPage::on_btn_clicked(int iIndex)
{
    if (iIndex < ALARMSETNUM)
    {
        switch (m_plistItem[iIndex]->getType())
        {
        case ALARMSET_ITEM_TYPE_SWITCH:
            {
                //int state ;
                //m_plistItem[iIndex]->toggleSwitchState();
                //state = m_plistItem[iIndex]->getSwitchState();

            }
            break;
        default:
            break;
        }
        printf("btn index %d\r\n",iIndex);
        return;
    }

    if (0XFF == iIndex)
    {
        save();
    }
    m_wndMain->prepareKeyStroke();
}

void AlarmSetPage::save()
{
    int iIdx = 0;
    DISP_ALARM_SETTING_STRU Param = gGlobalParam.AlarmSettting;

    for (iIdx = 0; iIdx < m_iRealNum; iIdx++)
    {
        switch(aIds[iIdx].iId)
        {
        
        /* Part One */
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP):
            /* for part1 */
            
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_254UV_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_254UV_OOP);
            }   
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_185UV_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_185UV_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_TANKUV_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_TANKUV_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TUBEUV_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_TUBEUV_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_TUBEUV_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PREPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_PREPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_PREPACK_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ACPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_ACPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_ACPACK_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_PPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_PPACK_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_ATPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_ATPACK_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_HPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_HPACK_OOP);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART0] |= (1<<DISP_ALARM_PART0_UPACK_OOP);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART0] &= ~(1<<DISP_ALARM_PART0_UPACK_OOP);
            }
            break;
            
            /* Part Two */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY):
             /*
             循环水质<设定值 
             */
             if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
             {
                 Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
             }
             else
             {
                 Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
             }
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY):
             /*
             HP产水<设定值 
             */
             if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
             {
                 Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
             }
             else
             {
                 Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
             }
             break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE);
            }
            break;
            /*
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE);
            }
            break;
            */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE);
            }            
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE):
             if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE):            
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGH_WORK_PRESSURE):
            if (BITMAPBUTTON_STATE_SEL == m_plistItem[iIdx]->getSwitchState())
            {
                Param.aulFlag[DISP_ALARM_PART1] |= (1<<DISP_ALARM_PART1_HIGH_WORK_PRESSURE);
            }
            else
            {
                Param.aulFlag[DISP_ALARM_PART1] &= ~(1<<DISP_ALARM_PART1_HIGH_WORK_PRESSURE);
            }
            iIdx++;
            break;

        }
    }    

    MainSaveAlarmSetting(gGlobalParam.iMachineType,Param);

    MainUpdateSpecificParam(NOT_PARAM_ALARM_PARAM);
    
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_ALARM_SETTING);
    
    show(false);
    m_parent->show(true);

}

void AlarmSetPage::update()
{
   int iIdx = 0;
   
   DISP_ALARM_SETTING_STRU *pParam = &gGlobalParam.AlarmSettting;

   for (iIdx = 0; iIdx < m_iRealNum; iIdx++)
   {
       /* for part1 */
       switch(aIds[iIdx].iId)
       { 
        /* Part One */
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP): 
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_254UV_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }           
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_185UV_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }       
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_TANKUV_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TUBEUV_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_TUBEUV_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
       case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PREPACK_OOP):
           if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_PREPACK_OOP))
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
           }
           else
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
           }
           break;
       case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ACPACK_OOP):
           if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_ACPACK_OOP))
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
           }
           else
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
           }
           break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_PPACK_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_ATPACK_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_HPACK_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP):
            if (pParam->aulFlag[DISP_ALARM_PART0] & (1<<DISP_ALARM_PART0_UPACK_OOP))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
            
            /* Part Two */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY):
             /*
             循环水质<设定值 
             */
             if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY))
             {
                 m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
             }
             else
             {
                 m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
             }
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY):
             /*
             HP产水<设定值 
             */
             if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY))
             {
                 m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
             }
             else
             {
                 m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
             }
             break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
            /*
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
            */
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE):
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;
        case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE): 
            if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE))
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
            }
            else
            {
                m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
            }
            break;

       case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW):
           if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW))
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
           }
           else
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
           }
           break;
       case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGH_WORK_PRESSURE):
           if (pParam->aulFlag[DISP_ALARM_PART1] & (1<<DISP_ALARM_PART1_HIGH_WORK_PRESSURE))
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_SEL);
           }
           else
           {
               m_plistItem[iIdx]->setSwitchState(BITMAPBUTTON_STATE_UNSEL);
           }
           break;
        }
   }      
}
