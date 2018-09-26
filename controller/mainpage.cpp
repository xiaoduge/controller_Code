#include <QDebug>
#include <QString>
#include <QPainter>

#include "mainwindow.h"
#include "mainpage.h"
#include "navigatorbar.h"

#define SINGLE_MACHINE_PANEL_XOFF (408)
#define SINGLE_MACHINE_PANEL_YOFF (-162)

#define SINGLE_MACHINE_DIALER_XOFF (0)
#define SINGLE_MACHINE_DIALER_YOFF (123)


#define TIMER_DURATION (100)
#define TIMER_DELAY_COUNTS (1000/TIMER_DURATION)

enum TEXT_ALIGN_ENUM
{
   TEXT_ALIGN_DEFAULT = 0,
   TEXT_ALIGN_LEFT,
   TEXT_ALIGN_RIGHT,
   TEXT_ALIGN_NUM
};


static CONFIG_BTN sBtns[BTN_NUMBER] = 
{
    {149,340,NULL,":/pic/navi_btn_up.png"   ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {148,494,NULL,":/pic/navi_btn_down.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {112,377,NULL,":/pic/navi_btn_left.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {266,378,NULL,":/pic/navi_btn_right.png",BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {507,340,NULL,":/pic/navi_btn_up.png"   ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {506,494,NULL,":/pic/navi_btn_down.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {470,377,NULL,":/pic/navi_btn_left.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    {624,378,NULL,":/pic/navi_btn_right.png",BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,},
    
    {235,246,":/pic/up_circulation.png"    ,NULL,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,},
    {362,285,":/pic/normal_circulation.png",NULL,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,},

    {176,403,NULL,":/pic/navi_btn_mid.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE ,},
    {534,403,NULL,":/pic/navi_btn_mid.png" ,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE ,},

    {400-18,370,":/pic/Remind.png"       ,NULL,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE ,},

    //{235,264,":/pics/image/btn_power_back.png","btn_power_front.png",BITMAPBUTTON_STYLE_PUSH,},
};


static CONFIG_LABEL sLabels[LABEL_NUMBER] = {
    /* UP */
    {77,106,146,50,GLOBAL_FONT_60,QColor(255,255,255),NULL,"18.2",0,0},
    {233,140,50,20,GLOBAL_FONT_14,QColor(255,255,255),NULL,"O",0,0},//M¦¸.cm
    {100,190,40,15,GLOBAL_FONT_14,QColor(255,255,255),NULL,"TOC",0,0},
    {145,177,90,30,GLOBAL_FONT_30,QColor(255,255,255),NULL,"3",0,0},
    {240,190,30,15,GLOBAL_FONT_14,QColor(255,255,255),NULL,"ppb",0,0},
    {130,230,70,30,GLOBAL_FONT_30,QColor(255,255,255),NULL,"25.0",0,0},
    {205,240,15,20,GLOBAL_FONT_14,QColor(255,255,255),NULL,"C",0,0}, // ¡æ
    {40,275,46,30,GLOBAL_FONT_30,QColor(255,255,255),":/pics/image/indicator.png","UP",0,0},
    {97,88,146,16,GLOBAL_FONT_14,QColor(255,255,255),NULL,0,0,TEXT_ALIGN_LEFT},

    /* EDI */
    {530,106,146,50,GLOBAL_FONT_60,QColor(255,255,255),NULL,"15.0",0,0},
    {686,140,50,20,GLOBAL_FONT_14,QColor(255,255,255),NULL,"O",0,0},
    {548,190,40,15,GLOBAL_FONT_14,QColor(255,255,255),NULL,"LOOP",0,0},
    {600,177,58,30,GLOBAL_FONT_30,QColor(255,255,255),NULL,"10.0",0,0},
    {663,190,50,20,GLOBAL_FONT_14,QColor(255,255,255),NULL,"us",0,0},
    {588,230,70,30,GLOBAL_FONT_30,QColor(255,255,255),NULL,"25.0",0,0},
    {665,240,15,20,GLOBAL_FONT_14,QColor(255,255,255),NULL,"C",0,0},
    {718,275,46,30,GLOBAL_FONT_30,QColor(255,255,255),":/pics/image/indicator.png","EDI",0,0},
    {550,88,146,16,GLOBAL_FONT_14,QColor(255,255,255),NULL,0,0,TEXT_ALIGN_LEFT},

    /* TANK */
    {360,115,75,35,GLOBAL_FONT_40,QColor(255,255,255),NULL,"0.0",0,0},
    {438,128,10,16,GLOBAL_FONT_14,QColor(255,255,255),NULL,"L",0,0},
    {360,170,75,35,GLOBAL_FONT_40,QColor(255,255,255),NULL,"18.2",0,0},
    {380,205,60,16,GLOBAL_FONT_14,QColor(255,255,255),NULL,"O",0,0}, // omg
    {360,50,146,16,GLOBAL_FONT_14,QColor(255,255,255),NULL,0,0,TEXT_ALIGN_LEFT},


    {186,435,60,25,GLOBAL_FONT_30,QColor(156,156,168),NULL," ",BTN_NAVI_UP_TW,0},
    {248,445,10,15,GLOBAL_FONT_14,QColor(156,156,168),NULL,"L",BTN_NAVI_UP_TW,0},

    {546,435,60,25,GLOBAL_FONT_30,QColor(156,156,168),NULL," ",BTN_NAVI_EDI_TW,0},
    {604,445,10,15,GLOBAL_FONT_14,QColor(156,156,168),NULL,"L",BTN_NAVI_EDI_TW,0},
    
    {346,76,112,184,GLOBAL_FONT_14,QColor(0,0,0),":/pic/tank_calibration.png",0,0,0},
    {346,76,112,184,GLOBAL_FONT_14,QColor(0,0,0),NULL,0,0,0},

    {167,395,108,74,GLOBAL_FONT_14,QColor(0,0,0),NULL,0,0,0},
    {525,395,108,74,GLOBAL_FONT_14,QColor(0,0,0),NULL,0,0,0},
    
};

MainPage::MainPage(QObject *parent,CBaseWidget *widget,MainWindow *wndMain) : CPage(parent,widget,wndMain)
{
    int iLoop;

    m_fToc = 3;

    m_bSingleMachine = false;


    for (iLoop = 0; iLoop < LABEL_NUMBER; iLoop++)
    {
        m_aiLblMap[iLoop] = iLoop;
    }
    
    for (iLoop = 0; iLoop < BTN_NUMBER; iLoop++)
    {
        m_aiBtnMap[iLoop] = iLoop;
    }


    /* single machine */
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_RO:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
        m_bSingleMachine = true;

        m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]   = LABEL_NAVI_UP_WQ_VALUE;
        m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE] = LABEL_NAVI_UP_TEMP_VALUE;
        m_aiLblMap[LABEL_NAVI_EDI_STATE]      = LABEL_NAVI_UP_STATE;
        m_aiLblMap[LABEL_EDI_SPEED]           = LABEL_UP_SPEED;

        m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_VALUE]   = LABEL_NAVI_UPWQUANTITY_VALUE;
        m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_UNIT]    = LABEL_NAVI_UPWQUANTITY_UNIT;

        m_aiBtnMap[BTN_NAVI_UP_INC_VOLAGE]    = BTN_NAVI_EDI_INC_VOLAGE;
        m_aiBtnMap[BTN_NAVI_UP_DEC_VOLTAGE]   = BTN_NAVI_EDI_DEC_VOLTAGE;
        m_aiBtnMap[BTN_NAVI_UP_DEC_VOLUME]    = BTN_NAVI_EDI_DEC_VOLUME;
        m_aiBtnMap[BTN_NAVI_UP_INC_VOLUME]    = BTN_NAVI_EDI_INC_VOLUME;

        m_aiBtnMap[BTN_NAVI_UP_TW]            = BTN_NAVI_EDI_TW;
        break;
    case MACHINE_PURIST:
        
        m_bSingleMachine = true;

        m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]   = LABEL_NAVI_UP_WQ_VALUE;
        m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE] = LABEL_NAVI_UP_TEMP_VALUE;
        m_aiLblMap[LABEL_NAVI_EDI_STATE]      = LABEL_NAVI_UP_STATE;
        m_aiLblMap[LABEL_EDI_SPEED]           = LABEL_UP_SPEED;

        m_aiBtnMap[BTN_NAVI_EDI_INC_VOLAGE]   = BTN_NAVI_UP_INC_VOLAGE;
        m_aiBtnMap[BTN_NAVI_EDI_DEC_VOLTAGE]  = BTN_NAVI_UP_DEC_VOLTAGE;
        m_aiBtnMap[BTN_NAVI_UP_DEC_VOLUME]    = BTN_NAVI_UP_DEC_VOLUME;
        m_aiBtnMap[BTN_NAVI_UP_INC_VOLUME]    = BTN_NAVI_UP_INC_VOLUME;
        
        m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_VALUE]   = LABEL_NAVI_UPWQUANTITY_VALUE;
        m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_UNIT]    = LABEL_NAVI_UPWQUANTITY_UNIT;

        break;
    }
    InitNames();
    
    creatTitle();

    initUi();

    buildTranslation();

    updEcoInfo(APP_EXE_I2_NO,m_wndMain->getEco(APP_EXE_I2_NO),true);
    updEcoInfo(APP_EXE_I4_NO,m_wndMain->getEco(APP_EXE_I4_NO),true);
    updEcoInfo(APP_EXE_I5_NO,m_wndMain->getEco(APP_EXE_I5_NO),true);
    //ex
    updEcoInfo(APP_EXE_I3_NO,m_wndMain->getEco(APP_EXE_I3_NO),true);
    //end

    for (iLoop = 0; iLoop < APP_DEV_HS_SUB_NUM; iLoop++)
    {
       m_afWQuantity[iLoop] = 0;
       
       m_abQtwFlag[iLoop] = false;
    }

    m_iNotState = 0;

    m_timeTimer = new QTimer(this);
    connect(m_timeTimer, SIGNAL(timeout()), this, SLOT(on_timerEvent()));
    //ex
    int iValue = m_wndMain->getRPumpValue(0);
    DrawSpeed(0, iValue);
    iValue = m_wndMain->getRPumpValue(1);
    DrawSpeed(1, iValue);
    //end
}

