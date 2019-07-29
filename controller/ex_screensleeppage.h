#ifndef EX_SCREENSLEEPPAGE_H
#define EX_SCREENSLEEPPAGE_H

#include "page.h"

class QPushButton;
class QLabel;
class Ex_DSleepWidget;

class Ex_ScreenSleepPage : public CPage
{
    Q_OBJECT
public:
    Ex_ScreenSleepPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow  *wndMain = 0);

    void buildTranslation();
    void update();

signals:
    void pageHide();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);

protected slots:
//    void on_screenBtn_clicked();
    void on_SleepPageShow(bool);

private:
    void initUi();
    int randomPos(int max);

private:
    Ex_DSleepWidget* m_pdwidget;
    QLabel* m_msgLabel;
    int m_timeID;
    QString m_titleMsg;

};

#endif // EX_SCREENSLEEPPAGE_H
