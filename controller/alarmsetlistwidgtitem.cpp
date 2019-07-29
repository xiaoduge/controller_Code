#include "alarmsetlistwidgtitem.h"
#include "systestlistwidgtitem.h"
#include "mainwindow.h"
#include "alarmsetpage.h"


#define LEFT_MARGIN       5
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   300
#define VALUE_ITEM_WIDTH  80
#define ITEM_SPACE        (100)

AlarmSetListwidgtItem::AlarmSetListwidgtItem(QWidget *parent,AlarmSetPage *owner,int iType,int id) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;

    m_pBtnSwitch= NULL;

    m_iType  = iType ;
    m_pOwner = owner;
    m_iId    = id;

    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(LEFT_MARGIN, TOP_MARGIN ,NAME_ITEM_WIDTH , ALARM_SET_LIST_WIDGET_HEIGHT - TOP_MARGIN*2));
    xOffset += NAME_ITEM_WIDTH;

    xOffset += ITEM_SPACE;

    m_pBtnSwitch = new CBitmapButton(this,BITMAPBUTTON_STYLE_SWITCH,BITMAPBUTTON_PIC_STYLE_NORMAL,m_iId);
    m_pBtnSwitch->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SWITCH_OFF]);
    m_pBtnSwitch->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_SWITCH_ON]);
    m_pBtnSwitch->setGeometry(SYSTEM_TEST_LIST_WIDGET_WIDTH - 50 - m_pBtnSwitch->getPicWidth(),ALARM_SET_LIST_WIDGET_HEIGHT/2 - m_pBtnSwitch->getPicHeight()/2,m_pBtnSwitch->getPicWidth(),m_pBtnSwitch->getPicHeight());
    m_pBtnSwitch->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSwitch, SIGNAL(clicked(int)), owner, SLOT(on_btn_clicked(int)));

}


