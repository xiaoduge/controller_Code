#include "historypage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QHeaderView>

#include "cbitmapbutton.h"
#include "ToastDlg.h"

extern QString select_sql_Water ;
extern QString delete_sql_Water ;

extern QString select_sql_Alarm ;
extern QString delete_sql_Alarm ;

extern QString select_sql_GetW  ;
extern QString delete_sql_GetW  ;

extern QString select_sql_PW ;
extern QString delete_sql_PW ;

extern QString select_sql_Log ;
extern QString delete_sql_Log ;

HistoryPage::HistoryPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iCurScroller    = 0;
    m_iPageRecordCount = 12;

    creatTitle();

    initUi();

    buildTranslation();

    dbDel();

}

void HistoryPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void HistoryPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("History");

    setTitles(stringList);

}

void HistoryPage::buildTranslation()
{
    int iLoop;
    
    for (iLoop = 0 ; iLoop < DB_TBL_NUM; iLoop++)
    {
        m_QryObjNameCbox->setItemText(iLoop,m_wndMain->getDbName(iLoop));
    }

    m_pBtns[HISTORYPAGE_BTN_CANCEL]->setTip(tr("Cancel") , BITMAPBUTTON_ICON_COCENTER);
    m_pBtns[HISTORYPAGE_BTN_OK]->setTip(tr("OK") , BITMAPBUTTON_ICON_COCENTER);
    m_pBtns[HISTORYPAGE_BTN_SEND]->setTip(tr("Send") , BITMAPBUTTON_ICON_BCENTER);
    m_pBtns[HISTORYPAGE_BTN_QRY]->setTip(tr("Search") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[HISTORYPAGE_BTN_DEL]->setTip(tr("Delete Record") , BITMAPBUTTON_ICON_CENTER);
    m_pBtns[HISTORYPAGE_BTN_PREV]->setTip(tr("Page Up") , BITMAPBUTTON_ICON_BCENTER);
    m_pBtns[HISTORYPAGE_BTN_NEXT]->setTip(tr("Page Down") , BITMAPBUTTON_ICON_BCENTER);
    m_plbStartTime->setText(tr("Start Date"));
    m_plbStopTime->setText(tr("End Date"));

}

void HistoryPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void HistoryPage::setBackColor()
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

void HistoryPage::createTable()
{
    m_pTable = new QTableWidget(50,11,m_widget);

    m_pTable->setGeometry(QRect(0,120 ,800,400));

    //QStringList RowHead;
    //m_pTable->setHorizontalHeaderLabels(RowHead);


    //m_pTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //QStringList ColumnHead;
    //m_pTable->setVerticalHeaderLabels(ColumnHead);


    //m_pTable->setRowHeight(0, 22);

    //m_pTable->horizontalHeader()->setStretchLastSection(true);
    m_pTable->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_pTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_pTable->setStyleSheet(m_wndMain->getQss4Table());
    m_pTable->setStyleSheet("selection-background-color:Gainsboro;");

    //m_pTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_pTable->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

#if 0

    for (int row = 0; row < 4; ++row)
    {
        QTableWidgetItem *item0, *item1;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;

        QString txt = QString("%1").arg(row + 1);
        item0->setText(txt);
        m_pTable->setItem(row, 0, item0);

        txt = QString("%1").arg((row + 1) * 2);
        item1->setText(txt);
        m_pTable->setItem(row, 1, item1);

    }

    m_pTable->setRowCount(5);
    QTableWidgetItem *vHeader5 = new QTableWidgetItem("Line5");
    m_pTable->setVerticalHeaderItem(4, vHeader5);

    QTableWidgetItem *item5_0, *item5_1;
    item5_0 = new QTableWidgetItem;
    item5_1 = new QTableWidgetItem;
    item5_0->setText(QString("%1").arg(5));
    item5_1->setText(QString("%1").arg(5 * 2));
    m_pTable->setItem(4, 0, item5_0);
    m_pTable->setItem(4, 1, item5_1);
#endif

    //m_pTable->removeRow(1);
    //m_pTable->removeColumn(10);

    //m_pTable->setColumnHidden(10 , true);
}

void HistoryPage::initUi()
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

    m_QryObjNameCbox = new QComboBox(m_pOperatingWidget);
    m_QryObjNameCbox->setGeometry(20,20,150,30);

    m_QryObjNameCbox->setStyleSheet(m_wndMain->getQss4Cmb());


    for (iLoop = 0 ; iLoop < DB_TBL_NUM; iLoop++)
    {
        m_QryObjNameCbox->addItem(m_wndMain->getDbName(iLoop));
    }

    connect(m_QryObjNameCbox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_sel_com(const QString &)));

    m_QryObjNameCbox->hide();

    m_plbStartTime = new QLabel(m_pOperatingWidget);
    m_plbStartTime->setGeometry(QRect(180, 10 , 120 , 20));
    m_plbStartTime->setAlignment(Qt::AlignRight);

    m_plbStopTime = new QLabel(m_pOperatingWidget);
    m_plbStopTime->setGeometry(QRect(180, 35 , 120 , 20));
    m_plbStopTime->setAlignment(Qt::AlignRight);

    m_pBtns[HISTORYPAGE_BTN_QRY_ST] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_QRY_ST);
    m_pBtns[HISTORYPAGE_BTN_QRY_ST]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_QRY_ST]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_QRY_ST]->setGeometry(QRect(300 ,2 ,m_pBtns[HISTORYPAGE_BTN_QRY_ST]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_QRY_ST]->getPicHeight()));

    m_plbQryStartTimeInfo = new QLabel(m_pBtns[HISTORYPAGE_BTN_QRY_ST]);
    m_plbQryStartTimeInfo->setText("2017-01-01");
    m_plbQryStartTimeInfo->setGeometry(QRect(20 ,7 ,180,20));

    m_pBtns[HISTORYPAGE_BTN_QRY_ET] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_QRY_ET);
    m_pBtns[HISTORYPAGE_BTN_QRY_ET]->setGeometry(QRect(300 ,2*2+m_pBtns[HISTORYPAGE_BTN_QRY_ST]->getPicHeight() ,m_pBtns[HISTORYPAGE_BTN_QRY_ST]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_QRY_ST]->getPicHeight()));
    m_pBtns[HISTORYPAGE_BTN_QRY_ET]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_QRY_ET]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);

    m_plbQryEndTimeInfo = new QLabel(m_pBtns[HISTORYPAGE_BTN_QRY_ET]);
    m_plbQryEndTimeInfo->setText("2017-01-01");
    m_plbQryEndTimeInfo->setGeometry(QRect(20 ,7 ,180,20));

    m_pBtns[HISTORYPAGE_BTN_QRY] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_QRY);
    m_pBtns[HISTORYPAGE_BTN_QRY]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_QRY]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_QRY]->setGeometry(QRect(500 ,2 ,m_pBtns[HISTORYPAGE_BTN_QRY]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_QRY]->getPicHeight()));


    m_pBtns[HISTORYPAGE_BTN_DEL] = new CBitmapButton(m_pOperatingWidget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_DEL);
    m_pBtns[HISTORYPAGE_BTN_DEL]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_INACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_DEL]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_30_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_DEL]->setGeometry(QRect(500 ,2*2+m_pBtns[HISTORYPAGE_BTN_QRY]->getPicHeight() ,m_pBtns[HISTORYPAGE_BTN_DEL]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_DEL]->getPicHeight()));
    m_pBtns[HISTORYPAGE_BTN_DEL]->hide();

    m_pBtns[HISTORYPAGE_BTN_SEND] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,HISTORYPAGE_BTN_SEND);
    m_pBtns[HISTORYPAGE_BTN_SEND]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_NORMAL]);
    m_pBtns[HISTORYPAGE_BTN_SEND]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_SEND]->setGeometry(QRect(560,540,m_pBtns[HISTORYPAGE_BTN_SEND]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_SEND]->getPicHeight()));
    m_pBtns[HISTORYPAGE_BTN_SEND]->setStyleSheet("background-color:transparent");

    m_pBtns[HISTORYPAGE_BTN_PREV] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,HISTORYPAGE_BTN_PREV);
    m_pBtns[HISTORYPAGE_BTN_PREV]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_NORMAL]);
    m_pBtns[HISTORYPAGE_BTN_PREV]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_PREV]->setGeometry(QRect(60,540,m_pBtns[HISTORYPAGE_BTN_PREV]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_PREV]->getPicHeight()));
    m_pBtns[HISTORYPAGE_BTN_PREV]->setStyleSheet("background-color:transparent");

    m_pBtns[HISTORYPAGE_BTN_NEXT] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,HISTORYPAGE_BTN_NEXT);
    m_pBtns[HISTORYPAGE_BTN_NEXT]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_NORMAL]);
    m_pBtns[HISTORYPAGE_BTN_NEXT]->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_SEND_ACTIVE]);
    m_pBtns[HISTORYPAGE_BTN_NEXT]->setGeometry(QRect(310,540,m_pBtns[HISTORYPAGE_BTN_NEXT]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_NEXT]->getPicHeight()));
    m_pBtns[HISTORYPAGE_BTN_NEXT]->setStyleSheet("background-color:transparent");
    {
        m_pTimeFrame = new QFrame(m_widget);
        
        QPalette pal(m_pTimeFrame->palette());
        
        pal.setColor(QPalette::Background, Qt::white);
        
        m_pTimeFrame->setAutoFillBackground(true);
        m_pTimeFrame->setPalette(pal);
        
        m_pTimeFrame->setGeometry(QRect(0 , 50 , 800 ,400));

        m_pCalendar = new QCalendarWidget(m_pTimeFrame);
        m_pCalendar->setGeometry(QRect(100,10,600,300));

        m_pBtns[HISTORYPAGE_BTN_CANCEL] = new CBitmapButton(m_pTimeFrame,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_CANCEL);
        m_pBtns[HISTORYPAGE_BTN_CANCEL]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_NORMAL]);
        m_pBtns[HISTORYPAGE_BTN_CANCEL]->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_CANEL_ACTIVE]);
        m_pBtns[HISTORYPAGE_BTN_CANCEL]->setGeometry(QRect(230 , 350 ,m_pBtns[HISTORYPAGE_BTN_CANCEL]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_CANCEL]->getPicHeight()));
        m_pBtns[HISTORYPAGE_BTN_CANCEL]->setStyleSheet("background-color:transparent");
    
        m_pBtns[HISTORYPAGE_BTN_OK]   = new CBitmapButton(m_pTimeFrame,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,HISTORYPAGE_BTN_OK);
        m_pBtns[HISTORYPAGE_BTN_OK]->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_NORMAL]);
        m_pBtns[HISTORYPAGE_BTN_OK]->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_OK_ACTIVE]);
        m_pBtns[HISTORYPAGE_BTN_OK]->setGeometry(QRect(430 , 350 ,m_pBtns[HISTORYPAGE_BTN_OK]->getPicWidth(),m_pBtns[HISTORYPAGE_BTN_OK]->getPicHeight()));
        m_pBtns[HISTORYPAGE_BTN_OK]->setStyleSheet("background-color:transparent");
        m_pTimeFrame->hide();
            
    }
    for (iLoop = 0; iLoop < HISTORYPAGE_BTN_NUM; iLoop++)
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
    //msg_box = new MsgBox();
    //msg_box->setInfo("test");

}

