#include "reportpage.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include <QRect>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QHeaderView>

#include "cbitmapbutton.h"

extern QString select_sql_Log ;
extern QString delete_sql_Log ;

ReportPage::ReportPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

}

void ReportPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void ReportPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Service Report");

    setTitles(stringList);

}

void ReportPage::buildTranslation()
{
    //int iLoop;
    
    m_pBtns[REPORTPAGE_BTN_CANCEL]->setTip(tr("Cancel") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[REPORTPAGE_BTN_OK]->setTip(tr("OK") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[REPORTPAGE_BTN_SEND]->setTip(tr("Send") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[REPORTPAGE_BTN_QRY]->setTip(tr("Search") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[REPORTPAGE_BTN_DEL]->setTip(tr("Delete Record") , BITMAPBUTTON_ICON_CENTER);
    m_plbStartTime->setText(tr("Start Date"));
    m_plbStopTime->setText(tr("End Date"));

}

void ReportPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void ReportPage::update()
{
    QDate date  = m_pCalendar->selectedDate();
    QString tmp = date.toString("yyyy-MM-dd");
    
    m_plbQryEndTimeInfo->setText(tmp);
    
    m_plbQryStartTimeInfo->setText(tmp);

}

void ReportPage::setBackColor()
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

void ReportPage::createTable()
{
    m_pTable = new QTableWidget(50,3,m_widget);

    m_pTable->setGeometry(QRect(0,120 ,800,400));

    m_pTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTable->setStyleSheet("selection-background-color:Gainsboro;");

    //m_pTable->setEditTriggers(QAbstractItemView::DoubleClicked);

    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //m_pTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

}

void ReportPage::initUi()
{
    int iLoop;
    
    setBackColor();

    createTable();

    //createList();

    m_pOperatingWidget = new QWidget(m_widget);

    QPalette pal(m_pOperatingWidget->palette());

    pal.setColor(QPalette::Background, Qt::white);

    m_pOperatingWidget->setAutoFillBackground(true);
    m_pOperatingWidget->setPalette(pal);

    m_pOperatingWidget->setGeometry(QRect(0 , 50 , 800 ,70));

    m_plbStartTime = new QLabel(m_pOperatingWidget);
    m_plbStartTime->setGeometry(QRect(20, 10 , 120 , 20));
    m_plbStartTime->setAlignment(Qt::AlignRight);

    m_plbStopTime = new QLabel(m_pOperatingWidget);
    m_plbStopTime->setGeometry(QRect(20, 35 , 120 , 20));
    m_plbStopTime->setAlignment(Qt::AlignRight);

    m_pBtns[REPORTPAGE_BTN_QRY_ST] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_QRY_ST);
    m_pBtns[REPORTPAGE_BTN_QRY_ST]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[REPORTPAGE_BTN_QRY_ST]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[REPORTPAGE_BTN_QRY_ST]->setGeometry(QRect(180 ,2 ,m_pBtns[REPORTPAGE_BTN_QRY_ST]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_QRY_ST]->getPicHeight()));

    m_plbQryStartTimeInfo = new QLabel(m_pBtns[REPORTPAGE_BTN_QRY_ST]);
    m_plbQryStartTimeInfo->setText("2017-01-01");
    m_plbQryStartTimeInfo->setGeometry(QRect(20 ,7 ,180,20));

    m_pBtns[REPORTPAGE_BTN_QRY_ET] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_QRY_ET);
    m_pBtns[REPORTPAGE_BTN_QRY_ET]->setGeometry(QRect(180 ,2*2+m_pBtns[REPORTPAGE_BTN_QRY_ST]->getPicHeight() ,m_pBtns[REPORTPAGE_BTN_QRY_ST]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_QRY_ST]->getPicHeight()));
    m_pBtns[REPORTPAGE_BTN_QRY_ET]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[REPORTPAGE_BTN_QRY_ET]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);

    m_plbQryEndTimeInfo = new QLabel(m_pBtns[REPORTPAGE_BTN_QRY_ET]);
    m_plbQryEndTimeInfo->setText("2017-01-01");
    m_plbQryEndTimeInfo->setGeometry(QRect(20 ,7 ,180,20));

    m_pBtns[REPORTPAGE_BTN_QRY] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_QRY);
    m_pBtns[REPORTPAGE_BTN_QRY]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[REPORTPAGE_BTN_QRY]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[REPORTPAGE_BTN_QRY]->setGeometry(QRect(380 ,2 ,m_pBtns[REPORTPAGE_BTN_QRY]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_QRY]->getPicHeight()));


    m_pBtns[REPORTPAGE_BTN_DEL] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_DEL);
    m_pBtns[REPORTPAGE_BTN_DEL]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[REPORTPAGE_BTN_DEL]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[REPORTPAGE_BTN_DEL]->setGeometry(QRect(380 ,2*2+m_pBtns[REPORTPAGE_BTN_QRY]->getPicHeight() ,m_pBtns[REPORTPAGE_BTN_DEL]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_DEL]->getPicHeight()));
    m_pBtns[REPORTPAGE_BTN_DEL]->hide();

    m_pBtns[REPORTPAGE_BTN_SEND] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,REPORTPAGE_BTN_SEND);
    m_pBtns[REPORTPAGE_BTN_SEND]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_NORMAL]);
    m_pBtns[REPORTPAGE_BTN_SEND]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_ACTIVE]);
    m_pBtns[REPORTPAGE_BTN_SEND]->setGeometry(560,540,m_pBtns[REPORTPAGE_BTN_SEND]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_SEND]->getPicHeight());
    m_pBtns[REPORTPAGE_BTN_SEND]->setStyleSheet("background-color:transparent");

    {
        m_pTimeFrame = new QFrame(m_widget);
        
        QPalette pal(m_pTimeFrame->palette());
        
        pal.setColor(QPalette::Background, Qt::white);
        
        m_pTimeFrame->setAutoFillBackground(true);
        m_pTimeFrame->setPalette(pal);
        
        m_pTimeFrame->setGeometry(QRect(0 , 50 , 780 ,400));

        m_pCalendar = new QCalendarWidget(m_pTimeFrame);
        m_pCalendar->setGeometry(QRect(100,10,600,300));

        m_pBtns[REPORTPAGE_BTN_CANCEL] = new CBitmapButton(m_pTimeFrame,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_CANCEL);
        m_pBtns[REPORTPAGE_BTN_CANCEL]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_NORMAL]);
        m_pBtns[REPORTPAGE_BTN_CANCEL]->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_CANEL_ACTIVE]);
        m_pBtns[REPORTPAGE_BTN_CANCEL]->setGeometry(QRect(230 , 350 ,m_pBtns[REPORTPAGE_BTN_CANCEL]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_CANCEL]->getPicHeight()));
        m_pBtns[REPORTPAGE_BTN_CANCEL]->setStyleSheet("background-color:transparent");
    
        m_pBtns[REPORTPAGE_BTN_OK]   = new CBitmapButton(m_pTimeFrame,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,REPORTPAGE_BTN_OK);
        m_pBtns[REPORTPAGE_BTN_OK]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_NORMAL]);
        m_pBtns[REPORTPAGE_BTN_OK]->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_OK_ACTIVE]);
        m_pBtns[REPORTPAGE_BTN_OK]->setGeometry(QRect(430 , 350 ,m_pBtns[REPORTPAGE_BTN_OK]->getPicWidth(),m_pBtns[REPORTPAGE_BTN_OK]->getPicHeight()));
        m_pBtns[REPORTPAGE_BTN_OK]->setStyleSheet("background-color:transparent");
        m_pTimeFrame->hide();
            
    }
    for (iLoop = 0; iLoop < REPORTPAGE_BTN_NUM; iLoop++)
    {
        connect(m_pBtns[iLoop], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
       
    }

    {
        CBaseWidget *tmpWidget;
    
        tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
        tmpWidget->setObjectName("send to");
        tmpWidget->setGeometry(230,120,320,350);
        m_pSendToPage = new SendToPage(this , tmpWidget , m_wndMain);
    }

}

