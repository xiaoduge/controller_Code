#ifndef ALLOCATION_SET_PAGE_H
#define ALLOCATION_SET_PAGE_H

#include "subpage.h"

class MainWindow;
class QComboBox;
class CBitmapButton;
class DLineEdit;

enum ALLOCPAGE_BTN_NAME
{
    ALLOCPAGE_BTN_SUNDAY = 0, 
    ALLOCPAGE_BTN_MONDAY,
    ALLOCPAGE_BTN_TUESDAY,
    ALLOCPAGE_BTN_WEDNESDAY,
    ALLOCPAGE_BTN_THURSDAY,
    ALLOCPAGE_BTN_FRIDAY,
    ALLOCPAGE_BTN_SATURDAY,
    ALLOCPAGE_BTN_SAVE,
    ALLOCPAGE_BTN_NUM
};


class AllocationSetPage : public CSubPage
{
    Q_OBJECT
public:
    AllocationSetPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();
    
    virtual void leaveSubPage();

private:

    void setBackColor();
    void save();
    void buildTitles();

    QLabel    *m_lbNameTitle;
    CBitmapButton *m_pBtnEnable;

    QLabel    *m_lbNameStartTime;
    QComboBox  *m_cmbStartTimeHour;
    QComboBox  *m_cmbStartTimeMin;

    QLabel    *m_lbNameEndTime;
    QComboBox  *m_cmbEndTimeHour;
    QComboBox  *m_cmbEndTimeMin;

    QLabel    *m_lbNameWeekday;
    CBitmapButton *m_pBtns[ALLOCPAGE_BTN_NUM];

    QLabel    *m_lbNameIdlePeriod;
    DLineEdit *m_leIdlePeriod;
    QLabel    *m_lbNameIdlePeriodUnit;
    
    QLabel    *m_lbNameIdleCirTime;
    DLineEdit *m_leIdleCirTime;
    QLabel    *m_lbNameIdleCirTimeUnit;

    QWidget   *m_pBackWidget[6];

    CBitmapButton     *m_pBtnSave;   

public slots:
    void on_btn_clicked(int);
};

#endif // TIMEPAGE_H
