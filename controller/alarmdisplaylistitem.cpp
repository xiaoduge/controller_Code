#include "Cslistitem.h"
#include <QPixmap>
#include "mainwindow.h"
#include "alarmdisplaylistitem.h"

#define ADCS_LEFT_MARGIN 5

AdcsListItem::AdcsListItem(QWidget *parent ,AlarmDisplayPage *owner) :
    QWidget(parent),m_pOwner(owner)
{

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background , QColor(244,247,255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->lbName = new QLabel(this);
    this->lbName->setGeometry(QRect(ADCS_LEFT_MARGIN , 15 , 120 , 20));

    this->lbPic = new QLabel(this);
    this->lbPic->setGeometry(QRect(ADCS_LEFT_MARGIN + 5 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->height()));
    this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]);

    this->lbWaterValue = new QLabel(this);
    this->lbWaterValue->setGeometry(QRect(ADCS_LEFT_MARGIN + 70 , 15 , 50 , 20));
    //this->lbWaterValue->setText("3000L");

    this->lbInstDate = new QLabel(this);
    this->lbInstDate->setGeometry(QRect(160 , 15 , 200 , 20));
    //this->lbInstDate->setText("InstDate:2017.10.10");

    this->lbChangeDate = new QLabel(this);
    this->lbChangeDate->setGeometry(QRect(160 , 45 , 200 , 20));
    //this->lbChangeDate->setText("ChangeDate:2018.10.10");

    this->lbCatNo = new QLabel(this);
    this->lbCatNo->setGeometry(QRect(ADCS_LEFT_MARGIN , 75 , 160 , 20));
    //this->lbCatNo->setText("CAT NO:123456");

    this->lbLotNo = new QLabel(this);
    this->lbLotNo->setGeometry(QRect(160 , 75 , 160 , 20));
    //this->lbLotNo->setText("LOT NO: 789456");
    m_id = 0XFF;

}

void AdcsListItem::updateState(int type)
{
    QPalette pal(this->palette());

    if(type)
    {
        this->lbPic->setGeometry(QRect(ADCS_LEFT_MARGIN + 5 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]->height()));
        this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]);
        pal.setColor(QPalette::Background , QColor(244,247,255));
    }
    else
    {
        this->lbPic->setGeometry(QRect(ADCS_LEFT_MARGIN + 5 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->height()));
        this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]);
        pal.setColor(QPalette::Background , QColor(255,255,255));
    }

}

#define LEFT_MARGIN       5
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   300
#define VALUE_ITEM_WIDTH  80
#define ITEM_SPACE        (100)

AdasListItem::AdasListItem(QWidget *parent,AlarmDisplayPage *owner) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;

    m_pOwner = owner;
    m_iId    = 0XFF;

    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(LEFT_MARGIN, TOP_MARGIN ,NAME_ITEM_WIDTH , ADAS_LIST_WIDGET_HEIGHT - TOP_MARGIN*2));
    xOffset += NAME_ITEM_WIDTH;

}

