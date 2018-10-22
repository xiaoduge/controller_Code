#include "ConsumableinsPage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

ConsumableInsPage::ConsumableInsPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;
    
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)) //DISP_SM_PreFilterColumn
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_PRE_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_PREPACK;
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
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_AC_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
     }

    //2018.10.12 add T PACK
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_P_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
     }
    
     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_EDI_LOOP:
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_AT_PACK;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
         iIdx++;
         break;
     }
    
     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_UP:  
     case MACHINE_L_RO_LOOP:
     //case MACHINE_Genie:
     case MACHINE_UP:
     case MACHINE_EDI:
     case MACHINE_RO:
     case MACHINE_PURIST:
     case MACHINE_ADAPT:
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_H_PACK;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
         iIdx++;
         break;
     case MACHINE_L_EDI_LOOP:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_H_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
        iIdx++;
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
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_U_PACK;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
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
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_N1_UV;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_N2_UV;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_N3_UV;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
         iIdx++;
         break;
     }
     //0628
     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
     case MACHINE_UP:
     case MACHINE_EDI:
     case MACHINE_RO:
     {
         if (gGlobalParam.SubModSetting.ulFlags & (1 <<DISP_SM_TubeUV))
         {
             aIds[iIdx].iType = 0;
             aIds[iIdx].iId   = DISP_N4_UV;
             aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
             iIdx++;
         }
         break;
     }
     case MACHINE_Genie:
     case MACHINE_PURIST:
         break;
     }

/*2018/05/21 TOC UV*/
#if 0
     switch(gGlobalParam.iMachineType) /*TOC UV E9*/
     {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_Genie:
         {
             if (gGlobalParam.SubModSetting.ulFlags &(1 << DISP_SM_HaveTOC))
            {
                 aIds[iIdx].iType = 0;
                 aIds[iIdx].iId   = DISP_N5_UV;
                 aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
                 iIdx++;
            }
         }
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
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_W_FILTER;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
         iIdx++;
         break;
     }
    
     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_Genie:
     case MACHINE_UP:
     case MACHINE_ADAPT:
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_T_B_FILTER;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
     case MACHINE_PURIST:
     case MACHINE_ADAPT:
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_T_A_FILTER;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
         iIdx++;
         break;
     }
     //0628
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:

    {
        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_HaveTubeFilter)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_FILTER;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }

        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_TubeDI)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_DI;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        }
        break;
    }
    case MACHINE_Genie:
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
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_SOURCE_BOOSTER_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_TUBE_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
         aIds[iIdx].iType = 1; //0
         aIds[iIdx].iId   = DISP_MACHINERY_RO_MEMBRANE;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
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
         aIds[iIdx].iType = 1; //0
         aIds[iIdx].iId   = DISP_MACHINERY_RO_BOOSTER_PUMP;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
         iIdx++;
         break;
     }

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_Genie:
     case MACHINE_EDI:
         aIds[iIdx].iType = 1; //0
         aIds[iIdx].iId   = DISP_MACHINERY_EDI;
         aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
         iIdx++;
         break;
     }     
     
    m_iRealItemNum = iIdx;

    creatTitle();

    initUi();

    buildTranslation();

}

void ConsumableInsPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void ConsumableInsPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Components Installation");

    setTitles(stringList);

}

