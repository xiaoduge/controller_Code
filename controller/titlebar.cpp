#include "titlebar.h"
#include <QtGui/qpainter.h>
#include "mainwindow.h"

titleBar::titleBar(QWidget *parent,int style) :
    QWidget(parent)
{

    m_pBarSeperator = NULL;

    m_pNaviBar = gpGlobalPixmaps[GLOBAL_BMP_NAVIGATOR_BG];

    //m_pBtnPower = new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,TITLE_BAR_ID_POWER_OFF);

    //m_pBtnPower->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_POWER_OFF_ACTIVE]);

    //m_pBtnPower->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_POWER_OFF_INACT]);

    //m_pBtnPower->setGeometry(745,3,40,40);

    //m_pBtnPower->setStyleSheet("background-color:transparent");
    
    //connect(m_pBtnPower, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    //m_pBtnPower->show();

    /* back */

    /* home */
    m_pBtnHome = new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,TITLE_BAR_ID_HOME);

    m_pBtnHome->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_HOME]);

    m_pBtnHome->setGeometry(5,3,m_pBtnHome->getPicWidth(),m_pBtnHome->getPicHeight());

    m_pBtnHome->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnHome, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    m_pBtnHome->show();

    if (style)
    {
    
        QRect rect = m_pBtnHome->geometry();
    
        m_pBtnBack = new CBitmapButton(this,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,TITLE_BAR_ID_BACK);
        
        m_pBtnBack->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BACK]);
        
        m_pBtnBack->setGeometry(rect.x() + rect.width() + 2 + gpGlobalPixmaps[GLOBAL_BMP_BAR_SEPERATOR]->width(),3,m_pBtnBack->getPicWidth(),m_pBtnBack->getPicHeight());
        
        m_pBtnBack->setStyleSheet("background-color:transparent");
        
        connect(m_pBtnBack, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
        
        m_pBtnBack->show(); 

        m_pBarSeperator = gpGlobalPixmaps[GLOBAL_BMP_BAR_SEPERATOR];
    }

    m_style = style;

}

void titleBar::display(bool bShow)
{
    if (bShow)
    {
        show();
    }
    else
    {
        hide();
    }
}

void titleBar::paintEvent(QPaintEvent *e)
{
    // int iTextX = 0;
    
    QWidget::paintEvent(e);

    QPainter painter(this);

    if (m_pNaviBar)
    {
//        painter.drawPixmap(rect().x(),rect().y(),m_pNaviBar->width(),m_pNaviBar->height(),*m_pNaviBar);
        painter.setBrush(QColor(240, 240, 240));
        painter.setPen(Qt::transparent);
        painter.drawRect(0, 0, 800, 55);
    }

    if (m_pBarSeperator)
    {
        QRect rect = m_pBtnHome->geometry();
    
        painter.drawPixmap(rect.x() + rect.width() + 1,rect.y(),m_pBarSeperator->width(),m_pBarSeperator->height(),*m_pBarSeperator);
    }
}

void titleBar::on_btn_clicked(int id)
{
    emit clicked(id); 
}