void MainPage::initUi()
{
    int index;
    int iLoop;

    int x;
    
    QPixmap back(m_bSingleMachine ? ":/pic/single_bg_ncir.png" : ":/pic/navigator_bg.png");

    QSize size(back.width(), back.height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    p.drawPixmap(0,0,back);

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);

    for( index = 0 ; index < TANK_WATER_LEVEL_SCALE_NUM ; index++)
    {
        QString strPath;
    
        strPath.sprintf(":/pic/tank_%02d.png",index);
   
        m_pBmpTankWaterLevels[index] = new QPixmap(strPath);
    }

    for( index = 0 ; index < TANK_WATER_LEVEL_SCALE_NUM ; index++)
    {
        QString strPath;
    
        strPath.sprintf(":/pic/speed_%02d.png",index);
   
        m_pBmpPumpSpeeds[index] = new QPixmap(strPath);
    }

    for( index = LABEL_UP_SPEED ; index < LABEL_NUMBER ; index++)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,sLabels[index].colorText);
        palette.setColor(QPalette::WindowText,sLabels[index].colorText);

        if(m_bSingleMachine)
        {
            if ( index == LABEL_UP_SPEED) 
            {
                sLabels[index].xPos += SINGLE_MACHINE_PANEL_XOFF;
                sLabels[index].yPos += SINGLE_MACHINE_PANEL_YOFF;
            }
        }

        if (!sLabels[index].parentBtn)
        {
            m_pLabels[index] = new QLabel(m_widget);
            m_pLabels[index]->setGeometry(sLabels[index].xPos,sLabels[index].yPos,sLabels[index].Width,sLabels[index].Height);
        }
        else
        {
            m_pLabels[index] = new QLabel(m_pBtns[sLabels[index].parentBtn]);
            m_pLabels[index]->setGeometry(sLabels[index].xPos - sBtns[sLabels[index].parentBtn].xPos,sLabels[index].yPos - sBtns[sLabels[index].parentBtn].yPos,sLabels[index].Width,sLabels[index].Height);
        }
        m_pLabels[index]->setPalette(palette);
        
        if (sLabels[index].strBackBitmapPath)
        {
            m_pLabels[index]->setPixmap(*(new QPixmap(sLabels[index].strBackBitmapPath)));
        }
        m_pLabels[index]->setStyleSheet("background-color:transparent");

        if (TEXT_ALIGN_LEFT == sLabels[index].attr)
        {
            m_pLabels[index]->setAlignment(Qt::AlignLeft);
        }
        else
        {
            m_pLabels[index]->setAlignment(Qt::AlignCenter);
        }

        m_pLabels[index]->setFont(*m_wndMain->getFont(sLabels[index].enFont));

        if (sLabels[index].str) m_pLabels[index]->setText(sLabels[index].str);

        if(m_bSingleMachine)
        {
            if ( index != LABEL_UP_SPEED) 
            {
                m_pLabels[index]->hide();
            }
            else
            {
                m_pLabels[index]->show();
            }
        }
        else
        {
            m_pLabels[index]->show();
        }
    }    

    for( index = 0 ; index < BTN_NUMBER ; index++)
    {
        m_pBtns[index] = new CBitmapButton(m_widget,sBtns[index].enStyle,sBtns[index].enPicStyle,index);

        if(m_bSingleMachine)
        {
            if ( (index >= BTN_NAVI_UP_INC_VOLAGE) 
                && (index <= BTN_NAVI_UP_INC_VOLUME))
            {
                sBtns[index].xPos += SINGLE_MACHINE_PANEL_XOFF;
                sBtns[index].yPos += SINGLE_MACHINE_PANEL_YOFF;
                
            }
            
            if ( (index == BTN_CIRCULATION_UP)
                ||(index == BTN_CIRCULATION_GENERAL)
                ||(index == BTN_ALARM_NOTIFY)) 
            {
                sBtns[index].yPos += SINGLE_MACHINE_DIALER_YOFF;
            } 

            if ( index == BTN_NAVI_UP_TW )
            {
                sBtns[index].xPos += SINGLE_MACHINE_PANEL_XOFF;
                sBtns[index].yPos += SINGLE_MACHINE_PANEL_YOFF;
            }
        }

        if (sBtns[index].strBackBitmapPath)
        {
            m_pBtns[index]->setButtonPicture(new QPixmap(sBtns[index].strBackBitmapPath));
        }

        if (sBtns[index].strFrontBitmapPath)
        {
            m_pBtns[index]->setPressPicture(new QPixmap(sBtns[index].strFrontBitmapPath));
        }
        m_pBtns[index]->setGeometry(sBtns[index].xPos,sBtns[index].yPos,m_pBtns[index]->getPicWidth(),m_pBtns[index]->getPicHeight());

        m_pBtns[index]->setStyleSheet("background-color:transparent");
        connect(m_pBtns[index], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
        
        if(m_bSingleMachine)
        {
            if ( (index >= BTN_NAVI_UP_INC_VOLAGE) 
                && (index <= BTN_NAVI_UP_INC_VOLUME))
            {
                m_pBtns[index]->show();
            }
            else if ((index == BTN_CIRCULATION_GENERAL)
                    ||(index == BTN_ALARM_NOTIFY)
                    ||(index == BTN_NAVI_UP_TW)) 
            
            {
                m_pBtns[index]->show();
            }
            else
            {
                m_pBtns[index]->hide();
            }
        }
        else
        {
            m_pBtns[index]->show();
        }
    }

    /* for volume up/down */
    connect(m_pBtns[BTN_NAVI_UP_DEC_VOLUME], SIGNAL(clicking(int)), this, SLOT(on_btn_clicking(int)));
    connect(m_pBtns[BTN_NAVI_UP_INC_VOLUME], SIGNAL(clicking(int)), this, SLOT(on_btn_clicking(int)));
    connect(m_pBtns[BTN_NAVI_EDI_DEC_VOLUME], SIGNAL(clicking(int)), this, SLOT(on_btn_clicking(int)));
    connect(m_pBtns[BTN_NAVI_EDI_INC_VOLUME], SIGNAL(clicking(int)), this, SLOT(on_btn_clicking(int)));

    for( index = 0 ; index < LABEL_UP_SPEED ; index++)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,sLabels[index].colorText);
        palette.setColor(QPalette::WindowText,sLabels[index].colorText);

        if(m_bSingleMachine)
        {
            if ( (index >= LABEL_NAVI_UP_WQ_VALUE) 
                && (index <= LABEL_NAVI_UP_STATE))
            {
                sLabels[index].yPos += SINGLE_MACHINE_DIALER_YOFF;
            }

            
            if ( (index >= LABEL_NAVI_UPWQUANTITY_VALUE) 
                && (index <= LABEL_NAVI_UPWQUANTITY_UNIT))
            {
                sLabels[index].xPos += SINGLE_MACHINE_PANEL_XOFF;
                sLabels[index].yPos += SINGLE_MACHINE_PANEL_YOFF;
                
            }
            
            if ( (index >= LABEL_NAVI_WT_VOL_VALUE) 
                && (index <= LABEL_NAVI_TANK_STATE))
            {
                sLabels[index].yPos += SINGLE_MACHINE_DIALER_YOFF;
            }
            
            if ( (index >= LABEL_TANK_CALIBRATION) 
                && (index <= LABEL_TANK_VALUE))
            {
                sLabels[index].yPos += SINGLE_MACHINE_DIALER_YOFF;
            }
                
        }
        
        if (!sLabels[index].parentBtn)
        {
            m_pLabels[index] = new QLabel(m_widget);
            m_pLabels[index]->setGeometry(sLabels[index].xPos,sLabels[index].yPos,sLabels[index].Width,sLabels[index].Height);
        }
        else
        {
            m_pLabels[index] = new QLabel(m_pBtns[sLabels[index].parentBtn]);
            m_pLabels[index]->setGeometry(sLabels[index].xPos - sBtns[sLabels[index].parentBtn].xPos,sLabels[index].yPos - sBtns[sLabels[index].parentBtn].yPos,sLabels[index].Width,sLabels[index].Height);
        }
        m_pLabels[index]->setPalette(palette);
        
        if (sLabels[index].strBackBitmapPath)
        {
            m_pLabels[index]->setPixmap(*(new QPixmap(sLabels[index].strBackBitmapPath)));
        }
        m_pLabels[index]->setStyleSheet("background-color:transparent");
        if (TEXT_ALIGN_LEFT == sLabels[index].attr)
        {
            m_pLabels[index]->setAlignment(Qt::AlignLeft);
        }
        else
        {
            m_pLabels[index]->setAlignment(Qt::AlignCenter);
        }

        m_pLabels[index]->setFont(*m_wndMain->getFont(sLabels[index].enFont));

        if (sLabels[index].str) m_pLabels[index]->setText(sLabels[index].str);
        
        if(m_bSingleMachine)
        {
            if ( (index >= LABEL_NAVI_UP_WQ_VALUE) 
                && (index <= LABEL_NAVI_UP_STATE))
            {
                m_pLabels[index]->show();
            }
            else if((index >= LABEL_NAVI_UPWQUANTITY_VALUE) 
                && (index <= LABEL_NAVI_UPWQUANTITY_UNIT))
            {
               // m_pLabels[m_aiLblMap[index]]->show();
                m_pLabels[m_aiLblMap[index]]->hide();
                
            }
            else if((index >= LABEL_NAVI_EDIWQUANTITY_VALUE) 
                && (index <= LABEL_NAVI_EDIWQUANTITY_UNIT))
            {
               // m_pLabels[m_aiLblMap[index]]->show();
                m_pLabels[m_aiLblMap[index]]->hide();
                
            }
            else if ( (index >= LABEL_NAVI_WT_VOL_VALUE) 
                && (index <= LABEL_NAVI_TANK_STATE))
            {
                m_pLabels[index]->show();
            }
            else if ( (index >= LABEL_TANK_CALIBRATION) 
                && (index <= LABEL_TANK_VALUE))
            {
                m_pLabels[index]->show();
            }
            else
            {
                m_pLabels[index]->hide();
            }
        }
        else
        {
            if((index >= LABEL_NAVI_UPWQUANTITY_VALUE)
                && (index <= LABEL_NAVI_UPWQUANTITY_UNIT))
            {
               // m_pLabels[m_aiLblMap[index]]->show();
                m_pLabels[m_aiLblMap[index]]->hide();

            }
            else if((index >= LABEL_NAVI_EDIWQUANTITY_VALUE)
                && (index <= LABEL_NAVI_EDIWQUANTITY_UNIT))
            {
               // m_pLabels[m_aiLblMap[index]]->show();
                m_pLabels[m_aiLblMap[index]]->hide();

            }
            else
            {
                m_pLabels[index]->show();
            }

        }
    }

    x = 400 - PAGEID_MARGIN/2 - PAGEID_MARGIN - gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width()*2;
    for ( index = 0 ; index < 4 ; index++)
    {
        m_pLbPageId[index] = new QLabel(m_widget);
        m_pLbPageId[index]->setGeometry(x ,560,gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width(),gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->height());
        x += (PAGEID_MARGIN + gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width());

        if (index == 0)
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]);
        }
        else
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_UNSELECT]);
        }
        
        m_pLbPageId[index]->show();

        qDebug() << gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width() << gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->height();
    }

    m_pLabels[LABEL_NAVI_UP_LOGO]->setText(m_aWaterType[gGlobalParam.iMachineType][0]) ;
    m_pLabels[LABEL_NAVI_EDI_LOGO]->setText(m_aWaterType[gGlobalParam.iMachineType][1]) ;
   
    m_pLabels[LABEL_NAVI_UP_TOC_LBL]->setText(m_aWaterTLD[gGlobalParam.iMachineType][0]) ;
    m_pLabels[LABEL_NAVI_EDI_LOOP_LBL]->setText(m_aWaterTLD[gGlobalParam.iMachineType][1]) ;
    
    m_pLabels[LABEL_NAVI_UP_TOC_UNIT]->setText(m_aWaterUnit[gGlobalParam.iMachineType][0]) ;
    m_pLabels[LABEL_NAVI_EDI_LOOP_UNIT]->setText(m_aWaterUnit[gGlobalParam.iMachineType][1]) ;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:   
    case MACHINE_L_UP: 
    {
        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->hide();
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->hide();
        break;
    }
    case MACHINE_PURIST:
        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->show();
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->show();
        break;
    default:     
    {
        m_pLabels[LABEL_NAVI_EDI_LOOP_VALUE]->hide();            
        m_pLabels[LABEL_NAVI_EDI_LOOP_UNIT]->hide();
        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->hide(); //
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->hide();  //
        break;
    }
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_EDI:
    case MACHINE_RO:
        m_pLabels[LABEL_NAVI_UP_TOC_VALUE]->hide();         
        m_pLabels[LABEL_NAVI_UP_TOC_UNIT]->hide(); 
        break;
    case MACHINE_PURIST: 
        {
            //m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->hide();
            //m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->hide();

            if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
            {
                m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->hide();
                m_pLabels[LABEL_NAVI_WT_VOL_UNIT]->hide();
            }
            
            m_pBtns[BTN_CIRCULATION_UP]->show();
            m_pBtns[BTN_CIRCULATION_GENERAL]->hide();
            break;
        }  
    case MACHINE_ADAPT: 
        m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->hide();
        m_pLabels[LABEL_NAVI_WT_VOL_UNIT]->hide();
        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->hide();
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->hide();      
        m_pLabels[LABEL_NAVI_TANK_STATE]->hide();
        m_pLabels[LABEL_TANK_VALUE]->hide();
        m_pBtns[BTN_CIRCULATION_GENERAL]->enable(false);
        break;
    }
    
    m_pBtns[BTN_ALARM_NOTIFY]->hide();

    m_pNaviBar = new navigatorBar(m_widget);

    m_pNaviBar->setGeometry(0,0,800,55);
    
    if(m_bSingleMachine)
    {
        m_pNaviBar->display(true);
        //m_pNaviBar->setShowDuration(0x7FFFFFFF);
        m_pNaviBar->setShowDuration(5);
    }
    connect(m_pNaviBar, SIGNAL(clicked(int)), this, SLOT(on_navi_clicked(int)));

}

