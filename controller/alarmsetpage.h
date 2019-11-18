#ifndef ALARMSETPAGE_H
#define ALARMSETPAGE_H

#include "subpage.h"
#include "Display.h"

#define ALARMSETNUM (DISP_ALARM_PART0_NUM + DISP_ALARM_PART1_NUM)

class QListWidget;
class QListWidgetItem;
class AlarmSetListwidgtItem;
class CBitmapButton;

class AlarmSetPage : public CSubPage
{
    Q_OBJECT

public:
    AlarmSetPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    
    virtual void update();

private:

    void buildTitles();

    void createList();

    void setBackColor();

    void save();

    struct AS_ITEM_TYPE {
        int iId;
    }aIds[ALARMSETNUM];

    QListWidget *m_plistWidget;

    QListWidgetItem *m_listWidgetIem[ALARMSETNUM];

    AlarmSetListwidgtItem *m_plistItem[ALARMSETNUM];
    CBitmapButton         *m_pBtnSave;
    int                    m_iRealNum;

public slots:
    void ItemClicked(QListWidgetItem * item);
    void on_btn_clicked(int);
};

#endif // SYSTESTPAGE_H
