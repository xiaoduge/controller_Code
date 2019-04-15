#include "maintenancecycle.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "cbitmapbutton.h"

#include <QRect>

#include "maintenancelistwidgtitem.h"
#include "notify.h"

/*static VALIDATE_INFO_STRU vi[MTCNUM]= 
{
    {0,90, 0,10000},
    {0,360,0,30000},
    {0,360,0,6000},
    {0,360,0,6000},
    {0,720,0,8000},
    {0,720,0,8000},
    {0,720,0,8000},
    {0,720,0,8000},
    {0,720,0,8000},
    {0,360,0,0},
    {0,90,0,0},
    {0,180,0,0},
    {0,180,0,0},
    {0,180,0,0},
    {0,30,0,0},
};*/

MaintenanceCyclePage::MaintenanceCyclePage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx   = 0;
    
    m_iRealNum = 0;

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
    {
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_PRE_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
    }
    //2018.10.22 ADD AC PACK
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
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_AC_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    //2018.10.12 add T-Pack
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        aIds[iIdx].iDspType = 1;   //2018.12.17
        aIds[iIdx].iId      = DISP_T_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
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
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_P_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_U_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_AT_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }
    

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_H_PACK;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_L_Genie:
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_N1_UV;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
    case MACHINE_RO:
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
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_N2_UV;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_EDI:
    case MACHINE_RO:
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
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_N3_UV;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iDspType = 2;
        aIds[iIdx].iId      = DISP_N4_UV;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }
    
/*2018/05/21 TOC UV*/
#if 0
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        {
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
            {
                aIds[iIdx].iDspType = 2;
                aIds[iIdx].iId      = DISP_N5_UV;
                aIds[iIdx].vi.v1Min = 0;
                aIds[iIdx].vi.v1Max = 720;
                aIds[iIdx].vi.v2Min = 0;
                aIds[iIdx].vi.v2Max = 8000;
                iIdx++;
            }
        }
        break;
    case MACHINE_EDI:
    case MACHINE_RO:
        break;
    }
#endif

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
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_W_FILTER;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_T_B_FILTER;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 99999;
        iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
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
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_T_A_FILTER;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 0;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_TUBE_FILTER;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 0;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_TUBE_DI;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 0;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
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
        aIds[iIdx].iDspType = 1;
        aIds[iIdx].iId      = DISP_ROC12;
        aIds[iIdx].vi.v1Min = 0;
        aIds[iIdx].vi.v1Max = 99999;
        aIds[iIdx].vi.v2Min = 0;
        aIds[iIdx].vi.v2Max = 0;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
    }
    
    m_iRealNum = iIdx;
    
    creatTitle();

    initUi();
    
    buildTranslation();

}

void MaintenanceCyclePage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void MaintenanceCyclePage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Consumables Life");

    setTitles(stringList);

}

