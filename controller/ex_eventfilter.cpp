#include "ex_eventfilter.h"
#include <QEvent>
#include "ExtraDisplay.h"
#include <QMutexLocker>
#include "mainwindow.h"
#include "helper.h"

Ex_EventFilter::Ex_EventFilter(QObject *parent) :
    QObject(parent)
{
}

bool Ex_EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMutexLocker locker(&ex_gMutex);
        g_screenSleep = 0;
        g_AutoLogoutTimer = 0;

        if(g_isScreenSleep)
        {
//            int value = gGlobalParam.MiscParam.iBrightness;
//            Write_sys_int(PWMLCD_FILE, value);
            g_isScreenSleep = false;
            return true;
        }

    }
    return QObject::eventFilter(watched, event);
}
