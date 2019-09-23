#include "ex_eventfilter.h"
#include <QEvent>
#include <QMouseEvent>
#include "ExtraDisplay.h"
#include <QMutexLocker>
#include "mainwindow.h"
#include "helper.h"
#include "ExtraDisplay.h"

Ex_EventFilter::Ex_EventFilter(QObject *parent) :
    QObject(parent)
{
}

bool Ex_EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        gpMainWnd->stopBuzzing();

        QMutexLocker locker(&ex_gMutex);
#ifdef TOUCHTEST
        touchTest(event); //print mouse press point
#endif
        g_screenSleep = 0;
        g_AutoLogoutTimer = 0;

        if(g_isScreenSleep)
        {
            g_isScreenSleep = false;
            return true;
        }

    }
    return QObject::eventFilter(watched, event);
}

#ifdef TOUCHTEST
void Ex_EventFilter::touchTest(QEvent *e)
{
    QMouseEvent *event = static_cast<QMouseEvent*>(e);
    qDebug() << ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo <<" touch test: "  << event->pos();
}
#endif
