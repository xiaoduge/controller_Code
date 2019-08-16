#include "ex_init_timepage.h"
#include "mainwindow.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>
#include <time.h>

#include <QCalendarWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "cbitmapbutton.h"

Ex_Init_TimePage::Ex_Init_TimePage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

    this->hideTitleBar();
}

void Ex_Init_TimePage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_Init_TimePage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Time & Date");

    setTitles(stringList);

}

void Ex_Init_TimePage::buildTranslation()
{
    int iLoop;

    m_astrDateName[0] = tr("Select Date");
    m_astrDateName[1] = tr("Select Time");

    for( iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        lbName[iLoop]->setText(m_astrDateName[iLoop]);
    }

    lbTitName->setText(tr("Time & Date"));
    m_pBtns[TIMEPAGE_BTN_OK]->setTip(tr("OK"));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setTip(tr("Cancel"));

    m_pExNextBtn->setText(tr("Next"));
    m_pExBackBtn->setText(tr("Back"));
}

void Ex_Init_TimePage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}



void Ex_Init_TimePage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void Ex_Init_TimePage::createHeads()
{
    QPixmap PixTit;
    PixTit.load(":/pic/time.png");

    lbTitPic = new QLabel(m_widget);
    lbTitPic->setGeometry(QRect(150, 130 , 28 , 28));
    lbTitPic->setPixmap(PixTit);

    lbTitName = new QLabel(m_widget);
    lbTitName->setGeometry(QRect(210, 135 , 300 , 28));
    lbTitName->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");
}

void Ex_Init_TimePage::Create_Cal_Ok()
{
    m_pBtns[TIMEPAGE_BTN_CANCEL] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,TIMEPAGE_BTN_CANCEL);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_NORMAL]);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_ACTIVE]);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setGeometry(QRect(230 , 450 ,m_pBtns[TIMEPAGE_BTN_CANCEL]->getPicWidth(),m_pBtns[TIMEPAGE_BTN_CANCEL]->getPicHeight()));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setStyleSheet("background-color:transparent");
    m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_CANCEL], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    m_pBtns[TIMEPAGE_BTN_OK]   = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,TIMEPAGE_BTN_OK);
    m_pBtns[TIMEPAGE_BTN_OK]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_NORMAL]);
    m_pBtns[TIMEPAGE_BTN_OK]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_ACTIVE]);
    m_pBtns[TIMEPAGE_BTN_OK]->setGeometry(QRect(430 , 450 ,m_pBtns[TIMEPAGE_BTN_OK]->getPicWidth(),m_pBtns[TIMEPAGE_BTN_OK]->getPicHeight()));
    m_pBtns[TIMEPAGE_BTN_OK]->setStyleSheet("background-color:transparent");
    m_pBtns[TIMEPAGE_BTN_OK]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_OK], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
}

