#include "setplistwidgtitem.h"

#include <QRegExpValidator>

#define LEFT_MARGIN       5
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   240
#define VALUE_ITEM_WIDTH  150
#define UNIT_ITEM_WIDTH   60
#define ITEM_SPACE       (20)

SetPlistwidgtitem::SetPlistwidgtitem(QWidget *parent,int iItems) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;
    QRegExp double_rx("([0-9]{0,2}[\.][0-9]{1,1})"); 

    
    m_iItems = iItems;

    m_lbName      = NULL;
    
    m_lbP1Name    = NULL;
    m_editP1Value = NULL;
    m_lbP1Unit    = NULL;
    
    m_lbP2Name    = NULL;
    m_editP2Value = NULL;
    m_lbP2Unit    = NULL;

    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += NAME_ITEM_WIDTH;

    xOffset += ITEM_SPACE;
    m_lbP1Name = new QLabel(this);
    m_lbP1Name->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
    m_lbP1Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    m_editP1Value = new QLineEdit(this);
    m_editP1Value->setGeometry(QRect(xOffset, TOP_MARGIN + (SET_P_LIST_WIDGET_HEIGHT/2) ,VALUE_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
    m_editP1Value->setAlignment(Qt::AlignCenter);
    m_editP1Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
    xOffset += VALUE_ITEM_WIDTH;
    m_editP1Value->setValidator(new QRegExpValidator(double_rx,this));

    xOffset += 5;
    m_lbP1Unit = new QLabel(this);
    m_lbP1Unit->setGeometry(QRect(xOffset, TOP_MARGIN + (SET_P_LIST_WIDGET_HEIGHT/2) ,UNIT_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
    m_lbP1Unit->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += UNIT_ITEM_WIDTH;
    
    if (SET_POINT_FORMAT2 == iItems)
    {
        xOffset += ITEM_SPACE;
        m_lbP2Name = new QLabel(this);
        m_lbP2Name->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
        m_lbP2Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        
        m_editP2Value = new QLineEdit(this);
        m_editP2Value->setGeometry(QRect(xOffset, TOP_MARGIN + (SET_P_LIST_WIDGET_HEIGHT/2) ,VALUE_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
        m_editP2Value->setAlignment(Qt::AlignCenter);
        m_editP2Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
        m_editP2Value->setValidator(new QRegExpValidator(double_rx,this));
        xOffset += VALUE_ITEM_WIDTH;

        xOffset += 5;
        m_lbP2Unit = new QLabel(this);
        m_lbP2Unit->setGeometry(QRect(xOffset, TOP_MARGIN + (SET_P_LIST_WIDGET_HEIGHT/2) ,UNIT_ITEM_WIDTH , SET_P_LIST_WIDGET_HEIGHT/2 - 2*TOP_MARGIN));
        m_lbP2Unit->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += UNIT_ITEM_WIDTH;
        
    }
}
