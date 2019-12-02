#ifndef DHISTORYPAGE_H
#define DHISTORYPAGE_H

#include "subpage.h"

class QCalendarWidget;
class SendToPage;
class MainWindow;
class QPushButton;
class QComboBox;
class QLabel;
class QDockWidget;
class QTableView;
class QSqlTableModel;
class QTableWidget;

class DHistoryPage : public CSubPage
{
    Q_OBJECT

public:
    enum HISTORYPAGE_BTN_NENUM
    {
        HISTORYPAGE_BTN_QRY_STIME = 0,
        HISTORYPAGE_BTN_QRY_ETIME,
        HISTORYPAGE_BTN_QRY,
        HISTORYPAGE_BTN_PREV,
        HISTORYPAGE_BTN_NEXT,
        HISTORYPAGE_BTN_SEND,
        HISTORYPAGE_BTN_NUM
    };

    enum HISTORYPAGE_TIME_BTN_NENUM
    {
        HISTORYPAGE_TIME_BTN_CANCEL,
        HISTORYPAGE_TIME_BTN_OK,
        HISTORYPAGE_TIME_BTN_NUM
    };
    enum HISTORYPAGE_DATA_TABLE
    {
        HISTORYPAGE_DATA_WATER = 0,
        HISTORYPAGE_DATA_ALARM,
        HISTORYPAGE_DATA_GETW,
        HISTORYPAGE_DATA_PWATER,
        HISTORYPAGE_DATA_LOG,
        HISTORYPAGE_DATA_TABLE_NUM
    };

public:
    DHistoryPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void update();
    virtual void leaveSubPage();

private slots:
    void on_HISTORYPAGE_BTN_QRY_STIME_clicked();
    void on_HISTORYPAGE_BTN_QRY_ETIME_clicked();
    void on_HISTORYPAGE_BTN_QRY_clicked();

    void on_HISTORYPAGE_BTN_PREV_clicked();
    void on_HISTORYPAGE_BTN_NEXT_clicked();
    void on_HISTORYPAGE_BTN_SEND_clicked();

    void on_m_QryObjNameCbox_currentIndexChanged(int index);

    void on_HISTORYPAGE_TIME_BTN_CANCEL_clicked();
    void on_HISTORYPAGE_TIME_BTN_OK_clicked();


private:
    void setBackColor();
    void buildTitles();

    void initConnect();
    void createDataModelAndView();
    void dbQuery();

    void dbQueryAll();
    void dbQueryWater();
    void dbQueryAlarm();
    void dbQueryGetWater();
    void dbQueryProduceWater();
    void dbQueryLog();

    void initTabelHeaderData(QSqlTableModel* tableModel, int tableID);
    void initTabelHeaderData();

private:
    QWidget      *m_pOperatingWidget;
    QComboBox    *m_QryObjNameCbox;

    QLabel        *m_plbStartTime;
    QLabel        *m_plbStopTime;

    QPushButton *m_phBtns[HISTORYPAGE_BTN_NUM];
    QPushButton* m_ptBtns[HISTORYPAGE_TIME_BTN_NUM];

    QCalendarWidget *m_pCalendar;
    QDockWidget* m_dockWidget;
    int m_btnID;

    SendToPage      *m_pSendToPage;

    //
    QTableView* m_pTableView;        //[HISTORYPAGE_DATA_TABLE_NUM];
    QSqlTableModel* m_pTableModel;   //[HISTORYPAGE_DATA_TABLE_NUM];

};


#endif // EX_HISTORYPAGE_H
