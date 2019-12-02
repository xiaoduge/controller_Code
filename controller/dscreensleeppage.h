#ifndef DSCREENSLEEPPAGE_H
#define DSCREENSLEEPPAGE_H

#include "page.h"

class QPushButton;
class QLabel;
class DSleepWidget;

class DScreenSleepPage : public CPage
{
    Q_OBJECT
public:
    DScreenSleepPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow  *wndMain = 0);

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

    void buildTranslationTitleRephile();
    void buildTranslationTitleVWR();

private:
    DSleepWidget* m_pdwidget;
    QLabel* m_msgLabel;
    int m_timeID;
    QString m_titleMsg;

};

#endif // EX_SCREENSLEEPPAGE_H