void MaintenanceCyclePage::buildTranslation()
{
    int iIndex = 0;

    for(iIndex = 0 ; iIndex < m_iRealNum ; iIndex++)
    {
        switch(aIds[iIndex].iId)
        {
        case DISP_PRE_PACK:
            /* 
            预处理柱  90天   10000升 
            */
            pMaintenancelistItem[iIndex]->setName(tr("Prefilter"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_AC_PACK:
            /*
            AC PACK  90Day   10000L
            */
            pMaintenancelistItem[iIndex]->setName(tr("AC Pack"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_T_PACK:
            /*
            T-Pack unknow Day; unknow L
            */
            pMaintenancelistItem[iIndex]->setName(tr("T Pack"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            //pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_P_PACK:
            /* 
            P-PACK     360天     30000升 
            */
            pMaintenancelistItem[iIndex]->setName(tr("P PACK"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_U_PACK:
            /* 
            U-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setName(tr("U PACK"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_AT_PACK:
            /* 
            AT-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setName(tr("AT PACK"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("L"));
            break;
        case DISP_H_PACK:
            /* 
            H-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setName(tr("H PACK"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("Litre"));
            break;
        case DISP_N1_UV:
            /* 
            254UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setName(tr("254 UV"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("hr"));
            break;
        case DISP_N2_UV:
            /* 
            185UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setName(tr("185 UV"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("hr"));
            break;
        case DISP_N3_UV:
            /* 
            水箱UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setName(tr("Tank UV"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("hr"));
            break;
        case DISP_N4_UV:
            /* 
            管路UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setName(tr("Tube UV"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("hr"));
            break;
        case DISP_N5_UV:
            /* 
            TOC UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setName(tr("TOC UV"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("Day"));
            pMaintenancelistItem[iIndex]->setP2Name(tr("hr"));
            break;
        case DISP_W_FILTER:
            /* 
            水箱空气过滤器   360天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("Tank Vent Filter"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
        case DISP_T_B_FILTER:
            /* 
            终端过滤器 B   90天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("Final Fliter B"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
        case DISP_T_A_FILTER:
            /* 
            终端过滤器A    180天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("Final Fliter A"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
        case DISP_TUBE_FILTER:
            /* 
            管路过滤器    180天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("Loop Filter"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
        case DISP_TUBE_DI:
            /* 
            管路DI    180天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("Loop DI"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
        case DISP_ROC12:
            /* 
            RO氯清洗    30天     
            */
            pMaintenancelistItem[iIndex]->setName(tr("RO Chlorine Cleaning"));
            pMaintenancelistItem[iIndex]->setP1Name(tr("days"));
            break;
            
        }
    }    

}

void MaintenanceCyclePage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void MaintenanceCyclePage::createList()
{
    /*{2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,};*/

    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};
    
    listWidget = new QListWidget((QWidget *)m_widget);

    listWidget->setStyleSheet("background-color:transparent");
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setFrameShape(QListWidget::NoFrame);

    listWidget->setGeometry(QRect(10,60 ,MTN_LIST_WIDGET_WIDTH,490));

    for(int iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        listWidgetIem[iLoop] = new QListWidgetItem;
        listWidgetIem[iLoop]->setSizeHint(QSize(600 , MTN_LIST_WIDGET_HEIGHT));
        listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        pMaintenancelistItem[iLoop] = new MaintenanceListWidgtItem(NULL,aIds[iLoop].iDspType);
        pMaintenancelistItem[iLoop]->setP1Range(aIds[iLoop].vi.v1Min,aIds[iLoop].vi.v1Max);
        pMaintenancelistItem[iLoop]->setP2Range(aIds[iLoop].vi.v2Min,aIds[iLoop].vi.v2Max);

        listWidget->insertItem(iLoop,listWidgetIem[iLoop]);
        listWidget->setItemWidget(listWidgetIem[iLoop] , pMaintenancelistItem[iLoop]);
        
    }
    

    listWidget->setStyleSheet(
                 "QListWidget{background-color:transparent;}"  
                 "QListWidget::item:!selected{}"  
                 "QListWidget::item:selected:active{background:#F2F2F2;color:#19649F;}"  
                 "QListWidget::item:selected{background:#F2F2F2;color:#19649F;}" 
                 );
    
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem*)));


}


void MaintenanceCyclePage::setBackColor()
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



void MaintenanceCyclePage::initUi()
{

    setBackColor();

    createList();

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,0);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");

    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);

    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}

void MaintenanceCyclePage:: update()
{
    int iIndex = 0;

    for(iIndex = 0 ; iIndex < m_iRealNum ; iIndex++)
    {
        switch(aIds[iIndex].iId)
        {
        case DISP_PRE_PACK:
            /* 
            预处理柱  90天   10000升 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEL]));
            break;
        case DISP_AC_PACK:
            /*
            AC PACK 90DAY  10000L
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEL]));
            break;
        case DISP_T_PACK:
            /*
            T-Pack
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY]));
            //pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEL]));
            break;
        case DISP_P_PACK:
            /* 
            P-PACK     360天     30000升 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEL]));
            break;
        case DISP_U_PACK:
            /* 
            U-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEL]));
            break;
        case DISP_AT_PACK:
            /* 
            AT-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEL]));
            break;
        case DISP_H_PACK:
            /* 
            H-PACK     360天     6000升 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEL]));
            break;
        case DISP_N1_UV:
            /* 
            254UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEHOUR]));
            break;
        case DISP_N2_UV:
            /* 
            185UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEHOUR]));

            break;
        case DISP_N3_UV:
            /* 
            水箱UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEHOUR]));

            break;
        case DISP_N4_UV:
            /* 
            管路UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEHOUR]));

            break;
        case DISP_N5_UV:
            /* 
            TOC UV    720天     8000小时 
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY]));
            pMaintenancelistItem[iIndex]->setP2(QString::number(gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEHOUR]));

            break;
        case DISP_W_FILTER:
            /* 
            水箱空气过滤器   360天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE]));

            break;
        case DISP_T_B_FILTER:
            /* 
            终端过滤器 B   90天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE]));

            break;
        case DISP_T_A_FILTER:
            /* 
            终端过滤器A    180天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE]));

            break;
        case DISP_TUBE_FILTER:
            /* 
            管路过滤器    180天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE]));

            break;
        case DISP_TUBE_DI:
            /* 
            管路DI    180天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE]));
            break;
        case DISP_ROC12:
            /* 
            RO氯清洗    30天     
            */
            pMaintenancelistItem[iIndex]->setP1(QString::number(gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY]));
            break;
            
        }
    }    

}

void MaintenanceCyclePage::ItemClicked(QListWidgetItem * pItem)
{
    QListWidget *pTmpListWidget = (QListWidget *)this->sender();
    
    int iIndex = pTmpListWidget->row(pItem);

    //m_wndMain->showOptDlg(true);

    printf("iIndex %d\r\n",iIndex);

}

void MaintenanceCyclePage::save()
{
    int iIndex = 0;

    int iTemp;

    DISP_CONSUME_MATERIAL_STRU  CMParam = gGlobalParam.CMParam;      

    for(iIndex = 0 ; iIndex < m_iRealNum ; iIndex++)
    {
        switch(aIds[iIndex].iId)
        {
        case DISP_PRE_PACK:
            /* 
            预处理柱  90天   10000升 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_PRE_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_PRE_PACKLIFEL] = iTemp;
            break;
        case DISP_AC_PACK:
            /*
            AC PACK 90DAY 10000L
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_AC_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_AC_PACKLIFEL] = iTemp;
            break;
        case DISP_T_PACK:
            /*
            T-Pack
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_T_PACKLIFEDAY] = iTemp;
      //      iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
      //      CMParam.aulCms[DISP_T_PACKLIFEL] = iTemp;
            break;
        case DISP_P_PACK:
            /* 
            P-PACK     360天     30000升 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_P_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_P_PACKLIFEL] = iTemp;
            break;
        case DISP_U_PACK:
            /* 
            U-PACK     360天     6000升 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_U_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_U_PACKLIFEL] = iTemp;
            break;
        case DISP_AT_PACK:
            /* 
            AT-PACK     360天     6000升 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_AT_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_AT_PACKLIFEL] = iTemp;
            break;
        case DISP_H_PACK:
            /* 
            H-PACK     360天     6000升 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_H_PACKLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_H_PACKLIFEL] = iTemp;
            break;
        case DISP_N1_UV:
            /* 
            254UV    720天     8000小时 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_N1_UVLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_N1_UVLIFEHOUR] = iTemp;
            break;
        case DISP_N2_UV:
            /* 
            185V    720天     8000小时 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_N2_UVLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_N2_UVLIFEHOUR] = iTemp;
            break;
        case DISP_N3_UV:
            /* 
            水箱UV    720天     8000小时 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_N3_UVLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_N3_UVLIFEHOUR] = iTemp;
            break;
        case DISP_N4_UV:
            /* 
            管路UV    720天     8000小时 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_N4_UVLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_N4_UVLIFEHOUR] = iTemp;
            break;
        case DISP_N5_UV:
            /* 
            TOC UV    720天     8000小时 
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_N5_UVLIFEDAY] = iTemp;
            iTemp = pMaintenancelistItem[iIndex]->getP2().toInt();
            CMParam.aulCms[DISP_N5_UVLIFEHOUR] = iTemp;
            break;
        case DISP_W_FILTER:
            /* 
            水箱空气过滤器   360天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_W_FILTERLIFE] = iTemp;
            break;
        case DISP_T_B_FILTER:
            /* 
            终端过滤器 B   90天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_T_B_FILTERLIFE] = iTemp;
            break;
        case DISP_T_A_FILTER:
            /* 
            终端过滤器A    180天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_T_A_FILTERLIFE] = iTemp;

            break;
        case DISP_TUBE_FILTER:
            /* 
            管路过滤器    180天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_TUBE_FILTERLIFE] = iTemp;

            break;
        case DISP_TUBE_DI:
            /* 
            管路DI    180天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_TUBE_DI_LIFE] = iTemp;
            break;
        case DISP_ROC12:
            /* 
            RO氯清洗    30天     
            */
            iTemp = pMaintenancelistItem[iIndex]->getP1().toInt();
            CMParam.aulCms[DISP_ROC12LIFEDAY] = iTemp;
            break;
        }
    }    


    MainSaveCMParam(gGlobalParam.iMachineType,CMParam);

    MainUpdateSpecificParam(NOT_PARAM_CM_PARAM);

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_MAINTAIN_PERIOD);

    show(false);
    m_parent->show(true);
}


void MaintenanceCyclePage::on_btn_clicked(int index)
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

