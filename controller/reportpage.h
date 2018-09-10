#ifndef REPORTPAGE_H
#define REPORTPAGE_H

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

#include "sendtopage.h"

#include "msgbox.h"

class MainWindow;

enum REPORTPAGE_BTN_NENUM
{
    REPORTPAGE_BTN_QRY_ST = 0,
    REPORTPAGE_BTN_QRY_ET,
    REPORTPAGE_BTN_SEND,
    REPORTPAGE_BTN_CANCEL,
    REPORTPAGE_BTN_OK,
    REPORTPAGE_BTN_QRY,
    REPORTPAGE_BTN_DEL,
    REPORTPAGE_BTN_NUM,
};

class ReportPage : public CSubPage
{
    Q_OBJECT
public:
    ReportPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

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

    QTableWidget *m_pTable;

    QWidget *m_pOperatingWidget;

    QFrame *m_pTimeFrame;

    CBitmapButton *m_pBtns[REPORTPAGE_BTN_NUM];

    QLabel        *m_plbStartTime;
    QLabel        *m_plbStopTime;
    QLabel        *m_plbQryStartTimeInfo;
    QLabel        *m_plbQryEndTimeInfo;

    QCalendarWidget *m_pCalendar;

    SendToPage     *m_pSendToPage;

    int             m_iIndex4TimeInfo;

public slots:

    void on_btn_clicked(int);
};


#endif // REPORTPAGE_H