void MainPage::creatTitle()
{
    CPage::creatTitle();
    buildTitles();
    selectTitle(0);
}


void MainPage::update()
{
   updAlarmState( m_wndMain->getAlarmState());

   updMainpageState();

   updOtherState();

   updIsInfo(APP_EXE_I2_NO,&m_aHistoryEco[APP_EXE_I2_NO]);
   updIsInfo(APP_EXE_I4_NO,&m_aHistoryEco[APP_EXE_I4_NO]);
   updIsInfo(APP_EXE_I5_NO,&m_aHistoryEco[APP_EXE_I5_NO]);

   //ex
   updIsInfo(APP_EXE_I3_NO,&m_aHistoryEco[APP_EXE_I3_NO]);

   updToc(m_fToc);
   //end
}

void MainPage::buildTitles()
{
    QStringList stringList;
    
    // strictly according to RF_STATE_ENUM
    stringList << tr("Menu");

    
    setTitles(stringList);

}

void MainPage::InitNames(void)
{
    int iLoop;
    for(iLoop = 0;iLoop < MACHINE_NUM;iLoop++)
    {
        switch(iLoop)
        {
        case MACHINE_L_Genie:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "EDI";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = "LOOP";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = tr("omg");
            break;
        case MACHINE_L_UP:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "RO";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = "DIST";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = tr("omg");
            break;
        case MACHINE_L_EDI_LOOP:
            m_aWaterType[iLoop][0] = "EDI";
            m_aWaterType[iLoop][1] = "";
            m_aWaterTLD[iLoop][0]  = "DIST";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = tr("omg");
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_L_RO_LOOP:
            m_aWaterType[iLoop][0] = "RO";
            m_aWaterType[iLoop][1] = "";
            m_aWaterTLD[iLoop][0]  = "DIST";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = tr("omg");
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_Genie:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "EDI";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_UP:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "RO";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = " ";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_EDI:
            m_aWaterType[iLoop][0] = "EDI";
            m_aWaterType[iLoop][1] = "";
            m_aWaterTLD[iLoop][0]  = "";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = "";
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_RO:
            m_aWaterType[iLoop][0] = "RO";
            m_aWaterType[iLoop][1] = "";
            m_aWaterTLD[iLoop][0]  = "";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = "";
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_PURIST:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = "";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = "";
            break;
        case MACHINE_ADAPT:
            m_aWaterType[iLoop][0] = "UP";
            m_aWaterType[iLoop][1] = "RO";
            m_aWaterTLD[iLoop][0]  = "TOC";
            m_aWaterTLD[iLoop][1]  = " ";
            m_aWaterUnit[iLoop][0] = "ppb";
            m_aWaterUnit[iLoop][1] = "";
            break;
        }
    }    
}
void MainPage::buildTranslation()
{
    QString strOmg = tr("omg");
    QString strUs  = tr("us");

    //m_pLabels[LABEL_NAVI_EDI_LOOP_UNIT]->setText(strUs);

    if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
    {
        m_pLabels[LABEL_NAVI_UP_WQ_UNIT]->setText(strOmg);
        m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(strOmg);
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->setText(strOmg);
    }
    else
    {
        m_pLabels[LABEL_NAVI_UP_WQ_UNIT]->setText(strUs);
        m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(strUs);
        m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->setText(strUs);
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        m_pLabels[LABEL_NAVI_UP_TEMP_UNIT]->setText(tr("Celsius"));
        m_pLabels[LABEL_NAVI_EDI_TEMP_UNIT]->setText(tr("Celsius"));
    }
    else
    {
        m_pLabels[LABEL_NAVI_UP_TEMP_UNIT]->setText(tr("Fahrenheit"));
        m_pLabels[LABEL_NAVI_EDI_TEMP_UNIT]->setText(tr("Fahrenheit"));
    }

    if (LIQUID_LEVEL_UINT_PERCENT == gGlobalParam.MiscParam.iUint4LiquidLevel)
    {
        m_pLabels[LABEL_NAVI_WT_VOL_UNIT]->setText(tr("%"));
    }
    else
    {
        m_pLabels[LABEL_NAVI_WT_VOL_UNIT]->setText(tr("L"));
    }

}

void MainPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void MainPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }
    m_curX = e->x();
    m_curY = e->y();

}

void MainPage::mousePressEvent(QMouseEvent *e)
{

    if (e->x() < 800
        && e->y() < 50)
    {
        m_pNaviBar->display(true);
    }

    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_lstFlag = 1;
        m_curX = e->x();
        m_curY = e->y();
    }
}

void MainPage::mouseReleaseEvent(QMouseEvent *e)
{

    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
       
        m_wndMain->naviPage(0,m_curX - m_lstX > 0 ? 1 : 0);
    }

    m_lstFlag = 0;

}

void MainPage::changeWaterQuantity(int iType,bool bAdd,float fValue)
{
    if (iType < 0 || iType >= APP_DEV_HS_SUB_NUM) return ;

    if (bAdd) 
    {
        m_afWQuantity[iType] += fValue;
    }
    else
    {
        m_afWQuantity[iType] = m_afWQuantity[iType] >= fValue ? m_afWQuantity[iType] - fValue : 0;
    }
}

void MainPage::on_btn_clicked(int index)
{
    printf("tmp = %d & map = %d\r\n" , index,m_aiBtnMap[index]);

    switch(m_aiBtnMap[index])
    {
    case BTN_NAVI_UP_INC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_HYPER])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_HYPER,true,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_HYPER,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER));
        break;
    case BTN_NAVI_UP_DEC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_HYPER])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_HYPER,false,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_HYPER,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER));
        break;
    case BTN_NAVI_UP_INC_VOLAGE:
        {
            int iValue = m_wndMain->getRPumpValue(0);

            if (iValue < PUMP_SPEED_NUM - 1)
            {
                iValue++;
                m_wndMain->changeRPumpValue(0,iValue);
                
                DrawSpeed(0,iValue);
                /* set speed */
                SetSpeed(APP_DEV_HS_SUB_HYPER,iValue);
            }
        }
        break;
    case BTN_NAVI_UP_DEC_VOLTAGE:
        {
            int iValue = m_wndMain->getRPumpValue(0);

            if (iValue > 0)
            {
                iValue--;
                m_wndMain->changeRPumpValue(0,iValue);
                
                DrawSpeed(0,iValue);
                SetSpeed(APP_DEV_HS_SUB_HYPER,iValue);
            }
        }        
        break;
    case BTN_NAVI_EDI_INC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_REGULAR])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_REGULAR,true,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_REGULAR,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR));
        break;
    case BTN_NAVI_EDI_DEC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_REGULAR])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_REGULAR,false,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_REGULAR,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR));
        break;
    case BTN_NAVI_EDI_INC_VOLAGE:
        {
            int iValue = m_wndMain->getRPumpValue(1);

            if (iValue < PUMP_SPEED_NUM - 1)
            {
                iValue++;
                m_wndMain->changeRPumpValue(1,iValue);

                DrawSpeed(1,iValue);
                SetSpeed(APP_DEV_HS_SUB_REGULAR,iValue);
            }
        }        
        break;
    case BTN_NAVI_EDI_DEC_VOLTAGE:
        {
            int iValue = m_wndMain->getRPumpValue(1);

            if (iValue > 0)
            {
                iValue--;
                m_wndMain->changeRPumpValue(1,iValue);
                DrawSpeed(1,iValue);
                SetSpeed(APP_DEV_HS_SUB_REGULAR,iValue);
            }
        }        
        
        break;
    case BTN_CIRCULATION_UP:
        m_wndMain->startCir(CIR_TYPE_UP);
        break;
    case BTN_CIRCULATION_GENERAL:
        m_wndMain->startCir(CIR_TYPE_HP);
        break;
    case BTN_NAVI_UP_TW:
        if(DISP_WORK_STATE_IDLE == DispGetWorkState4Pw())
        {
            DB_HANDLER_STRU * hdl;
                                    
            hdl = m_wndMain->getDefaultHandler(APP_DEV_HS_SUB_HYPER);

            if (hdl)
            {
                DispSndHoSystemTest(hdl->address - APP_HAND_SET_BEGIN_ADDRESS,!m_abDecFlag[APP_DEV_HS_SUB_HYPER],APP_PACKET_HO_SYS_TEST_TYPE_DEPRESSURE);
            }
        }
        else
        {
            if(0 == m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER))
            {
                m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = false;
            }   
            m_wndMain->startQtw(APP_DEV_HS_SUB_HYPER,m_abQtwFlag[APP_DEV_HS_SUB_HYPER]);
        }
        break;
    case BTN_NAVI_EDI_TW:
        if(DISP_WORK_STATE_IDLE == DispGetWorkState4Pw())
        {
            DB_HANDLER_STRU * hdl;
                                    
            hdl = m_wndMain->getDefaultHandler(APP_DEV_HS_SUB_REGULAR);

            if (hdl)
            {
                DispSndHoSystemTest(hdl->address - APP_HAND_SET_BEGIN_ADDRESS,!m_abDecFlag[APP_DEV_HS_SUB_REGULAR],APP_PACKET_HO_SYS_TEST_TYPE_DEPRESSURE);
            }
        }
        else
        {
            if(0 == m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR))
            {
                m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = false;
            }   
            m_wndMain->startQtw(APP_DEV_HS_SUB_REGULAR,m_abQtwFlag[APP_DEV_HS_SUB_REGULAR]);
        }
        break;
    case BTN_ALARM_NOTIFY:
        {
            /* navigate to alarm page,late implement*/
           m_wndMain->goAlarm();
        }
        break;
    }
    
    startTimer(false);
    m_wndMain->prepareKeyStroke();

}