void Ex_Init_TimePage::Create_Date_Time()
{
    int iLoop;

    int tmp;

    for(iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        ShowWidget[iLoop] = new QWidget(m_widget);

        QPalette pal(ShowWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        ShowWidget[iLoop]->setAutoFillBackground(true);
        ShowWidget[iLoop]->setPalette(pal);

        ShowWidget[iLoop]->setGeometry(QRect(134 , 190 + 90 * iLoop , 530 ,60));

        lbName[iLoop] = new QLabel(ShowWidget[iLoop]);
        lbName[iLoop]->setGeometry(QRect(25, 20 , 180 , 20));
        lbName[iLoop]->setAlignment(Qt::AlignLeft);

        m_pBtns[iLoop] = new CBitmapButton(ShowWidget[iLoop],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,iLoop);
        m_pBtns[iLoop]->setGeometry(QRect(350 ,20 ,200,20));

        connect(m_pBtns[iLoop], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

        if(DATE_SET == iLoop)
        {
            CalS = new QCalendarWidget(m_widget);
            //gGlobalParam.MiscParam.iLan
            switch(gGlobalParam.MiscParam.iLan)
            {
            case APP_LAN_CHN:
                CalS->setLocale(QLocale::Chinese);
                break;
            case APP_LAN_SPA:
                CalS->setLocale(QLocale::Spanish);
                break;
            case APP_LAN_FRE:
                CalS->setLocale(QLocale::French);
                break;
            case APP_LAN_ITA:
                CalS->setLocale(QLocale::Italian);
                break;
            case APP_LAN_SKR:
                CalS->setLocale(QLocale::Korean);
                break;
            case APP_LAN_GER:
                CalS->setLocale(QLocale::German);
                break;
            case APP_LAN_RUS:
                CalS->setLocale(QLocale::Russian);
                break;
            case APP_LAN_POR:
                CalS->setLocale(QLocale::Portuguese);
                break;
            default:
                CalS->setLocale(QLocale::English);
                break;
            }
            CalS->setGeometry(QRect(130,100,500,300));
            CalS->hide();

        }

        else if(TIME_SET == iLoop)
        {
            TimeHCbox = new QComboBox(m_widget);
            TimeHCbox->setGeometry(230,200,80,30);
            TimeHCbox->hide();

            for(tmp = 0 ; tmp < 24 ; tmp++ )
            {
                TimeHCbox->addItem(QString::number(tmp,10));
            }

            TimeMCbox = new QComboBox(m_widget);
            TimeMCbox->setGeometry(330,200,80,30);
            TimeMCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeMCbox->addItem(QString::number(tmp,10));
            }

            TimeSCbox = new QComboBox(m_widget);
            TimeSCbox->setGeometry(430,200,80,30);
            TimeSCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeSCbox->addItem(QString::number(tmp,10));
            }

        }

    }

    m_pExNextBtn = new QPushButton(m_widget);
    m_pExBackBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_ExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_ExBackBtn_Clicked()));

    m_pExBackBtn->move(200, 450);
    m_pExNextBtn->move(500, 450);
}



void Ex_Init_TimePage::initUi()
{
     setBackColor();

     createHeads();

     Create_Cal_Ok();

     Create_Date_Time();
}

void Ex_Init_TimePage::update()
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    m_pBtns[TIMEPAGE_BTN_DATE_SET]->setTip(sysDateTime.toString("yyyy-MM-dd"));
    m_pBtns[TIMEPAGE_BTN_TIME_SET]->setTip(sysDateTime.toString("hh:mm:ss"));
}

void Ex_Init_TimePage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_curX = e->x();
        m_curY = e->y();
        m_lstFlag = 1;
    }
}

void Ex_Init_TimePage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void Ex_Init_TimePage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        m_wndMain->naviInitPage(Ex_Init_Time, m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}


void Ex_Init_TimePage::Date_show_hide(bool show)
{
    if(show)
    {
        CalS->show();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->show();
        m_pBtns[TIMEPAGE_BTN_OK]->show();
        ShowWidget[DATE_SET]->hide();
        ShowWidget[TIME_SET]->hide();

        m_pExNextBtn->hide();
        m_pExBackBtn->hide();
    }
    else
    {
        CalS->hide();
        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();

        m_pExNextBtn->show();
        m_pExBackBtn->show();
    }
}

void Ex_Init_TimePage::Time_show_hide(bool show)
{
    if(show)
    {
        QDateTime sysDateTime;
        sysDateTime = QDateTime::currentDateTime();

        int hour = sysDateTime.time().hour();
        int min  = sysDateTime.time().minute();
        int sec  = sysDateTime.time().second();

        TimeHCbox->show();
        TimeHCbox->setCurrentIndex(hour);

        TimeMCbox->show();
        TimeMCbox->setCurrentIndex(min);

        TimeSCbox->show();
        TimeSCbox->setCurrentIndex(sec);

        m_pBtns[TIMEPAGE_BTN_OK]->show();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->show();

        lbTitPic->hide();
        lbTitName->hide();

        ShowWidget[DATE_SET]->hide();
        ShowWidget[TIME_SET]->hide();

        m_pExNextBtn->hide();
        m_pExBackBtn->hide();
    }
    else
    {
        TimeHCbox->hide();
        TimeMCbox->hide();
        TimeSCbox->hide();

        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

        lbTitPic->show();
        lbTitName->show();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();

        m_pExNextBtn->show();
        m_pExBackBtn->show();
    }

}

