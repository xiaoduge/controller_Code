#include "Cslistitem.h"
#include "mainwindow.h"
#include "consumablestatepage.h"
#include "cbitmapbutton.h"


CsListItem::CsListItem(QWidget *parent ,ConsumableStatePage *owner,int iId) :
    QWidget(parent),m_pOwner(owner),m_id(iId)
{
    //QPixmap PixWaterValue , PixLightTime ,PixShopCar;
    //PixWaterValue.load(":/pic/WaterValue.png");
    //PixLightTime.load(":/pic/LightTime.png");
    //PixShopCar.load("Channel_Press.png");
    
    this->lbName = new QLabel(this);
    this->lbName->setGeometry(QRect(15 , 15 , 180 , 20)); //120
    //this->lbName->setText("ConsumableStatePage");

    this->lbPic = new QLabel(this);

    QPalette pal(this->palette());

    this->lbPic->setGeometry(QRect(20 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->height()));
    this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]);
    pal.setColor(QPalette::Background , QColor(244,247,255));

    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->lbWaterValue = new QLabel(this);
    this->lbWaterValue->setGeometry(QRect(145 , 15 , 80 , 20)); //85

    this->lbInstDate = new QLabel(this);
    this->lbInstDate->setGeometry(QRect(230 , 15 , 240 , 20));

    this->lbChangeDate = new QLabel(this);
    this->lbChangeDate->setGeometry(QRect(230 , 45 , 240 , 20));

    this->lbCatNo = new QLabel(this);
    this->lbCatNo->setGeometry(QRect(490 , 15 , 180 , 20));

    this->lbLotNo = new QLabel(this);
    this->lbLotNo->setGeometry(QRect(490 , 45 , 180 , 20));

    btnShopCar =  new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,0);

    btnShopCar->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SHOPPING_CART]);
    btnShopCar->setStyleSheet("background-color:transparent");
    btnShopCar->setGeometry(QRect(700 , CS_LIST_WIDGET_HEIGHT/2 - btnShopCar->getPicHeight()/2 ,btnShopCar->getPicWidth(),btnShopCar->getPicHeight()));

    connect(btnShopCar, SIGNAL(clicked(int)), m_pOwner, SLOT(on_btn_clicked(int)));
}

void CsListItem::updateState(int type)
{
    QPalette pal(this->palette());

    if(type)
    {
        this->lbPic->setGeometry(QRect(20 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]->height()));
        this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NOTIFICATION]);

        pal.setColor(QPalette::Background , QColor(244,247,255));
    }
    else
    {
        this->lbPic->setGeometry(QRect(20 , 45 , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->width() , gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]->height()));
        this->lbPic->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_CS_NORMAL]);
        pal.setColor(QPalette::Background , QColor(255,255,255));
    }
}