void ConsumableInsPage::buildTranslation()
{
    int iIdx = 0;

    int iMapIdx = 0;

    int iEscape = 0;

    for (iIdx = 0; iIdx < m_iRealItemNum; iIdx++)
    {
       iEscape = 0;
       switch(aIds[iIdx].iId)
       {
       case DISP_PRE_PACK:
            /*
            预处理柱     
            */
            m_aInsListItem[iMapIdx]->setName(tr("Prefilter"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
       case DISP_AC_PACK:
            /*
            AC PACK
            */
            m_aInsListItem[iMapIdx]->setName(tr("AC Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
       case DISP_T_PACK:
            /*
            T Pack
            */
            m_aInsListItem[iMapIdx]->setName(tr("T Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
       case DISP_P_PACK:
           /*
           P-Pack  
           */
           m_aInsListItem[iMapIdx]->setName(tr("P Pack"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_U_PACK:
           /*
           U-Pack  
           */
           m_aInsListItem[iMapIdx]->setName(tr("U Pack"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_AT_PACK:
           /*
           AT-Pack  
           */
           m_aInsListItem[iMapIdx]->setName(tr("AT Pack"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_H_PACK:
           /*
           H-Pack  
           */
           m_aInsListItem[iMapIdx]->setName(tr("H Pack"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_N1_UV:
           /*
           254UV 
           */
           m_aInsListItem[iMapIdx]->setName(tr("254 UV Lamp"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_N2_UV:
           /*
           185UV       
           */
           m_aInsListItem[iMapIdx]->setName(tr("185 UV Lamp"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_N3_UV:
           /*
           水箱UV       
           */
           m_aInsListItem[iMapIdx]->setName(tr("Tank UV Lamp"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_N4_UV:
           /*
           管路UV       
           */
           m_aInsListItem[iMapIdx]->setName(tr("TUBE UV"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_N5_UV:
           /*
           TOC UV       
           */
           m_aInsListItem[iMapIdx]->setName(tr("TOC UV"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_W_FILTER:
           /*
           空气过滤器       
           */
           if ((gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
           {
               m_aInsListItem[iMapIdx]->setName(tr("Tank Vent Filter"));
               m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           }
           else
           {
               iEscape = 1;
           }
           break;
       case DISP_T_B_FILTER:
           /*
           终端过滤器B      
           */
           m_aInsListItem[iMapIdx]->setName(tr("Final Fliter B"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_T_A_FILTER:
           /*
           终端过滤器A      
           */
           m_aInsListItem[iMapIdx]->setName(tr("Final Fliter A"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_TUBE_FILTER:
           /*
           管路过滤器       
           */
           m_aInsListItem[iMapIdx]->setName(tr("Loop Filter"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_TUBE_DI:
           /*
           管路DI     
           */
           m_aInsListItem[iMapIdx]->setName(tr("Loop DI"));
           m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
           break;
       case DISP_MACHINERY_SOURCE_BOOSTER_PUMP:

          /*
          原水增压泵
          */
          m_aInsListItem[iMapIdx]->setName(tr("Feed Pump"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;
          
      case DISP_MACHINERY_TUBE_CIR_PUMP:
          /*
          管路循环泵       
          */
          m_aInsListItem[iMapIdx]->setName(tr("Dist. Pump"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;

      case DISP_MACHINERY_CIR_PUMP:
          
          /*
          循环泵       
          */
          m_aInsListItem[iMapIdx]->setName(tr("Recir. Pump"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;
          
      case DISP_MACHINERY_RO_MEMBRANE:
          /*
          RO膜
          */
          m_aInsListItem[iMapIdx]->setName(tr("RO Membrane"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;
          
      case DISP_MACHINERY_RO_BOOSTER_PUMP:
          /*
          RO增压泵
          安装
          */
          m_aInsListItem[iMapIdx]->setName(tr("RO Pump"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;
          
      case DISP_MACHINERY_EDI:
          /*
          EDI
          */
          m_aInsListItem[iMapIdx]->setName(tr("EDI Module"));
          m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
          break;           
       }
       if (iEscape)
       {
          continue;
       }
       m_aInsListItem[iMapIdx]->setItemId(iIdx);
       m_aInsListItem[iMapIdx]->setBtnId(iMapIdx);
       m_aInsListItem[iMapIdx]->setCatName(tr("CAT"));
       m_aInsListItem[iMapIdx]->setLotName(tr("LOT"));
       iMapIdx++;
    }

    for (iIdx = 0; iIdx < iMapIdx; iIdx++)
    {
        listWidget->setItemHidden(listWidgetIem[iIdx],false);
    }

    for (iIdx = iMapIdx; iIdx < m_iRealItemNum; iIdx++)
    {
        listWidget->setItemHidden(listWidgetIem[iIdx],true);
    }

    m_iViewItemNum = iMapIdx;
}

void ConsumableInsPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}



void ConsumableInsPage::createList()
{
    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};

    
    listWidget = new QListWidget((QWidget *)m_widget);

    listWidget->setStyleSheet("background-color:transparent");
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setFrameShape(QListWidget::NoFrame);

    listWidget->setGeometry(QRect(10,60 ,780,490));

    m_iCurrentItem = 0;

    for(int iLoop = 0 ; iLoop < m_iRealItemNum ; iLoop++)
    {
        listWidgetIem[iLoop] = new QListWidgetItem;
        listWidgetIem[iLoop]->setSizeHint(QSize(600 , CI_LIST_WIDGET_HEIGHT));
        listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        m_aInsListItem[iLoop] = new CosumableInsListWidgtItem(NULL,this,iLoop);

        listWidget->insertItem(iLoop,listWidgetIem[iLoop]);
        
        listWidget->setItemWidget(listWidgetIem[iLoop] , m_aInsListItem[iLoop]);

    }
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem *)));
}

void ConsumableInsPage::setBackColor()
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

void ConsumableInsPage::initUi()
{
    setBackColor();

    createList();
}

void ConsumableInsPage::update()
{
   m_wndMain->setWorkMode(APP_WORK_MODE_INSTALL);
   
   buildTranslation();
   
}

void ConsumableInsPage::fade()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_NORMAL);
}

void ConsumableInsPage::updateRfidInfo(int iRfId)
{
   /* search item */
   int index = m_aInsListItem[m_iCurrentItem]->getItemId();
   int iMapIdx;
   
   qDebug() << "updateRfidInfo :  " << index << m_iCurrentItem <<aIds[index].iRfid; 
   
   if (aIds[index].iRfid != iRfId)
   {
      return ;
   }

   if (index < m_iViewItemNum)
   {
      int                 iRet;

      CATNO cn;
      LOTNO ln;
      
      memset(cn,0,sizeof(CATNO));
      memset(ln,0,sizeof(LOTNO));

      if (m_wndMain->getRfidState(iRfId))
      {
          m_wndMain->getRfidCatNo(iRfId,cn);
          m_wndMain->getRfidLotNo(iRfId,ln);
      }
      else
      {
          if (!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization)))
          {
              iRet = m_wndMain->readRfid(iRfId);
          
              if (iRet)
              {
                  return;
              }
          }
          
          m_wndMain->getRfidCatNo(iRfId,cn);
          m_wndMain->getRfidLotNo(iRfId,ln);
      }

      m_aInsListItem[m_iCurrentItem]->setP1(cn);
      m_aInsListItem[m_iCurrentItem]->setP2(ln);
  }       
}


void ConsumableInsPage::ItemClicked(QListWidgetItem * item)
{
    QListWidget *pTmpListWidget = (QListWidget *)this->sender();
    
    m_iCurrentItem = pTmpListWidget->row(item);

}

void ConsumableInsPage::toCurrentItem(int index)
{
    m_iCurrentItem = index;
}

void ConsumableInsPage::on_btn_clicked(int index)
{  
   int iOffset = 0;
   int iLen;
   
   /* check SN number */
   if (index < m_iViewItemNum)
   {
       CATNO cn;
       LOTNO ln;
       QString strCn = m_aInsListItem[index]->getP1();
       QString strLn = m_aInsListItem[index]->getP2();

       int iMapIdx = m_aInsListItem[index]->getItemId();

       memset(cn,0,sizeof(CATNO));
       memset(ln,0,sizeof(LOTNO));

       toCurrentItem(index);

       if (strCn.isEmpty()
           || strLn.isEmpty())
       {
          /* try to read info by RFID */
          
          if ( aIds[iMapIdx].iRfid < APP_RF_READER_MAX_NUMBER)
          {
              int  iRet;

              if (m_wndMain->getRfidState(aIds[iMapIdx].iRfid))
              {
                  m_wndMain->getRfidCatNo(aIds[iMapIdx].iRfid,cn);
                  m_wndMain->getRfidLotNo(aIds[iMapIdx].iRfid,ln);
              }
              else
              {
              
                  if (!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization)))
                  {
                      iRet = m_wndMain->readRfid(aIds[iMapIdx].iRfid);
    
                      if (iRet)
                      {
                          QMessageBox::about(NULL, tr("Alarm"), "<font color='red'>"+tr("Failed to read RFID,please make sure the part is installed correctly.")+"</font>");
                          return;
                      }
                  }
                  
                  m_wndMain->getRfidCatNo(aIds[iMapIdx].iRfid,cn);
                  m_wndMain->getRfidLotNo(aIds[iMapIdx].iRfid,ln);
              }

              m_aInsListItem[index]->setP1(cn);
              m_aInsListItem[index]->setP2(ln);
          }
          else
          {
              QMessageBox::about(NULL, tr("About"), tr("Please Input Serial Number !"));
              return;
          }
       }
       
       strCn = m_aInsListItem[index]->getP1();
       strLn = m_aInsListItem[index]->getP2();

       if (strCn.isEmpty()
           || strLn.isEmpty())
       {
           QMessageBox::about(NULL, tr("About"), tr("Please Input Serial Number !"));
           return;
       }
       switch(aIds[iMapIdx].iType)
       {
       case 0:
          {
              int cmIdx = aIds[iMapIdx].iId;
          
              //if (0 == strcmp(strCn.toAscii(),gGlobalParam.cmSn.aCn[cmIdx])
              //    || 0 == strcmp(strLn.toAscii(),gGlobalParam.cmSn.aLn[cmIdx]))
              //{
              //   return ;
              //}
   
              /* install */
              strncpy(cn,strCn.toAscii(),APP_CAT_LENGTH);
              strncpy(ln,strLn.toAscii(),APP_LOT_LENGTH);
   
              MainSaveCMSnItem(gGlobalParam.iMachineType,cn,ln,cmIdx);
              
              strncpy(gGlobalParam.cmSn.aCn[cmIdx],cn,APP_CAT_LENGTH);
              strncpy(gGlobalParam.cmSn.aLn[cmIdx],ln,APP_LOT_LENGTH);
   
              MainResetCmInfo(cmIdx);
   
              m_wndMain->MainWriteCMInstallInfo2Db(cmIdx,0,cn,ln);
          }        
           break;
       case 1:
         {
             int macIdx = aIds[iMapIdx].iId - DISP_MACHINERY_SOURCE_BOOSTER_PUMP;
             if (0 == strcmp(strCn.toAscii(),gGlobalParam.macSn.aCn[macIdx])
                || 0 == strcmp(strLn.toAscii(),gGlobalParam.macSn.aLn[macIdx]))
             {
                return ;
             }
             
             /* install */
             strncpy(cn,strCn.toAscii(),APP_CAT_LENGTH);
             strncpy(ln,strLn.toAscii(),APP_LOT_LENGTH);
             
             MainSaveMacSnItem(gGlobalParam.iMachineType,cn,ln,macIdx);
             
             strncpy(gGlobalParam.cmSn.aCn[macIdx],cn,APP_CAT_LENGTH);
             strncpy(gGlobalParam.cmSn.aLn[macIdx],ln,APP_LOT_LENGTH);

             /* reset state info */
             m_wndMain->MainWriteMacInstallInfo2Db(macIdx,0,cn,ln);
         }        
         break;       
       }
    }
   
   m_wndMain->prepareKeyStroke();
}

