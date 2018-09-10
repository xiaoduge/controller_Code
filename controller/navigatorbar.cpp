#include "navigatorbar.h"
#include <QtGui/qpainter.h>
#include "mainwindow.h"

navigatorBar::navigatorBar(QWidget *parent) :
    QWidget(parent)
{

    m_pNaviBar = gpGlobalPixmaps[GLOBAL_BMP_NAVIGATOR_BG];
    m_pTimeBar = gpGlobalPixmaps[GLOBAL_BMP_NAVIGATOR_TIMER_BAR];

    m_pDateFont = new QFont("" , 14 ,  QFont::Bold);
    m_pTimeFont = new QFont("" , 24 ,  QFont::Bold);
    
    m_pLabelDate = new QLabel(this);
    m_pLabelDate->setGeometry(360,5,80,15);
    m_pLabelDate->setFont(*m_pDateFont);

    m_pLabelTime = new QLabel(this);
    m_pLabelTime->setGeometry(360,25,80,20);
    m_pLabelTime->setFont(*m_pTimeFont);

    m_pBtnPower = new CBitmapButton(this,BITMAPBUTTON_STYLE_SWITCH,BITMAPBUTTON_PIC_STYLE_NORMAL,NBAR_ID_POWER_OFF);

    m_pBtnPower->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_RUN]);

    m_pBtnPower->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_STOP]);

    m_pBtnPower->setGeometry(745,2,m_pBtnPower->getPicWidth(),m_pBtnPower->getPicHeight());

    m_pBtnPower->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnPower, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    m_pBtnPower->show();

    m_timeTimer = new QTimer(this);
    connect(m_timeTimer, SIGNAL(timeout()), this, SLOT(on_timerEvent()));
    m_timeTimer->start(1000); // peroid of one minute   
    m_timerCnt = 0;
    m_iDuration = 5;

    showTime();
}

void navigatorBar::on_timerEvent()
{
    m_timerCnt++;

    if (m_timerCnt >= m_iDuration)
    {
        m_timeTimer->stop(); // peroid of one minute    
        hide();
    }
    else
    {
        /* update time info */
        showTime();
    }
}

void navigatorBar::display(bool bShow)
{
    if (bShow)
    {
        m_timeTimer->start(1000 );
        m_timerCnt = 0;
        show();
    }
    else
    {
        m_timeTimer->stop(); 
        hide();
    }
}

void navigatorBar::updateRunInfo(bool bRun)
{
    m_pBtnPower->setState(bRun ? BITMAPBUTTON_STATE_SEL : BITMAPBUTTON_STATE_UNSEL );
}

bool navigatorBar::getState()
{
    
    bool stateRun = (m_pBtnPower->getState() == BITMAPBUTTON_STATE_SEL);

    return stateRun;
}

void navigatorBar::paintEvent(QPaintEvent *e)
{

    QWidget::paintEvent(e);

    QPainter painter(this);

    if (m_pNaviBar)
    {
        painter.drawPixmap(rect().x(),rect().y(),m_pNaviBar->width(),m_pNaviBar->height(),*m_pNaviBar);
    }
    
    if (m_pTimeBar)
    {
        painter.drawPixmap(rect().x() + rect().width()/2 - m_pTimeBar->width() / 2,rect().y(),m_pTimeBar->width(),m_pTimeBar->height(),*m_pTimeBar);
    }
}

void navigatorBar::on_btn_clicked(int id)
{
    m_timerCnt = 0;

    emit clicked(id); 
}

void navigatorBar::showTime()
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    QString astrWeekday[] = {
        tr("Mon"),
        tr("Tue"),
        tr("Wed"),
        tr("Thu"),
        tr("Fri"),
        tr("Sat"),
        tr("Sun"),
    };

    //int day,mon,week,hour,min;
    int week;

    QString strDate;

    //day  = sysDateTime.date().day();
    // mon  = sysDateTime.date().month();
    week = sysDateTime.date().dayOfWeek () - 1;

    m_pLabelTime->setText(sysDateTime.toString("hh:mm"));

    strDate = QDateTime::currentDateTime().toString("MM-dd");

    strDate = astrWeekday[week] + " " + strDate;
    
    m_pLabelDate->setText(strDate);

}

