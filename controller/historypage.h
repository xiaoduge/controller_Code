#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>

#include <QComboBox>
#include <QCalendarWidget>

#include <QTableWidget>

#include "historylistwidgtitem.h"

#include "sendtopage.h"

#include "msgbox.h"

#define HistoryMax  50

class MainWindow;

enum HISTORYPAGE_BTN_NENUM
{
    HISTORYPAGE_BTN_QRY_ST = 0,
    HISTORYPAGE_BTN_QRY_ET,
    HISTORYPAGE_BTN_SEND,
    HISTORYPAGE_BTN_CANCEL,
    HISTORYPAGE_BTN_OK,
    HISTORYPAGE_BTN_QRY,
    HISTORYPAGE_BTN_DEL,
    HISTORYPAGE_BTN_PREV,
    HISTORYPAGE_BTN_NEXT,
    HISTORYPAGE_BTN_NUM,
};

class HistoryPage : public CSubPage
{
    Q_OBJECT
public:
    HistoryPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();


private:
    
    void createTable();

    void setBackColor();

    void buildTitles();

    void dbQuery();

    void dbDel();

    void on_send_clicked();
    void cancel_ok_clicked(int index);
    
    int m_iCurScroller;
    int m_iPageRecordCount;

    QTableWidget *m_pTable;

    QWidget      *m_pOperatingWidget;

    QFrame       *m_pTimeFrame;

    QComboBox    *m_QryObjNameCbox;


    CBitmapButton *m_pBtns[HISTORYPAGE_BTN_NUM];

    QLabel        *m_plbStartTime;
    QLabel        *m_plbStopTime;
    QLabel        *m_plbQryStartTimeInfo;
    QLabel        *m_plbQryEndTimeInfo;

    QCalendarWidget *m_pCalendar;

    SendToPage      *m_pSendToPage;


    int              m_iIndex4TimeInfo;

public slots:

    void on_btn_clicked(int);
    void on_sel_com(const QString &strName);
};

#endif // HISTORYPAGE_H