void ReportPage::dbQuery()
{
    QSqlQuery query;
    QString   Date;
    bool bDbResult = false;

    /* set column header */
    QStringList ColumnHead;

    int iRow = 0;

    ColumnHead << tr("name");
    ColumnHead << tr("action");
    ColumnHead << tr("time");

    m_pTable->clear();
    m_pTable->setColumnCount(ColumnHead.count());
    m_pTable->setHorizontalHeaderLabels(ColumnHead);
    
    query.prepare(select_sql_Log);
    query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
    query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
    
    bDbResult = query.exec();
    qDebug()<<"select_sql_Log "<< bDbResult;
    while (query.next())
    {
        QTableWidgetItem *item0;
        item0 = new QTableWidgetItem;
        item0->setText(query.value(1).toString());
//        m_pTable->setItem(iRow,1,item0);
        m_pTable->setItem(iRow,0,item0);

        item0 = new QTableWidgetItem;
        item0->setText(query.value(2).toString());
//        m_pTable->setItem(iRow,2,item0);
        m_pTable->setItem(iRow,1,item0);

        item0 = new QTableWidgetItem;
        item0->setText(query.value(4).toString());
//        m_pTable->setItem(iRow,3,item0);
        m_pTable->setItem(iRow,2,item0);

        iRow++;
    }
            
     
}

void ReportPage::dbDel()
{
    QSqlQuery query;
    bool bDbResult = false;

    query.prepare(delete_sql_Log);
    query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
    query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
    bDbResult = query.exec();
    if (!bDbResult)
    {
        QMessageBox::about(NULL, tr("Delete Records"), tr("Fail to Delete Records !"));
    
    }
     
}

void ReportPage::on_btn_clicked(int index)
{
    switch(index)
    {
    case REPORTPAGE_BTN_QRY_ST:
    case REPORTPAGE_BTN_QRY_ET:
        {
            m_pTimeFrame->show();
            m_iIndex4TimeInfo = index - REPORTPAGE_BTN_QRY_ST;
        }        
        break;
    case REPORTPAGE_BTN_QRY:
        {
            dbQuery();
        }
        break;
#if 0        
    case REPORTPAGE_BTN_DEL:
        {
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("Delete Record"), tr("Are you sure to delete Record?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No); 
            if(rb == QMessageBox::Yes) 
            {
                dbDel();
            }
        }
        break;
#endif        
    case REPORTPAGE_BTN_SEND:
        on_send_clicked();
        break;
    case REPORTPAGE_BTN_CANCEL:
    case REPORTPAGE_BTN_OK:
        cancel_ok_clicked(index);
        break;
    }
    
    m_wndMain->prepareKeyStroke();
}

void ReportPage::cancel_ok_clicked(int index)
{
    if(REPORTPAGE_BTN_OK == index)
    {
        QDate date  = m_pCalendar->selectedDate();
        QString tmp = date.toString("yyyy-MM-dd");

        if(m_iIndex4TimeInfo)
        {
            m_plbQryEndTimeInfo->setText(tmp);
        }
        else
        {
            m_plbQryStartTimeInfo->setText(tmp);
        }

    }

    m_pTimeFrame->hide();
}


void ReportPage::on_send_clicked()
{

    m_widget->setWindowOpacity(0.0);

    m_pSendToPage->show(true);

}


