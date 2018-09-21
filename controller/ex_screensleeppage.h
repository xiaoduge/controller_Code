#ifndef EX_SCREENSLEEPPAGE_H
#define EX_SCREENSLEEPPAGE_H

#include "page.h"

class QPushButton;
class QLabel;

class Ex_ScreenSleepPage : public CPage
{
    Q_OBJECT
public:
    Ex_ScreenSleepPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow  *wndMain = 0);

    void buildTranslation();

signals:
    void pageHide();

protected:
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
//    void on_screenBtn_clicked();

private:
    void initUi();

private:
    QLabel* m_msgLabel;

};

#endif // EX_SCREENSLEEPPAGE_H