void Ex_Init_TimePage::on_Ok_clicked()
{
    if(CalS->isVisible())
    {
        QDate date  = CalS->selectedDate();
        QString tmp = date.toString("yyyy-MM-dd");
        m_pBtns[TIMEPAGE_BTN_DATE_SET]->setTip(tmp);

        Date_show_hide(false);
    }
    else if(TimeHCbox->isVisible())
    {
        QString TimeStr = TimeHCbox->currentText();
        TimeStr.append(":");
        TimeStr.append(TimeMCbox->currentText());
        TimeStr.append(":");
        TimeStr.append(TimeSCbox->currentText());
        m_pBtns[TIME_SET]->setTip(TimeStr);

        Time_show_hide(false);
    }

    changeTime();

}

void Ex_Init_TimePage::on_Cancel_clicked()
{
    if(CalS->isVisible())
    {
        Date_show_hide(false);
    }
    else if(TimeHCbox->isVisible())
    {
        Time_show_hide(false);
    }
}

void Ex_Init_TimePage::on_btn_clicked(int index)
{
    switch(index)
    {
    case TIMEPAGE_BTN_DATE_SET:
        Date_show_hide(true);
        break;
    case TIMEPAGE_BTN_TIME_SET:
        Time_show_hide(true);
        break;
    case TIMEPAGE_BTN_CANCEL:
        on_Cancel_clicked();
        break;
    case TIMEPAGE_BTN_OK:
        on_Ok_clicked();
        break;
    }

    m_wndMain->prepareKeyStroke();
}

void Ex_Init_TimePage::on_ExNextBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_Time, 0);
    m_wndMain->prepareKeyStroke();
}

void Ex_Init_TimePage::on_ExBackBtn_Clicked()
{
    m_wndMain->naviInitPage(Ex_Init_Time, 1);
    m_wndMain->prepareKeyStroke();
}

void Ex_Init_TimePage::changeTime()
{
    //QDateTime time;
    time_t    Time_t;
    struct tm curtime;
    struct rtc_time rtctime ;

    int fb;

    //time = QDateTime::fromString(m_pBtns[TIMEPAGE_BTN_DATE_SET]->getTip() +  " " + m_pBtns[TIMEPAGE_BTN_TIME_SET]->getTip(), "yyyy-MM-dd hh:mm:ss");

    QStringList timeStrList = m_pBtns[TIMEPAGE_BTN_TIME_SET]->getTip().split(QRegExp("[:]"));

    QStringList dateStrList = m_pBtns[TIMEPAGE_BTN_DATE_SET]->getTip().split(QRegExp("[-]"));

    int iYear,iMon,iDay,iHour,iMin,iSec;

    iSec  = QString(timeStrList[2]).toInt();
    iMin  = QString(timeStrList[1]).toInt();
    iHour = QString(timeStrList[0]).toInt();
    iDay  = QString(dateStrList[2]).toInt();
    iMon  = QString(dateStrList[1]).toInt();
    iYear = QString(dateStrList[0]).toInt();

    curtime.tm_sec  = iSec;
    curtime.tm_min  = iMin;
    curtime.tm_hour = iHour;
    curtime.tm_mday = iDay;
    curtime.tm_mon  = iMon  - 1;
    curtime.tm_year = iYear - 1900;
    curtime.tm_isdst= -1;

    Time_t = mktime(&curtime);

    stime(&Time_t);

    struct tm *tmp_ptr = NULL;

    tmp_ptr = gmtime(&Time_t);

    rtctime.tm_sec  = tmp_ptr->tm_sec;
    rtctime.tm_min  = tmp_ptr->tm_min;
    rtctime.tm_hour = tmp_ptr->tm_hour;
    rtctime.tm_mday = tmp_ptr->tm_mday;
    rtctime.tm_mon  = tmp_ptr->tm_mon;
    rtctime.tm_year = tmp_ptr->tm_year;
    rtctime.tm_isdst=-1;

    fb = open("/dev/rtc0", QIODevice::ReadWrite);
    if (fb != -1)
    {
        int ret = ioctl(fb , RTC_SET_TIME , &rtctime);

        if (ret)
        {
            qDebug() << " ioctl : " << ret;
        }

        ::close(fb);
    }
}


