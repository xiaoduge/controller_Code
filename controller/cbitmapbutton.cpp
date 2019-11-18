#include "cbitmapbutton.h"
#include <QPainter>
#include "ExtraDisplay.h"

CBitmapButton::CBitmapButton(QWidget *parent,int style,int bgStyle,int id) :
    QWidget(parent)
{
    pressPicture  = NULL;
    buttonPicture = NULL;
    curPicture    = NULL;
    pmIcon        = NULL;
    btnStyle      = style;
    btnState      = BITMAPBUTTON_STATE_UNSEL;
    iBtnId        =  id;
    bEnable       = true;
    titleFont     = NULL;
    picStyle      = bgStyle;
    txtposType    = BITMAPBUTTON_TIP_CENTER;
}

void CBitmapButton::setIcon(QPixmap *pic)
{
    curPicture = pic;
    update();
}

void CBitmapButton::mousePressEvent(QMouseEvent *e)
{
    if(g_isScreenSleep) return;
    if (!bEnable) return ;

    switch(btnStyle)
    {
    case BITMAPBUTTON_STYLE_PUSH:
        this->setIcon (pressPicture);
        emit clicking(iBtnId);
        break;
    case BITMAPBUTTON_STYLE_CHECK:
        this->setIcon (pressPicture);
        break;
    case BITMAPBUTTON_STYLE_SWITCH:
        if (btnState)
        {
            this->setIcon(buttonPicture);
        }
        else
        {
            this->setIcon(pressPicture);
        }
        break;
  } 
  //parentWidget()->update(geometry());  
}

/*!
    \reimp
*/
void CBitmapButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(g_isScreenSleep) return;
    if (!bEnable) return ;

    switch(btnStyle)
    {
    case BITMAPBUTTON_STYLE_PUSH:
        this->setIcon(buttonPicture);
        emit clicked(iBtnId);
        break;
    case BITMAPBUTTON_STYLE_CHECK:
        if (btnState != BITMAPBUTTON_STATE_SEL)
        {
            btnState = BITMAPBUTTON_STATE_SEL;
            emit clicked(iBtnId);
        }
        break;
    case BITMAPBUTTON_STYLE_SWITCH:
        btnState = !btnState;
        if (btnState)
        {
            this->setIcon(pressPicture);
        }
        else
        {
            this->setIcon(buttonPicture);
        }
        emit clicked(iBtnId);
        break;
    case BITMAPBUTTON_STYLE_NUM:
        break;
  }
  //parentWidget()->update(geometry());  
  //this->setIconSize(QSize(buttonPicture->width(), buttonPicture->height()));
}

void CBitmapButton::setButtonPicture(QPixmap *pic)  
{  
    buttonPicture = pic;  
      
    this->setIcon(buttonPicture);
}  
  
void CBitmapButton::setPressPicture(QPixmap *pic)  
{  
    pressPicture = pic;  
}  

