#ifndef NAVIGATOR_BAR_H
#define NAVIGATOR_BAR_H

#include <QWidget>


enum NBAR_ID_ENUM
{
    NBAR_ID_POWER_OFF = 0
};

enum NBAR_LABEL_NAME
{
    NBAR_LABEL_DATE = 0,

    NBAR_LABEL_TIME,
    
    NBAR_LABEL_NUMBER
};


class CBitmapButton;
class QLabel;

class navigatorBar : public QWidget
{
    Q_OBJECT
public:
    explicit navigatorBar(QWidget *parent = 0);
    
    void display(bool bShow);
    void updateRunInfo(bool bRun);
    void setShowDuration(int iDuration){m_iDuration = iDuration;}
    bool getState();

signals:
    void clicked();
    void clicked(int);
public slots:

    void on_btn_clicked(int id);
    void on_timerEvent();

protected:
    virtual void paintEvent(QPaintEvent *pe);

private:
    void showTime();

private:
    CBitmapButton *m_pBtnPower;
    QPixmap *m_pNaviBar;
    QPixmap *m_pTimeBar;
    QLabel *m_pLabelDate;
    QLabel *m_pLabelTime;
    QFont  *m_pDateFont;
    QFont  *m_pTimeFont;
    QTimer *m_timeTimer;

    int     m_timerCnt;
    int     m_iDuration;

};

#endif // NAVIGATOR_BAR_H
