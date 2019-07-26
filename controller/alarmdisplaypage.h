#ifndef ALARMDISPLAYPAGE_H
#define ALARMDISPLAYPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QListWidgetItem>

#include "alarmdisplaylistitem.h"
#include "Display.h"


#define ADS_MAX_CsListNum (DISP_ROC12 + 1)

typedef enum
{   
    ALARM_TYPE_CS,
    ALARM_TYPE_AS,
    ALARM_TYPE_NUM,
}ALARM_TYPE_ENUM;

class MainWindow;

class AlarmDisplayPage : public CSubPage
{
    Q_OBJECT
public:
    AlarmDisplayPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    virtual void update();

private:

    void createList();

    void setBackColor();

    void buildTitles();

    void csUpdate();

    void asUpdate();

    QLabel          *m_lbNotName;
    QLabel          *m_lbAlarmName;

    AdcsListItem    *m_pCslistItem[ADS_MAX_CsListNum];
    QListWidgetItem *m_pCslistWidgetItem[ADS_MAX_CsListNum];

    QListWidgetItem *m_pAslistWidgetIem[DISP_ALARM_TOTAL_NUM];
    AdasListItem    *m_pAslistItem[DISP_ALARM_TOTAL_NUM];

    struct AD_ITEM_TYPE {
        int iId;
    }aCsIds[ADS_MAX_CsListNum],aAsIds[DISP_ALARM_TOTAL_NUM];
    
    int aCsIndex[ADS_MAX_CsListNum],aAsIndex[DISP_ALARM_PART_NUM][DISP_ALARM_TOTAL_NUM];

    QListWidget     *m_listWidget[2];

    QString         m_astrPartAlarm[DISP_ALARM_PART0_NUM + DISP_ALARM_PART1_NUM];

    int             m_aiRealNum[2];

public slots:
    
    void on_btn_clicked(int index);
};

#endif // CONSUMABLESTATEPAGE_H
