#include "ex_screensleepthread.h"
#include "ExtraDisplay.h"
#include <QMutexLocker>
#include <QTimerEvent>
#include <QDebug>
#include "ExtraDisplay.h"

const int iTimeF = 1000* 10; //10s

Ex_ScreenSleepThread::Ex_ScreenSleepThread(QObject *parent) :
    QThread(parent)
{
    g_screenSleep = 0;
}

Ex_ScreenSleepThread::~Ex_ScreenSleepThread()
{

}

void Ex_ScreenSleepThread::run()
{
    qDebug() << "ScreenSleep thread run()";
    m_timerID = startTimer(iTimeF);

    exec();
}

void Ex_ScreenSleepThread::stop()
{
    killTimer(m_timerID);
    quit();
}

void Ex_ScreenSleepThread::timerEvent(QTimerEvent *event)
{
    if(m_timerID == event->timerId())
    {
        QMutexLocker locker(&m_mutex);
        g_screenSleep++;

        if(g_screenSleep == ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime * 6)
        {
            emit screenSleep(true);
        }
        qDebug() << "g_screenSleep: " << g_screenSleep;
    }
}