void HistoryPage::dbQuery()
{
    QSqlQuery query;
    QString   Date;
    bool bDbResult = false;

    int iSel ;

    int iRow = 0;


    QTableWidgetItem *item0;

    //iSel = m_QryObjNameCbox->currentIndex();

    m_pTable->clear();
    m_pTable->setColumnCount(10);

    m_pTable->setRowCount(2000);

    for (iSel = 0; iSel < DB_TBL_NUM; iSel++)
    {

        switch(iSel)
        {
        case DB_TBL_BEST_WATER_QUALITY:
            {
                /* set column header */
                QStringList ColumnHead;
                
                ColumnHead << tr("Water");
                ColumnHead << tr("ecoid");
                ColumnHead << tr("quality");
                ColumnHead << tr("time");

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[0]);
                m_pTable->setItem(iRow,0,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[1]);
                m_pTable->setItem(iRow,1,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[2]);
                m_pTable->setItem(iRow,2,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[3]);
                m_pTable->setItem(iRow,3,item0);
                iRow++;

                //m_pTable->clear();
                //m_pTable->setColumnCount(ColumnHead.count());
                //m_pTable->setHorizontalHeaderLabels(ColumnHead);
                
                query.prepare(select_sql_Water);
                query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
                query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
                
                bDbResult = query.exec();
                qDebug()<<"select_sql_Water "<< bDbResult<< iRow;

                
                while (query.next())
                {
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(1).toString());
                    m_pTable->setItem(iRow,1,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(2).toString());
                    m_pTable->setItem(iRow,2,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(3).toString());
                    m_pTable->setItem(iRow,3,item0);

                    qDebug()<< query.value(1).toString() << query.value(2).toString() << query.value(3).toString();

                    iRow++;
                }
                iRow++;
            }        
            break;
        case DB_TBL_ALARM:
            {
                /* set column header */
                QStringList ColumnHead;
                
                ColumnHead << tr("Alarm");
                ColumnHead << tr("type");
                ColumnHead << tr("status");
                ColumnHead << tr("time");

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[0]);
                m_pTable->setItem(iRow,0,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[1]);
                m_pTable->setItem(iRow,1,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[2]);
                m_pTable->setItem(iRow,2,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[3]);
                m_pTable->setItem(iRow,3,item0);
                iRow++;

                //m_pTable->clear();
                //m_pTable->setColumnCount(ColumnHead.count());
                //m_pTable->setHorizontalHeaderLabels(ColumnHead);
                
                query.prepare(select_sql_Alarm);
                query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
                query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
                
                bDbResult = query.exec();
                
                qDebug()<<"select_sql_Alarm "<< bDbResult<< iRow;
                while (query.next())
                {
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(1).toString());
                    m_pTable->setItem(iRow,1,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(2).toString());
                    m_pTable->setItem(iRow,2,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(3).toString());
                    m_pTable->setItem(iRow,3,item0);

                    iRow++;
                }
                iRow++;
            }        
            break;
        case DB_TBL_TAKE_WATER:
            {
                /* set column header */
                QStringList ColumnHead;

                ColumnHead << tr("GetTw");
                ColumnHead << tr("name");
                ColumnHead << tr("quantity");
                ColumnHead << tr("quality");
                ColumnHead << tr("tmperature");
                ColumnHead << tr("time");
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[0]);
                m_pTable->setItem(iRow,0,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[1]);
                m_pTable->setItem(iRow,1,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[2]);
                m_pTable->setItem(iRow,2,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[3]);
                m_pTable->setItem(iRow,3,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[4]);
                m_pTable->setItem(iRow,4,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[5]);
                m_pTable->setItem(iRow,5,item0);
                
                iRow++;

                //m_pTable->clear();
                //m_pTable->setColumnCount(ColumnHead.count());
                //m_pTable->setHorizontalHeaderLabels(ColumnHead);
                
                query.prepare(select_sql_GetW);
                query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
                query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
                
                bDbResult = query.exec();
                qDebug()<<"select_sql_GetTW "<< bDbResult<< iRow;
                while (query.next())
                {
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(1).toString());
                    m_pTable->setItem(iRow,1,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(2).toString());
                    m_pTable->setItem(iRow,2,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(3).toString());
                    m_pTable->setItem(iRow,3,item0);
                    
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(4).toString());
                    m_pTable->setItem(iRow,4,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(5).toString());
                    m_pTable->setItem(iRow,5,item0);

                    iRow++;
                }
                iRow++;
            }        
            break;
        case DB_TBL_PRODUCE_WATER:
            {
                /* set column header */
                QStringList ColumnHead;

                /* id, time, ecoroin, tmproin, ecorores, ecoropw, tmpropw, ecoedi, tmpedi, duration */
                ColumnHead << tr("GetPw");
                ColumnHead << tr("time");
                ColumnHead << tr("roinQua");
                ColumnHead << tr("roinTmp");
                ColumnHead << tr("rores");
                ColumnHead << tr("ropw");
                ColumnHead << tr("ropwTmp");
                ColumnHead << tr("ediQua");
                ColumnHead << tr("ediTmp");
                ColumnHead << tr("duration");

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[0]);
                m_pTable->setItem(iRow,0,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[1]);
                m_pTable->setItem(iRow,1,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[2]);
                m_pTable->setItem(iRow,2,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[3]);
                m_pTable->setItem(iRow,3,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[4]);
                m_pTable->setItem(iRow,4,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[5]);
                m_pTable->setItem(iRow,5,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[6]);
                m_pTable->setItem(iRow,6,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[7]);
                m_pTable->setItem(iRow,7,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[8]);
                m_pTable->setItem(iRow,8,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[9]);
                m_pTable->setItem(iRow,9,item0);
                
                iRow++;

                //m_pTable->clear();
                //m_pTable->setColumnCount(ColumnHead.count());
                //m_pTable->setHorizontalHeaderLabels(ColumnHead);
                
                query.prepare(select_sql_PW);
                query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
                query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
                
                bDbResult = query.exec();
                qDebug()<<"select_sql_GetPW "<< bDbResult<< iRow;
                while (query.next())
                {
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(1).toString());
                    m_pTable->setItem(iRow,1,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(2).toString());
                    m_pTable->setItem(iRow,2,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(3).toString());
                    m_pTable->setItem(iRow,3,item0);
                    
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(4).toString());
                    m_pTable->setItem(iRow,4,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(5).toString());
                    m_pTable->setItem(iRow,5,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(6).toString());
                    m_pTable->setItem(iRow,6,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(7).toString());
                    m_pTable->setItem(iRow,7,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(8).toString());
                    m_pTable->setItem(iRow,8,item0);


                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(9).toString());
                    m_pTable->setItem(iRow,9,item0);

                    iRow++;
                }
                iRow++;
            }        
            break;
        case DB_TBL_LOG:
            {
                /* set column header */
                QStringList ColumnHead;
                
                ColumnHead << tr("LOG");
                ColumnHead << tr("name");
                ColumnHead << tr("action");
                ColumnHead << tr("time");

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[0]);
                m_pTable->setItem(iRow,0,item0);

                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[1]);
                m_pTable->setItem(iRow,1,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[2]);
                m_pTable->setItem(iRow,2,item0);
                
                item0 = new QTableWidgetItem;
                item0->setText(ColumnHead[3]);
                m_pTable->setItem(iRow,3,item0);
                iRow++;

                //m_pTable->clear();
                //m_pTable->setColumnCount(ColumnHead.count());
                //m_pTable->setHorizontalHeaderLabels(ColumnHead);
                
                query.prepare(select_sql_Log);
                query.addBindValue(m_plbQryStartTimeInfo->text() + " 00:00:00");
                query.addBindValue(m_plbQryEndTimeInfo->text()   + " 23:59:59");
                
                bDbResult = query.exec();
                qDebug()<<"select_sql_Log "<< bDbResult<< iRow;
                while (query.next())
                {
                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(1).toString());
                    m_pTable->setItem(iRow,1,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(2).toString());
                    m_pTable->setItem(iRow,2,item0);

                    item0 = new QTableWidgetItem;
                    item0->setText(query.value(4).toString());
                    m_pTable->setItem(iRow,3,item0);

                    iRow++;
                }
                iRow++;
            }        
            break;
        default:
            break;
        }
    }
}

