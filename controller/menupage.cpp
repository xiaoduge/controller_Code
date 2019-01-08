#include <QDebug>
#include <QString>

#include "menupage.h"

#include "mainwindow.h"

#include "titlebar.h"

#include "waterqualitypage.h"

#include "consumablestatepage.h"

#include "systemmonitorpage.h"

#include "historypage.h"
#include "alarmdisplaypage.h"
#include "sysmsgpage.h"
#include "ex_sysmsgpage.h"
#include "ex_usermanualpage.h"

#include <QPainter>

#define BTNS_PER_ROW (4)

#define BTNS_H_MARGE (10)

#define BTNS_V_MARGE (10)

static QString sPixelmapName[MENU_BTN_NUMBER] = 
{
    ":/pic/menu_water_quality.png",
    ":/pic/menu_consume_material_status.png",
    ":/pic/menu_sys_noti_alarm.png",
    ":/pic/menu_monitor.png",
//    ":/pic/menu_history_record.png",
    ":/pic/menu_user_manu.png",
    ":/pic/menu_sys_info.png",
};

static QString SubPageName[MENU_PAGE_NUM] = 
{
    "Water Quality",
    "Material Status",
    "System Monitor",
    "Alarm",
//    "History",
    "User Manu",
    "System Info",
};


static QPixmap *sTipBmps[MENU_BTN_NUMBER] ;

static CONFIG_BTN1 sBtns[MENU_BTN_NUMBER] = 
{
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
//    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,0,},
};

static CONFIG_BTN_TIP menuBtnTips[MENU_BTN_NUMBER] = 
{
    {&sTipBmps[0],BITMAPBUTTON_ICON_TOPCENTER},
    {&sTipBmps[1],BITMAPBUTTON_ICON_TOPCENTER},
    {&sTipBmps[2],BITMAPBUTTON_ICON_TOPCENTER},
    {&sTipBmps[3],BITMAPBUTTON_ICON_TOPCENTER},
    {&sTipBmps[4],BITMAPBUTTON_ICON_TOPCENTER},
    {&sTipBmps[5],BITMAPBUTTON_ICON_TOPCENTER},
//    {&sTipBmps[6],BITMAPBUTTON_ICON_TOPCENTER},
};


MenuPage::MenuPage(QObject *parent,CBaseWidget *widget,MainWindow *wndMain) : CPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

    Create_subPage();
}

void MenuPage::creatTitle()
{
    CPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void MenuPage::initUi()
{
    int row,col,rows,index;

    int xPos,yPos,yStartPos;

    int itemWidth;

    int x;

    //QPixmap back(":/pic/navigator_bg.png");

    //QSize size(back.width(), back.height());
    
    for (index = 0; index < MENU_BTN_NUMBER; index++)
    {
        sTipBmps[index] = new QPixmap(sPixelmapName[index]);
    }

    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    //p.drawPixmap(0,0,back);

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);


    rows = (MENU_BTN_NUMBER + BTNS_PER_ROW - 1) / BTNS_PER_ROW;

    itemWidth = (width() - BTNS_H_MARGE * 2) / BTNS_PER_ROW;

    yStartPos = height()/2 ;

    for( index = 0 ; index < MENU_BTN_NUMBER ; index++)
    {
        row = index / BTNS_PER_ROW;
        col = index % BTNS_PER_ROW;
        
        m_pBtns[index] = new CBitmapButton(m_widget,sBtns[index].enStyle,sBtns[index].enPicStyle,index);

        if (*sBtns[index].pBackBitmap)
        {
            m_pBtns[index]->setButtonPicture(*sBtns[index].pBackBitmap);
        }

        if (*sBtns[index].pFrontBitmap)
        {
            m_pBtns[index]->setPressPicture(*sBtns[index].pFrontBitmap);
        }

        xPos = BTNS_H_MARGE + col*itemWidth + col*BTNS_H_MARGE;
        
        yPos = yStartPos + (row - (rows/2))*105 - BTNS_V_MARGE/2;
        
        m_pBtns[index]->setGeometry(xPos,yPos,m_pBtns[index]->getPicWidth(),m_pBtns[index]->getPicHeight());

        m_pBtns[index]->setTip(*menuBtnTips[index].pBackBitmap, menuBtnTips[index].enStyle);
        
        m_pBtns[index]->setStyleSheet("background-color:transparent");
        connect(m_pBtns[index], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
        m_pBtns[index]->show();
    }

    x = 400 - PAGEID_MARGIN/2 - PAGEID_MARGIN - gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width()*2;
    for ( index = 0 ; index < 4 ; index++)
    {
        m_pLbPageId[index] = new QLabel(m_widget);
        m_pLbPageId[index]->setGeometry(x ,560,gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width(),gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->height());
        x += (PAGEID_MARGIN + gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width());

        if (index == 1)
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]);
        }
        else
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_UNSELECT]);
        }
    }
        
    m_pTitleBar = new titleBar(m_widget);

    m_pTitleBar->setGeometry(0,0,800,55);
    
    connect(m_pTitleBar, SIGNAL(clicked(int)), this, SLOT(on_navi_clicked(int)));

}

