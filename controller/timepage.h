#ifndef TIMEPAGE_H
#define TIMEPAGE_H

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
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include <QComboBox>
#include <QCalendarWidget>
#include "cbitmapbutton.h"

class MainWindow;

typedef enum
{
    DATE_SET = 0,
    TIME_SET,
    DATE_NUM
}DATE_SET_ENUM;

enum TIMEPAGE_BTN_ENUM
{
    TIMEPAGE_BTN_DATE_SET = 0,
    TIMEPAGE_BTN_TIME_SET,
    TIMEPAGE_BTN_CANCEL,
    TIMEPAGE_BTN_OK,
    TIMEPAGE_BTN_NUM
};


class TimePage : public CSubPage
{
    Q_OBJECT
public:
    TimePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    ~TimePage();
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

private:

    void setBackColor();

    void buildTitles();

    void Date_show_hide(bool tmp);

    void Time_show_hide(bool tmp);

    void createHeads();

    void Create_Cal_Ok();

    void Create_Date_Time();

    void on_Ok_clicked();

    void on_Cancel_clicked();
    
    void changeTime();

    QLabel *lbTitPic;
    QLabel *lbTitName;

    QCalendarWidget *CalS;

    QLabel *lbName[DATE_NUM];
    QWidget   *ShowWidget[DATE_NUM];

    QComboBox *TimeHCbox;
    QComboBox *TimeMCbox;
    QComboBox *TimeSCbox;

    CBitmapButton *m_pBtns[TIMEPAGE_BTN_NUM];
    
    QString m_astrDateName[DATE_NUM];

public slots:

    void on_btn_clicked(int);

};

#endif // TIMEPAGE_H
