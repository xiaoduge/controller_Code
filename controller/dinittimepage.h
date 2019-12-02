#ifndef DINITTIMEPAGE_H
#define DINITTIMEPAGE_H

#include "subpage.h"

class MainWindow;
class QPushButton;
class QLabel;
class QComboBox;
class QCalendarWidget;
class CBitmapButton;


class DInitTimePage : public CSubPage
{
    Q_OBJECT

public:
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

public:
    DInitTimePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

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

private:
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

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

public slots:
    void on_btn_clicked(int);

    void on_ExNextBtn_clicked();
    void on_ExBackBtn_Clicked();

};


#endif // EX_INIT_TIMEPAGE_H