void MainPage::startTimer(bool bStart)
{
    if (bStart)
    {
        m_timeTimer->start(TIMER_DURATION);
        m_iTimerCnt = 0;
    }
    else
    {
        m_timeTimer->stop(); 
    }
}

void MainPage::on_timerEvent()
{
    if (m_iTimerCnt++ < TIMER_DELAY_COUNTS) return; 

    switch(m_iVolumeBtnId)
    {
    case BTN_NAVI_UP_INC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_HYPER])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_HYPER,true,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_HYPER,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER));
        break;
    case BTN_NAVI_UP_DEC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_HYPER])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_HYPER,false,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_HYPER,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER));
        break;
    case BTN_NAVI_EDI_INC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_REGULAR])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_REGULAR,true,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_REGULAR,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR));
        break;
    case BTN_NAVI_EDI_DEC_VOLUME:
        if (!m_abQtwFlag[APP_DEV_HS_SUB_REGULAR])
        {
            m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = true;
        }
        else
        {
            m_wndMain->changeWaterQuantity(APP_DEV_HS_SUB_REGULAR,false,1);
        }
        updQtwInfo(APP_DEV_HS_SUB_REGULAR,m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR));
        break;
    }
}

void MainPage::on_btn_clicking(int index)
{
    printf("tmp = %d\r\n" , index);

    switch(m_aiBtnMap[index])
    {
    case BTN_NAVI_UP_INC_VOLUME:
        break;
    case BTN_NAVI_UP_DEC_VOLUME:
        break;
    case BTN_NAVI_EDI_INC_VOLUME:
        break;
    case BTN_NAVI_EDI_DEC_VOLUME:
        break;
    default:
        return;
    }

    m_iVolumeBtnId = m_aiBtnMap[index] ;
    startTimer(true);
}