void CBitmapButton::paintEvent(QPaintEvent *e)
{
    int iTextX = 0,iTextY = 0;

    QRect rectText = rect();
    
    QWidget::paintEvent(e);

    QPainter painter(this);

    painter.setBrush(Qt::transparent); //ex

    if (curPicture)
    {
        painter.drawPixmap(rect().x(),rect().y(),curPicture->width(),curPicture->height(),*curPicture);
    }
    else 
    {
        if (BITMAPBUTTON_PIC_STYLE_NORMAL == picStyle)
        {
            if (buttonPicture) painter.drawPixmap(rect().x() - 2,rect().y() - 2,buttonPicture->width(),buttonPicture->height(),*buttonPicture);
            else if (pressPicture) painter.drawPixmap(rect().x() - 2,rect().y() - 2,pressPicture->width(),pressPicture->height(),*pressPicture);
        }
    }

    iTextX = rect().x();
    iTextY = rect().y();
    
    if (pmIcon != NULL)
    {
        switch(iconposType)
        {
        case BITMAPBUTTON_ICON_TOPLEFT:
            painter.drawPixmap(rect().x(),rect().y(),pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = iTextX + pmIcon->width() ;
            rectText.setX(iTextX);
            break;
        case BITMAPBUTTON_ICON_CENTER:
            painter.drawPixmap(rect().x()+rect().width()/2-pmIcon->width()/2,rect().y()+rect().height()/2-pmIcon->height()/2,pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = rect().x()+rect().width()/2-pmIcon->width()/2 + pmIcon->width();
            break;
        case BITMAPBUTTON_ICON_VCENTER:
            painter.drawPixmap(rect().x(),rect().y()+rect().height()/2-pmIcon->height()/2,pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = iTextX +pmIcon->width() ;
            rectText.setX(iTextX);
            break;
        case BITMAPBUTTON_ICON_TOPCENTER:
            painter.drawPixmap(rect().x()+rect().width()/2-pmIcon->width()/2,rect().y(),pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextY = rect().y() + pmIcon->height() ;
            rectText.setY(iTextY);
            break;
        case BITMAPBUTTON_ICON_BOTTOMCENTER:
            painter.drawPixmap(rect().x()+rect().width()/2-pmIcon->width()/2,rect().y() + rect().height() - pmIcon->height() / 2,pmIcon->width(),pmIcon->height(),*pmIcon);
            rectText.setBottom(rect().y()+rect().height() - pmIcon->height());
            break;
        case BITMAPBUTTON_ICON_BCENTER:
            painter.drawPixmap(rect().x()+rect().width()/2-pmIcon->width()/2,rect().y()+rect().height()/2-pmIcon->height()/2,pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = rect().x()+rect().width()/2-pmIcon->width()/2 + pmIcon->width();
            break;
        case BITMAPBUTTON_ICON_COCENTER:
            painter.drawPixmap(rect().x()+rect().width()/2-pmIcon->width()/2,rect().y()+rect().height()/2-pmIcon->height()/2,pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = rect().x()+rect().width()/2-pmIcon->width()/2 + pmIcon->width();
            break;
        default:
            painter.drawPixmap(rect().x(),rect().y()+rect().height()/2-pmIcon->height()/2,pmIcon->width(),pmIcon->height(),*pmIcon);
            iTextX = rect().x() + pmIcon->width() ;
            rectText.setX(iTextX);
            break;
        }

    }
    {
        QString strDst = (BITMAPBUTTON_STATE_UNSEL == btnState) ? strTip : strSelectTip;
        
        QColor colorDst = (BITMAPBUTTON_STATE_UNSEL == btnState) ? colorText : colorSelectText;
        
        if (!strDst.isEmpty())
        {
            if (titleFont)
            {
                painter.setFont(*titleFont);
            }
            
            painter.setPen(colorDst);

            switch(txtposType)
            {
            case BITMAPBUTTON_ICON_BCENTER:
                {
                    rectText.setY(rectText.bottom()-52);
                    rectText.setX(rectText.x()-10);
//                    painter.drawText(rectText,Qt::AlignCenter,strDst);

                    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
                    option.setWrapMode(QTextOption::WordWrap);
                    painter.drawText(rect(), strDst, option);
                }
                break;
            case BITMAPBUTTON_ICON_COCENTER:
                {
                    rectText.setY(rectText.y()+12);
                    rectText.setX(rectText.x());
//                    painter.drawText(rectText,Qt::AlignCenter,strDst);
                    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
                    option.setWrapMode(QTextOption::WordWrap);
                    painter.drawText(rect(), strDst, option);
                }
                break;
            case BITMAPBUTTON_TIP_BOTTOMCENTER:
                {
                    QFontMetrics fm = painter.fontMetrics();
                    rectText.setY(rectText.bottom() - (fm.ascent()+fm.descent()) - rect().height() / 5);
                    painter.drawText(rectText,Qt::AlignCenter,strDst);
                }
                break;
           default:
                QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
                option.setWrapMode(QTextOption::WordWrap);
                painter.drawText(rect(), strDst, option);
                break;
            }
        }
    }
}

void CBitmapButton::setTip(QString str,QColor color,QPixmap *pIcon,int iconpos,int txtpos)
{
    strTip = str;
    colorText = color;

    strSelectTip = str;
    colorSelectText = color;
    
    pmIcon = pIcon;
    iconposType = iconpos;
    txtposType = txtpos;
}

void CBitmapButton::setTip(QPixmap *pIcon,int iconpos)
{
    pmIcon = pIcon;
    iconposType = iconpos;
}

void CBitmapButton::setTip(QString str,int txtpos)
{
    strTip = str;
    strSelectTip = str;
    txtposType = txtpos;
}

void CBitmapButton::setTip(QString str,QColor color,int txtpos)
{
    strTip = str;
    txtposType = txtpos;
    colorText = color;
    strSelectTip = str;
    colorSelectText = color;
}

void CBitmapButton::setState(int state)
{
    if (state == btnState)
    {
        return ;
    }

    if (BITMAPBUTTON_STYLE_PUSH == btnStyle )
    {
        return ;
    }

    btnState = state;

    
    if (BITMAPBUTTON_STATE_UNSEL == btnState)
    {
        this->setIcon(buttonPicture);
    }
    else
    {
        this->setIcon(pressPicture);
    }
    
}


void CBitmapButton::toggleState()
{

    if (BITMAPBUTTON_STYLE_PUSH == btnStyle )
    {
        return ;
    }
    
    if (BITMAPBUTTON_STATE_UNSEL == btnState)
    {
        btnState = BITMAPBUTTON_STATE_SEL;
        this->setIcon(pressPicture);
    }
    else
    {
        btnState = BITMAPBUTTON_STATE_UNSEL;
        this->setIcon(buttonPicture);
    }
    
}

void CBitmapButton::enable(bool bEnable)
{
    this->bEnable = bEnable;
}

void CBitmapButton::setText(QString str,int state)
{
    switch(state)
    {
    default:
        strTip = str;
        break;
    case BITMAPBUTTON_STATE_SEL:
        strSelectTip = str;
        break;
    }
}

void CBitmapButton::setColor(QColor color,int state)
{
    switch(state)
    {
    default:
        colorText = color;
        break;
    case BITMAPBUTTON_STATE_SEL:
        colorSelectText = color;
        break;
    }
}

