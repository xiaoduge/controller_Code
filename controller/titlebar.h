#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "cbitmapbutton.h"

enum TITLE_BAR_ID_ENUM
{
    TITLE_BAR_ID_POWER_OFF = 0,
    TITLE_BAR_ID_BACK,
    TITLE_BAR_ID_HOME,
};

class CBitmapButton;

class titleBar : public QWidget
{
    Q_OBJECT
public:
    explicit titleBar(QWidget *parent = 0,int style = 0);
    
    void display(bool bShow);
signals:
    void clicked();
    void clicked(int);
public slots:

    void on_btn_clicked(int id);

protected:
    virtual void paintEvent(QPaintEvent *pe);

private:
    //CBitmapButton *m_pBtnPower;
    CBitmapButton *m_pBtnHome;
    CBitmapButton *m_pBtnBack;
    
    QPixmap *m_pNaviBar;
    QPixmap *m_pBarSeperator;

    int     m_style;

};

#endif // TITLE_BAR_H