void MainPage::DrawSpeed(int type,int index)
{
    if (index >= PUMP_SPEED_NUM
        || index < 0)
    {
        return;
    }
    
    {
        switch(type)
        {
        case 0:
            m_pLabels[LABEL_UP_SPEED]->setPixmap(*m_pBmpPumpSpeeds[index]);
            break;
        case 1:
            m_pLabels[m_aiLblMap[LABEL_EDI_SPEED]]->setPixmap(*m_pBmpPumpSpeeds[index]);
            break;
        default:
            break;
        }
    }

}

void MainPage::DrawTank(int index)
{
    if (index >= TANK_WATER_LEVEL_SCALE_NUM)
    {
       index = TANK_WATER_LEVEL_SCALE_10;
    }
    if (index < 0)
    {
        index = TANK_WATER_LEVEL_SCALE_00;
    }

    m_pLabels[m_aiLblMap[LABEL_TANK_VALUE]]->setPixmap(*m_pBmpTankWaterLevels[index]);
}

void  MainPage::SetSpeed(int iType,int index)
{
    int iActive;
    int iTmp;
    int iRet;

    
    DispGetRPumpSwitchState(APP_EXE_C2_NO - APP_EXE_C1_NO,&iActive);
    
    iTmp = DispConvertVoltage2RPumpSpeed(((24.0 - 5.0)*index)/PUMP_SPEED_NUM + 5);
    
    if (iActive) iTmp = 0XFF00 | iTmp;
    
    iRet = DispSetRPump(APP_EXE_C2_NO - APP_EXE_C1_NO,iTmp);
    if (0 == iRet)
    {
        /* broadcast to handlers */
        DispSndHoSpeed(iType,index);
    }
}

void  MainPage::updSpeed(int itype,int iLevel)
{
    DrawSpeed(itype,iLevel);
    m_wndMain->changeRPumpValue(itype,iLevel);
}

void  MainPage::updDecPressureState(int itype,int iAction)
{
    m_abDecFlag[itype] = iAction;
}


void  MainPage::updTankState(int iLevel)
{
    if (iLevel < TANK_WATER_LEVEL_SCALE_NUM 
        && TANK_WATER_LEVEL_SCALE_NUM >= 0)
    {
        DrawTank(iLevel);
    }

}

void  MainPage::updToc(float fToc)
{
    m_fToc = fToc;

    if (isVisible())
    {
        if(fToc == 200)
        {
            m_pLabels[m_aiLblMap[LABEL_NAVI_UP_TOC_VALUE]]->setText(tr(">200"));
        }
        else
        {
            m_pLabels[m_aiLblMap[LABEL_NAVI_UP_TOC_VALUE]]->setText(QString::number(fToc,'f',0));
        }

    }
}


void MainPage::updQtwInfo(int iType,float fValue)
{
   if (isVisible())
   {
       QLabel  *pLabel = NULL;
       QLabel  *pLabelUNIT = NULL;
   
       switch(iType)
       {
       case APP_DEV_HS_SUB_HYPER:
           pLabel     = m_pLabels[m_aiLblMap[LABEL_NAVI_UPWQUANTITY_VALUE]];
           pLabelUNIT = m_pLabels[m_aiLblMap[LABEL_NAVI_UPWQUANTITY_UNIT]];
           break;
       case APP_DEV_HS_SUB_REGULAR:
           pLabel     = m_pLabels[m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_VALUE]];
           pLabelUNIT = m_pLabels[m_aiLblMap[LABEL_NAVI_EDIWQUANTITY_UNIT]];
           break;
       default:
           return ;
       }
       
       if (fValue > 0XFFFFFF0
        || 0 == fValue)
       {
           //pLabel->setText("  ");
           //pLabelUNIT->setText(" ");
           pLabel->hide();
           pLabelUNIT->hide();
       }
       else
       {
           pLabel->show();
           pLabelUNIT->show();
           pLabel->setText(QString::number(fValue/10,'f',1));           
       }
   }
}

void MainPage::updateRunInfo(bool bRun)
{
    m_pNaviBar->updateRunInfo(bRun  );
}

void MainPage::on_navi_clicked(int index)
{
    switch(index)
    {
    case NBAR_ID_POWER_OFF:
        {
            bool stateRun = m_pNaviBar->getState();
#if 0
            //ex_dcj
            if(!stateRun)
            {
                if(DISP_WORK_STATE_IDLE != DispGetWorkState4Pw())
                {
                    if(0 == m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_HYPER))
                    {
                        m_abQtwFlag[APP_DEV_HS_SUB_HYPER] = false;
                    }
                    m_wndMain->startQtw(APP_DEV_HS_SUB_HYPER,m_abQtwFlag[APP_DEV_HS_SUB_HYPER]);

                    if(0 == m_wndMain->getWaterQuantity(APP_DEV_HS_SUB_REGULAR))
                    {
                        m_abQtwFlag[APP_DEV_HS_SUB_REGULAR] = false;
                    }
                    m_wndMain->startQtw(APP_DEV_HS_SUB_REGULAR,m_abQtwFlag[APP_DEV_HS_SUB_REGULAR]);
                }
            }
            //end
#endif
            m_wndMain->run(stateRun);
            
        }
        break;
    }
}

