#include "deventfilter.h"
#include <QEvent>
#include <QMouseEvent>
#include "exconfig.h"
#include <QMutexLocker>
#include "mainwindow.h"
#include "helper.h"
#include "exconfig.h"

DEventFilter::DEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool DEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        gpMainWnd->stopBuzzing();

        QMutexLocker locker(&ex_gMutex);
#ifdef TOUCHTEST
        touchTest(event); //print mouse press point
#endif
        gScreenSleepTimer = 0;
        gAutoLogoutTimer = 0;

        if(gScreenSleeping)
        {
            gScreenSleeping = false;
            return true;
        }

    }
    return QObject::eventFilter(watched, event);
}

#ifdef TOUCHTEST
void DEventFilter::touchTest(QEvent *e)
{
    QMouseEvent *event = static_cast<QMouseEvent*>(e);
    qDebug() << gAdditionalCfgParam.productInfo.strSerialNo <<" touch test: "  << event->pos();
}
#endif
