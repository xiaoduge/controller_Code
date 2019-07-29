#include "msgbox.h"
#include <QRect>
#include <QLabel>
#include "cbitmapbutton.h"

MsgBox::MsgBox(QWidget *parent) :
    QDialog(parent)
{
    this->resize(320, 210);
    this->setGeometry(QRect(240,150,320,210));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setStyleSheet("background-color:#ffffff;border:1px solid #dfe7ed");

    lbmsg = new QLabel(this);
    lbmsg->setGeometry(QRect(20,20,280,28));
    lbmsg->setAlignment(Qt::AlignCenter);
    lbmsg->setStyleSheet(" font-size:18pt;color:#16181e;font-family:Arial;QFont::Bold;border:0px");

    CancelBtn = new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,0);
    CancelBtn->setButtonPicture(new QPixmap(":/pic/Cancle.png"));
    CancelBtn->setPressPicture(new QPixmap(":/pic/Cancle_p.png"));
    CancelBtn->setTip("Cancle");
    CancelBtn->setGeometry(QRect(60 , 150 ,CancelBtn->width(),CancelBtn->height()));
    CancelBtn->setStyleSheet("background-color:transparent");

    connect(CancelBtn, SIGNAL(clicked(int)), this, SLOT(on_Cal_clicked(int)));

    SaveBtn   = new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,0);
    SaveBtn->setButtonPicture(new QPixmap(":/pic/ok.png"));
    SaveBtn->setPressPicture(new QPixmap(":/pic/ok_p.png"));
    SaveBtn->setTip("ok");
    SaveBtn->setGeometry(QRect(182 , 150 ,SaveBtn->width(),SaveBtn->height()));
    SaveBtn->setStyleSheet("background-color:transparent");

    connect(SaveBtn, SIGNAL(clicked(int)), this, SLOT(on_Ok_clicked(int)));
}

void MsgBox::setInfo(QString info)
{
    lbmsg->setText(info);
}

void MsgBox::on_Ok_clicked(int)
{
    this->accept();
}

void MsgBox::on_Cal_clicked(int)
{
    this->reject();
}