void MenuPage::Create_subPage()
{
    CBaseWidget *tmpWidget;

    int index;
    
    for (index = 0; index < MENU_BTN_NUMBER; index++)
    {
        m_pSubPages[index] = NULL;

        switch(index)
        {
        case MENU_BTN_WATER_QUALITY_PARAMETER:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new WaterQualityPage(this , tmpWidget , m_wndMain);
            
            break;
        case MENU_BTN_CONSUMPATION_MATERAIL_SATUS:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new ConsumableStatePage(this , tmpWidget , m_wndMain);
            
            break;
        case MENU_BTN_SYSTEM_MONITOR:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SystemMonitorPage(this , tmpWidget , m_wndMain);
            break;
/*
        case MENU_BTN_HISTORY_RECORD:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new HistoryPage(this , tmpWidget , m_wndMain);
            break;
*/
        case MENU_BTN_SYSTEM_NOTIFICATION_ALARM:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new AlarmDisplayPage(this , tmpWidget , m_wndMain);
            break;
        case MENU_BTN_SYSTEM_INFORMATION:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
//            m_pSubPages[index] = new SysMsgPage(this , tmpWidget , m_wndMain);
            m_pSubPages[index] = new Ex_SysMsgPage(this , tmpWidget , m_wndMain);
            break;

        case MENU_BTN_USER_MANU:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_UserManualPage(this , tmpWidget , m_wndMain);
            break;
        }
    }

}

void MenuPage::update()
{
}

void MenuPage::buildTitles()
{
    QStringList stringList;
    
    // strictly according to RF_STATE_ENUM
    stringList << tr("Menu");

    setTitles(stringList);

}

void MenuPage::buildTranslation()
{
    int index;
    m_pBtns[MENU_BTN_WATER_QUALITY_PARAMETER]->setTip(tr("Water Quality"),BITMAPBUTTON_TIP_BOTTOMCENTER);
    m_pBtns[MENU_BTN_CONSUMPATION_MATERAIL_SATUS]->setTip(tr("Consumables Status"),BITMAPBUTTON_TIP_BOTTOMCENTER);
    m_pBtns[MENU_BTN_SYSTEM_MONITOR]->setTip(tr("System Status"),BITMAPBUTTON_TIP_BOTTOMCENTER);
    m_pBtns[MENU_BTN_SYSTEM_NOTIFICATION_ALARM]->setTip(tr("Alarm & Alerts"),BITMAPBUTTON_TIP_BOTTOMCENTER);
    m_pBtns[MENU_BTN_USER_MANU]->setTip(tr("User Manual"),BITMAPBUTTON_TIP_BOTTOMCENTER);
    m_pBtns[MENU_BTN_SYSTEM_INFORMATION]->setTip(tr("Information"),BITMAPBUTTON_TIP_BOTTOMCENTER);

    for (index = 0; index < MENU_BTN_NUMBER; index++)
    {
        m_pBtns[index]->setFont(m_wndMain->getFont(GLOBAL_FONT_14));
    }
}

void MenuPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void MenuPage::on_btn_clicked(int index)
{
    printf("tmp = %d\r\n" , index);

    if (m_pSubPages[index])
    {
        show(false);
        
        m_pSubPages[index]->show(true);
    }
    
    m_wndMain->prepareKeyStroke();
 
}

void MenuPage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_curX = e->x();
        m_curY = e->y();
        m_lstFlag = 1;
    }
}

void MenuPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void MenuPage::mouseReleaseEvent(QMouseEvent *e)
{

    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
       
        m_wndMain->naviPage(1,m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}


void MenuPage::on_navi_clicked(int index)
{
    switch(index)
    {
    case TITLE_BAR_ID_POWER_OFF:
        break;
    case TITLE_BAR_ID_BACK:
        break;
    case TITLE_BAR_ID_HOME:
        show(false);
        m_wndMain->home();
        break;
    }
    printf("tmp = %d\r\n" , index);
}