void HistoryPage::dbDel()
{
    QSqlQuery query;
    bool bDbResult = false;
    
    int iSel = m_QryObjNameCbox->currentIndex();
    
    QDateTime sysDateTime;
    QDateTime dbdDateTime;
    sysDateTime = QDateTime::currentDateTime();
    dbdDateTime = sysDateTime.addYears(-2);

    QString tmp = dbdDateTime.toString("yyyy-MM-dd");

    switch(iSel)
    {
    case DB_TBL_BEST_WATER_QUALITY:
        {
            query.prepare(delete_sql_Water);
            query.addBindValue(tmp + " 00:00:00");
            query.addBindValue(tmp + " 23:59:59");
            
            bDbResult = query.exec();

        }        
        break;
    case DB_TBL_ALARM:
        {
            query.prepare(delete_sql_Alarm);
            query.addBindValue(tmp + " 00:00:00");
            query.addBindValue(tmp + " 23:59:59");
            
            bDbResult = query.exec();

        }        
        break;
    case DB_TBL_TAKE_WATER:
        {
            query.prepare(delete_sql_GetW);
            query.addBindValue(tmp + " 00:00:00");
            query.addBindValue(tmp + " 23:59:59");
            
            bDbResult = query.exec();

        }        
        break;
    case DB_TBL_PRODUCE_WATER:
        {
            query.prepare(delete_sql_PW);
            query.addBindValue(tmp + " 00:00:00");
            query.addBindValue(tmp + " 23:59:59");
            bDbResult = query.exec();
        }        
        break;
    case DB_TBL_LOG:
        {
            query.prepare(delete_sql_Log);
            query.addBindValue(tmp + " 00:00:00");
            query.addBindValue(tmp + " 23:59:59");
            bDbResult = query.exec();
        }        
        break;
    default:
        return;
    }
     
    if (!bDbResult)
    {
        //QMessageBox::about(NULL, tr("Delete Records"), tr("Fail to Delete Records !"));
        ToastDlg::makeToast(tr("Fail to Delete Records !"));
    
    }
}



