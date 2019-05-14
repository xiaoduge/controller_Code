#include "maintenancelistwidgtitem.h"
#include <QIntValidator>

#define LEFT_MARGIN       10
#define TOP_MARGIN        10
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   200
#define VALUE_ITEM_WIDTH  150
#define UINT_ITEM_WIDHT   60
#define ITEM_SPACE       (20)

MaintenanceListWidgtItem::MaintenanceListWidgtItem(QWidget *parent,int iItems) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;
    //QRegExp double_rx("([0-9]{0,5})"); 

    m_lbName      = NULL;
    m_lbP1Name    = NULL;
    m_editP1Value = NULL;
    m_lbP2Name    = NULL;
    m_editP2Value = NULL;
    
    
    m_iItems = iItems;
    
    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , MTN_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += NAME_ITEM_WIDTH;
    
    m_editP1Value = new QLineEdit(this);
    m_editP1Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , MTN_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_editP1Value->setAlignment(Qt::AlignCenter);
    m_editP1Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
    xOffset += VALUE_ITEM_WIDTH;

    xOffset += ITEM_SPACE;
    m_lbP1Name = new QLabel(this);
    m_lbP1Name->setGeometry(QRect(xOffset, TOP_MARGIN ,UINT_ITEM_WIDHT , MTN_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbP1Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset+= UINT_ITEM_WIDHT;    
    
    
    if (2 == iItems)
    {
        xOffset = (NAME_ITEM_WIDTH + ITEM_SPACE) + (MTN_LIST_WIDGET_WIDTH - NAME_ITEM_WIDTH - ITEM_SPACE) /2 ;
        
        m_editP2Value = new QLineEdit(this);
        m_editP2Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , MTN_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_editP2Value->setAlignment(Qt::AlignCenter);
        m_editP2Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");

        xOffset += VALUE_ITEM_WIDTH;

        xOffset += ITEM_SPACE;
    
        m_lbP2Name = new QLabel(this);
        m_lbP2Name->setGeometry(QRect(xOffset, TOP_MARGIN ,UINT_ITEM_WIDHT , MTN_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbP2Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    }
}

void MaintenanceListWidgtItem::setP1Range(int iMin,int iMax)
{
    if (m_editP1Value) m_editP1Value->setValidator(new QIntValidator(iMin, iMax, this));

}

void MaintenanceListWidgtItem::setP2Range(int iMin,int iMax)
{
    if (m_editP2Value) m_editP2Value->setValidator(new QIntValidator(iMin, iMax, this));

}

