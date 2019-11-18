#include "consumableinslistwidgtitem.h"
#include "mainwindow.h"
#include "ConsumableinsPage.h"


#define LEFT_MARGIN       10
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5

#define NAME_ITEM_WIDTH   180
#define VALUE_ITEM_WIDTH  150
#define ITEM_SPACE       (20)
#define SHORT_ITEM_WIDTH  60

#define ITEM_MAX_CHARS (9)

CosumableInsListWidgtItem::CosumableInsListWidgtItem(QWidget *parent,ConsumableInsPage *owner,int iId) :
    QWidget(parent),m_owner(owner),m_iId(iId)
{
    int xOffset = LEFT_MARGIN;
    
    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , CI_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += NAME_ITEM_WIDTH;

    m_lbCatName = new QLabel(this);
    m_lbCatName->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , CI_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbCatName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += SHORT_ITEM_WIDTH;
    
    m_editP1Value = new DLineEdit(this);
    m_editP1Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , CI_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_editP1Value->setAlignment(Qt::AlignCenter);
    m_editP1Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
    m_editP1Value->setMaxLength (ITEM_MAX_CHARS);
    xOffset += VALUE_ITEM_WIDTH;

    m_lbLotName = new QLabel(this);
    m_lbLotName->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , CI_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbLotName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += SHORT_ITEM_WIDTH;
    
    m_editP2Value = new DLineEdit(this);
    m_editP2Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , CI_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_editP2Value->setAlignment(Qt::AlignCenter);
    m_editP2Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
//    m_editP2Value->setMaxLength (ITEM_MAX_CHARS);
    m_editP2Value->setMaxLength (10);//0704
    xOffset += VALUE_ITEM_WIDTH;

    xOffset += ITEM_SPACE;
    xOffset += VALUE_ITEM_WIDTH;    

    m_btnInstall = new CBitmapButton(this,BITMAPBUTTON_STYLE_SWITCH,BITMAPBUTTON_PIC_STYLE_SINGLE,iId);
    m_btnInstall->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    m_btnInstall->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    m_btnInstall->setGeometry(CI_LIST_WIDGET_WIDTH - 100,CI_LIST_WIDGET_HEIGHT/2 - m_btnInstall->getPicHeight()/2 ,m_btnInstall->getPicWidth(),m_btnInstall->getPicHeight());
    m_btnInstall->setStyleSheet("background-color:transparent");
    m_btnInstall->show();
    
    connect(m_btnInstall, SIGNAL(clicked(int)), m_owner, SLOT(on_btn_clicked(int)));
}