void HistoryPage::on_btn_clicked(int index)
{
    switch(index)
    {
    case HISTORYPAGE_BTN_QRY_ST:
    case HISTORYPAGE_BTN_QRY_ET:
        {
            m_pTimeFrame->show();
            m_iIndex4TimeInfo = index - HISTORYPAGE_BTN_QRY_ST;
        }        
        break;
    case HISTORYPAGE_BTN_QRY:
        {
            dbQuery();
        }
        break;
#if 0        
    case HISTORYPAGE_BTN_DEL:
        {
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("Delete Record"), tr("Are you sure to delete Record?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No); 
            if(rb == QMessageBox::Yes) 
            {
                dbDel();
            }
        }
        break;
#endif        
    case HISTORYPAGE_BTN_SEND:
        on_send_clicked();
        break;
    case HISTORYPAGE_BTN_CANCEL:
    case HISTORYPAGE_BTN_OK:
        cancel_ok_clicked(index);
        break;
    case HISTORYPAGE_BTN_NEXT:
        {
            /*
            int limitIndex = (currentPage - 2) * m_iPageRecordCount;  
            RecordQuery(limitIndex);  
            currentPage -= 1;  
            UpdateStatus();
            */
            int maxValue = m_pTable->verticalScrollBar()->maximum(); 
            m_iCurScroller = m_pTable->verticalScrollBar()->value(); 

            if(m_iCurScroller<maxValue)
                m_pTable->verticalScrollBar()->setSliderPosition(m_iPageRecordCount+m_iCurScroller);
            else
                m_pTable->verticalScrollBar()->setSliderPosition(0);
        }
        break;
    case HISTORYPAGE_BTN_PREV:
        {
             /*
            int limitIndex = currentPage * m_iPageRecordCount;  
            RecordQuery(limitIndex);  
            currentPage += 1;  
            UpdateStatus(); 
            */
            int maxValue = m_pTable->verticalScrollBar()->maximum(); 
            m_iCurScroller = m_pTable->verticalScrollBar()->value();

            if(m_iCurScroller>0)
                m_pTable->verticalScrollBar()->setSliderPosition(m_iCurScroller-m_iPageRecordCount);
            else
                m_pTable->verticalScrollBar()->setSliderPosition(maxValue);
        }
        break;
    }
    
    m_wndMain->prepareKeyStroke();
}

void HistoryPage::update()
{
    QDate date  = m_pCalendar->selectedDate();
    QString tmp = date.toString("yyyy-MM-dd");
    
    m_plbQryEndTimeInfo->setText(tmp);
    
    m_plbQryStartTimeInfo->setText(tmp);

}

void HistoryPage::cancel_ok_clicked(int index)
{

    if(HISTORYPAGE_BTN_OK == index)
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

void HistoryPage::on_sel_com(const QString &strName)
{
    qDebug()<<strName;

    QStringList RowHead;

    RowHead<<strName;
    m_pTable->setHorizontalHeaderLabels(RowHead);
}

void HistoryPage::on_send_clicked()
{

    m_widget->setWindowOpacity(0.0);

    m_pSendToPage->show(true);

    //msg_box->exec();
}

