#include "warnlistwidgtitem.h"
#include <QLabel>

warnListWidgtItem::warnListWidgtItem(QWidget *parent) :
    QWidget(parent)
{
    this->lbType = new QLabel(this);
    this->lbType->setGeometry(QRect(Type_X, Type_Y ,47 , 62));
    this->lbType->clear();

    this->lbMsg = new QLabel(this);
    this->lbMsg->setGeometry(QRect(Msg_X, Msg_Y ,72 , 18));

    QPixmap image_Warn;
    QPixmap image_Remind;

    image_Warn.load(":/pic/Warn.png");
    image_Remind.load(":/pic/Remind.png");
    
    this->lbType ->setPixmap(image_Warn);

    this->lbMsg->setText("Systemwarn");
}

void warnListWidgtItem::WarnClear(warnListWidgtItem *pWarnListItem)
{
    pWarnListItem->lbType->clear();
    pWarnListItem->lbMsg->clear();
}

void warnListWidgtItem::WarnSet(warnListWidgtItem *pWarnListItem )
{
    QPixmap image_Warn; 
    QPixmap image_Remind; 
    
    image_Warn.load(":/pics/Warn.png");
    image_Remind.load(":/pics/Remind.png");
    
    pWarnListItem->lbType ->setPixmap(image_Warn);

    pWarnListItem->lbMsg->setText("test");
}