void MainPage::updIsInfo(int iIndex,ECO_INFO_STRU *info)
{
    float fQ ;
    float fT;    
    
    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        fT = info->fTemperature;
    }
    else
    {
        fT = toFahrenheit(info->fTemperature);
    }


    switch(iIndex)
    {
    case APP_EXE_I2_NO:
        {
            fQ = info->fQuality;
            switch(gGlobalParam.iMachineType)
            {
            case MACHINE_PURIST:
            {
                if (DispGetUpQtwFlag())
                {
                    m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ).sprintf("%3d",(int)fQ > 200 ?200:(int)fQ));

                    m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->setText(tr("us"));
                }
                break;
            }
            case MACHINE_ADAPT:
            {
                m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));

                m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ).sprintf("%3d",(int)fQ > 200 ?200:(int)fQ));

                m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(tr("us"));
                break;
            }
            case MACHINE_UP:
            {
                if(!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
                {
                    m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                    m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ).sprintf("%3d",(int)fQ > 200 ?200:(int)fQ));
                    m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(tr("us"));
                }
                break;
            }
            }
        }
        break;
    case APP_EXE_I5_NO:
        { 
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }
            
            //if (DispGetUpQtwFlag())
            {
                {
                    if( 1 > fQ)
                    {
                        m_pLabels[LABEL_NAVI_UP_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                    }
                    else
                    {
                        m_pLabels[LABEL_NAVI_UP_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                    }
                }
                m_pLabels[LABEL_NAVI_UP_TEMP_VALUE]->setText(QString::number(fT,'f',1));
            }

        }
        break;
    case APP_EXE_I4_NO:
        {
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }

            switch(gGlobalParam.iMachineType)
            {
            case MACHINE_PURIST:
            case MACHINE_UP:
            case MACHINE_ADAPT:
                break;
            case MACHINE_Genie:
            {
                if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
                {
                    //if (DispGetEdiQtwFlag())
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]]->setText(QString::number(fQ,'f',3));
                        }
                        else
                        {
                            m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]]->setText(QString::number(fQ,'f',1));
                        }
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                    }

                    if( 1 > fQ)
                    {
                        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                    }
                    else
                    {
                        m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                    }
                }
                break;
            }
            default:
                break;
            }

        }
        break;

    case APP_EXE_I3_NO:
    {
        if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
        {
            fQ = info->fQuality;
        }
        else
        {
            fQ = toConductivity(info->fQuality);
        }

        switch(gGlobalParam.iMachineType)
        {
        case MACHINE_Genie:
        {
            if(!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
            {
                if( 1 > fQ)
                {
                    m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]]->setText(QString::number(fQ,'f',3));
                }
                else
                {
                    m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_WQ_VALUE]]->setText(QString::number(fQ,'f',1));
                }
                m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
            }
            break;
        }
        case MACHINE_UP:
        {
            if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
            {
                if( 1 > fQ)
                {
                    m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                }
                else
                {
                    m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                }
            }
            break;
        }
        default:
            break;
            //
        }
        break;
    }
    }
}

void MainPage::updEcoInfo(int iIndex,ECO_INFO_STRU *info,bool bForceUpd)
{
    float fQ ;
    float fT;

    bool bVisible = isVisible();
    
    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        fT = info->fTemperature;
    }
    else
    {
        fT = toFahrenheit(info->fTemperature);
    }

    switch(iIndex)
    {
    case APP_EXE_I2_NO:
        {
            fQ = info->fQuality;
            switch(gGlobalParam.iMachineType)
            {
            case MACHINE_PURIST:
            {
                if (DispGetUpQtwFlag())
                {
                     if (bVisible)
                     {
                         if(fQ > 200)
                         {
                             fQ = 200;
                         }
                         m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                         m_pLabels[LABEL_NAVI_WT_WQ_UNIT]->setText(tr("us"));
                     }

                     m_aHistoryEco[iIndex] = *info;
                }
                break;
            }

            case MACHINE_ADAPT:
            {
                if (bVisible)
                {
                    m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));

                    m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ).sprintf("%3d",(int)fQ > 200 ? 200 : (int)fQ));

                    m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(tr("us"));
                }
                break;
            }
            case MACHINE_UP:
            {
                if(!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
                {
                    if(DispGetEdiQtwFlag() && bVisible)
                    {
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                        m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ).sprintf("%3d",(int)fQ > 200 ?200:(int)fQ));
                        m_pLabels[LABEL_NAVI_EDI_WQ_UNIT]->setText(tr("us"));
                    }
                    m_aHistoryEco[iIndex] = *info; //ex
                }
                break;
            }
            //RO unfinished
            case MACHINE_RO:
                break;
            default:
                break;
            }
         }
        break;
    case APP_EXE_I5_NO:
        { 
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }
            
            if (DispGetUpQtwFlag()
                ||DispGetUpCirFlag()
                || bForceUpd)
            {
                if (bVisible) 
                {
                    if( 1 > fQ)
                    {
                        m_pLabels[LABEL_NAVI_UP_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                    }
                    else
                    {
                        m_pLabels[LABEL_NAVI_UP_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                    }
                }
                
                if (bVisible) m_pLabels[LABEL_NAVI_UP_TEMP_VALUE]->setText(QString::number(fT,'f',1));
                m_aHistoryEco[iIndex] = *info;
            }

        }
        break;
    case APP_EXE_I4_NO:
        {
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }

            switch(gGlobalParam.iMachineType)
            {
            case MACHINE_Genie:
            {
                if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
                {
                    if(DispGetEdiQtwFlag())
                    {
                        if (bVisible)
                        {
                            if( 1 > fQ)
                            {
                                m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                            }
                            else
                            {
                                m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                            }
                            m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                        }
                        m_aHistoryEco[iIndex] = *info;
                    }
                    else if(DispGetTankCirFlag())
                    {
                        if (bVisible)
                        {
                            if( 1 > fQ)
                            {
                                m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                            }
                            else
                            {
                                m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                            }
                        }
                        m_aHistoryEco[iIndex] = *info;
                    }
                    else if(bForceUpd)
                    {
                        if (bVisible)
                        {
                            if( 1 > fQ)
                            {
                                m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                            }
                            else
                            {
                                m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                            }
                            m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));

                            if( 1 > fQ)
                            {
                                m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                            }
                            else
                            {
                                m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                            }
                        }
                        m_aHistoryEco[iIndex] = *info;
                    }
                }
                break;
            }
            case MACHINE_PURIST:
            case MACHINE_UP:
                break;
            default:
                break;
            }
        }
        break;

    case APP_EXE_I3_NO:
    {
        if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
        {
            fQ = info->fQuality;
        }
        else
        {
            fQ = toConductivity(info->fQuality);
        }
        switch(gGlobalParam.iMachineType)
        {
        case MACHINE_Genie:
        {
            if(!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
            {
                if(DispGetEdiQtwFlag())
                {
                    if (bVisible)
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                    }
                    m_aHistoryEco[iIndex] = *info;
                }
                else if (bForceUpd)
                {
                    if (bVisible)
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                    }
                    m_aHistoryEco[iIndex] = *info;
                }
            }
            break;
        }
        case MACHINE_UP:
        {
            if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
            {
                if(DispGetEdiQtwFlag())
                {
                    if (bVisible)
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));
                    }
                    m_aHistoryEco[iIndex] = *info;
                }
                else if(DispGetTankCirFlag())
                {
                    if (bVisible)
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                    }
                    m_aHistoryEco[iIndex] = *info;
                }
                else if (bForceUpd)
                {
                    if (bVisible)
                    {
                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',3));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_EDI_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                        m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_TEMP_VALUE]]->setText(QString::number(fT,'f',1));

                        if( 1 > fQ)
                        {
                            m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',2));
                        }
                        else
                        {
                            m_pLabels[LABEL_NAVI_WT_WQ_VALUE]->setText(QString::number(fQ,'f',1));
                        }
                    }
                    m_aHistoryEco[iIndex] = *info;
                }
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    }
}

