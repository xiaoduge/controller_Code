#include <QMouseEvent>
#include "cbitmapbutton.h"
#include "menupage.h"
#include "mainwindow.h"
#include "titlebar.h"
#include "consumablestatepage.h"
#include "systemmonitorpage.h"
#include "alarmdisplaypage.h"
#include "dsysmsgpage.h"
#include "dusermanualpage.h"
#include "dwaterqualitypage.h"
#include "exconfig.h"
#include "dpushbutton.h"
#include "setpage.h"


#define BTNS_PER_ROW (4)

#define BTNS_H_MARGE (15)

#define BTNS_V_MARGE (10)

static QString SubPageName[MENU_PAGE_NUM] = 
{
    "Water Quality",
    "Material Status",
    "System Monitor",
    "Alarm",
    "User Manu",
    "System Info",
};

static ButtonInfo Btninfos[MENU_BTN_NUMBER] =
{
    {QObject::tr("Water Quality"), QString(":/ButtonIcon/Water Quality.png")},
    {QObject::tr("Consumables Status"), QString(":/ButtonIcon/consumables status.png")},
    {QObject::tr("System Status"), QString(":/ButtonIcon/system status.png")},
    {QObject::tr("Alarm & Alerts"), QString(":/ButtonIcon/alarm alert.png")},
    {QObject::tr("User Manual"), QString(":/ButtonIcon/user manual.png")},
    {QObject::tr("Information"), QString(":/ButtonIcon/about.png")},
};

MenuPage::MenuPage(QObject *parent,CBaseWidget *widget,MainWindow *wndMain) : CPage(parent,widget,wndMain)
{
    initUi();
    creatTitle();
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

    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255)); //238, 223, 204

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);


    rows = (MENU_BTN_NUMBER + BTNS_PER_ROW - 1) / BTNS_PER_ROW;

    itemWidth = (width() - BTNS_H_MARGE * 2) / BTNS_PER_ROW;

    yStartPos = height()/2  - 60;

    for( index = 0 ; index < MENU_BTN_NUMBER ; index++)
    {
        row = index / BTNS_PER_ROW;
        col = index % BTNS_PER_ROW;

        m_pBtns[index] = new DPushButton(Btninfos[index].m_fileName, Btninfos[index].m_text, m_widget, index);

        xPos = BTNS_H_MARGE + col*itemWidth + col*BTNS_H_MARGE + 60;

        yPos = yStartPos + (row - (rows/2))*120 - BTNS_V_MARGE/2;

        m_pBtns[index]->setMoveRule(DPushButton::CenterMove);
        m_pBtns[index]->cmove(xPos, yPos);

        connect(m_pBtns[index], SIGNAL(clicked()), this, SLOT(on_btn_clicked()));
    }

    x = 400 - PAGEID_MARGIN/2 - PAGEID_MARGIN - gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width()*2;
    for ( index = 0 ; index < 3 ; index++)
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
            //m_pSubPages[index] = new WaterQualityPage(this , tmpWidget , m_wndMain);
            m_pSubPages[index] = new DWaterQualityPage(this , tmpWidget , m_wndMain);
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
            m_pSubPages[index] = new DSysMsgPage(this , tmpWidget , m_wndMain);
            break;

        case MENU_BTN_USER_MANU:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new DUserManualPage(this , tmpWidget , m_wndMain);
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

    stringList << tr("Menu");

    setTitles(stringList);

}

void MenuPage::buildTranslation()
{
    m_pBtns[MENU_BTN_WATER_QUALITY_PARAMETER]->setText(tr("Water Quality"));
    m_pBtns[MENU_BTN_CONSUMPATION_MATERAIL_SATUS]->setText(tr("Consumables Status"));
    m_pBtns[MENU_BTN_SYSTEM_MONITOR]->setText(tr("System Status"));
    m_pBtns[MENU_BTN_SYSTEM_NOTIFICATION_ALARM]->setText(tr("Alarm & Alerts"));
    m_pBtns[MENU_BTN_USER_MANU]->setText(tr("User Manual"));
    m_pBtns[MENU_BTN_SYSTEM_INFORMATION]->setText(tr("Information"));

    for(int i = 0; i < MENU_BTN_NUMBER; ++i)
    {
        m_pBtns[i]->setFontSize(13);
    }
}

void MenuPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void MenuPage::on_btn_clicked()
{
    QObject* obj = sender();
    DPushButton* button = qobject_cast<DPushButton*>(obj);
    int index = button->id();

    if(0 != gAdditionalCfgParam.productInfo.iCompany)
    {
        if(MENU_BTN_USER_MANU == index)
        {
            return;
        }
    }

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

void MenuPage::goAlarm()
{
    if (m_pSubPages[MENU_BTN_SYSTEM_NOTIFICATION_ALARM])
    {
        show(false);
        m_pSubPages[MENU_BTN_SYSTEM_NOTIFICATION_ALARM]->show(true);
    }
}

void MenuPage::mouseReleaseEvent(QMouseEvent *e)
{

    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
#ifdef FLOWCHART
        m_wndMain->naviPage(2,m_curX - m_lstX > 0 ? 1 : 0);
#else
        m_wndMain->naviPage(1,m_curX - m_lstX > 0 ? 1 : 0);
#endif

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
}