void MainPage::updTank(int iIndex,float fVolume)
{
	bool bVisible = isVisible();

    if (LIQUID_LEVEL_UINT_PERCENT == gGlobalParam.MiscParam.iUint4LiquidLevel)
    {
        if (bVisible) m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->setText(QString::number(iIndex));    
    }
    else
    {
        if (bVisible) m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->setText(QString::number((int)fVolume));    
    }

    DrawTank(iIndex / 10);
}

void MainPage::updQtwState(int iType,bool bStart)
{

    switch(iType)
    {
    case APP_DEV_HS_SUB_HYPER:
        break;
    case APP_DEV_HS_SUB_REGULAR:
        break;
    default:
        return;
    }
    
    m_abQtwFlag[iType] = false;

    if (!bStart)
    {
        updQtwInfo(iType,0XFFFFFFF);        
    }
}

void MainPage::updAlarmState(int iType)
{
   if (m_iNotState == iType)
   {
       return;
   }

   m_iNotState = iType;

   if (MAINPAGE_NOTIFY_STATE_NONE == iType)
   {
       m_pBtns[BTN_ALARM_NOTIFY]->hide();
       return;
   }

   if (iType & MAINPAGE_NOTIFY_STATE_ALARM)
   {
       /* alarm */
       m_pBtns[BTN_ALARM_NOTIFY]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_ALARM_ICON]);
       
       m_pBtns[BTN_ALARM_NOTIFY]->show();
       return ;
   }

   if (iType & MAINPAGE_NOTIFY_STATE_NOT)
   {
       /* alarm */
       m_pBtns[BTN_ALARM_NOTIFY]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_REMIND_ICON]);
       
       m_pBtns[BTN_ALARM_NOTIFY]->show();
       return ;
   }
   
}

void  MainPage::updOtherState(void)
{
    if (!(gGlobalParam.MiscParam.ulAllocMask & (1 << DISPLAY_ALLOC_FLAG_SWITCHON)))
    {
        m_pLabels[LABEL_NAVI_EDI_LOOP_LBL]->hide();
        m_pLabels[LABEL_NAVI_EDI_LOOP_VALUE]->hide();
        m_pLabels[LABEL_NAVI_EDI_LOOP_UNIT]->hide();
     }
    else
    {
        m_pLabels[LABEL_NAVI_EDI_LOOP_LBL]->show();
        m_pLabels[LABEL_NAVI_EDI_LOOP_VALUE]->show();
        m_pLabels[LABEL_NAVI_EDI_LOOP_UNIT]->show();
    }

    
    if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC)))
    {
        m_pLabels[LABEL_NAVI_UP_TOC_LBL]->hide();
        m_pLabels[LABEL_NAVI_UP_TOC_VALUE]->hide();
        m_pLabels[LABEL_NAVI_UP_TOC_UNIT]->hide();
     }
    else
    {
        m_pLabels[LABEL_NAVI_UP_TOC_LBL]->show();
        m_pLabels[LABEL_NAVI_UP_TOC_VALUE]->show();
        m_pLabels[LABEL_NAVI_UP_TOC_UNIT]->show();
    }

    if(!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
    {
        m_pLabels[m_aiLblMap[LABEL_TANK_VALUE]]->hide();
    }

}

void  MainPage::updMainpageState(void)
{
    switch(DispGetWorkState())
    {
    case DISP_WORK_STATE_IDLE:
         if((m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->text().toInt() >= B2_FULL) 
            && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
         {
             m_pLabels[LABEL_NAVI_TANK_STATE] ->setText(tr("Tank Full"));
         }
         else
         {
              m_pLabels[LABEL_NAVI_TANK_STATE] ->setText("");
         }
         break;
    case DISP_WORK_STATE_PREPARE:
        if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
        {
            if(NOT_RUNING_STATE_FLUSH == DispGetRunningStateFlag())
            {
                m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]]->setText(tr("Flush"));
            }
        }
        break;
    case DISP_WORK_STATE_RUN:
        if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
        {
            if(NOT_RUNING_STATE_CLEAN == DispGetRunningStateFlag())
            {
                m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]]->setText(tr("Rinsing"));
            }
        }
         
         if (DispGetPwFlag()
             && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
         {
             m_pLabels[LABEL_NAVI_TANK_STATE] ->setText(tr("Filling"));
         }
         else
         {
             m_pLabels[LABEL_NAVI_TANK_STATE] ->setText("");
         }
         break;
    case DISP_WORK_STATE_LPP:
        if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
        {
             m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]]->setText("");
        }
         break;
    case DISP_WORK_STATE_KP:
         break;
    }
    
    switch(DispGetWorkState4Pw())
    {
    case DISP_WORK_STATE_IDLE:

          if((m_pLabels[LABEL_NAVI_WT_VOL_VALUE]->text().toInt() >= B2_FULL)
               && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
          {
              m_pLabels[LABEL_NAVI_TANK_STATE] ->setText(tr("Tank Full"));
          }
          else
          {
               m_pLabels[LABEL_NAVI_TANK_STATE] ->setText("");
          }
                   
         switch(DispGetSubWorkState4Pw())
         {
         case DISP_WORK_SUB_IDLE_DEPRESSURE:
             m_pLabels[LABEL_NAVI_UP_STATE]  ->setText(tr("Pressure Relief"));
             if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
             {
                 m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]] ->setText(tr("Pressure Relief"));
             }
             break;
         default:
             m_pLabels[LABEL_NAVI_UP_STATE]  ->setText(tr("Standby"));
             if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
             {
                 m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]] ->setText(tr("Standby"));
             }
            break;
         }
        break;
    case DISP_WORK_STATE_RUN:
        
         m_abDecFlag[APP_DEV_HS_SUB_HYPER] = APP_PACKET_HO_ACTION_STOP;
         m_abDecFlag[APP_DEV_HS_SUB_REGULAR] = APP_PACKET_HO_ACTION_STOP;
         
         if (MACHINE_FUNCTION_UP == (MACHINE_FUNCTION_UP & gaMachineType[gGlobalParam.iMachineType].iFunctions))
         {
             if (DispGetUpQtwFlag())
             {
                 m_pLabels[LABEL_NAVI_UP_STATE]  ->setText(tr("Dispense"));
             }
             else if (DispGetUpCirFlag())
             {
                 m_pLabels[LABEL_NAVI_UP_STATE] ->setText(tr("Recirculation"));
             }
             else 
             {
                 m_pLabels[LABEL_NAVI_UP_STATE]  ->setText(tr("Ready"));
             }
         }
         
         if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
         {
             if (DispGetEdiQtwFlag())
             {
                 m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]] ->setText(tr("Dispense"));
             }
             else 
             {
                 if (NOT_RUNING_STATE_NONE == DispGetRunningStateFlag())
                 {
                     m_pLabels[m_aiLblMap[LABEL_NAVI_EDI_STATE]] ->setText(tr("Ready"));
                 }
             }
         }

         if (DispGetTankCirFlag())
         {
             m_pLabels[LABEL_NAVI_TANK_STATE] ->setText(tr("Recirculation"));
         }
         else
         {
             m_pLabels[LABEL_NAVI_TANK_STATE] ->setText("");
         }
         break;
    }

}


