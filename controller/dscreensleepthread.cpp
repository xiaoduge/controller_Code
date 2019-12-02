#include "dscreensleepthread.h"
#include "exconfig.h"
#include "mainwindow.h"
#include <QMutexLocker>
#include <QTimerEvent>
#include "exconfig.h"

const int iTimeF = 1000* 10; //10s

DScreenSleepThread::DScreenSleepThread(QObject *parent) :
    QThread(parent)
{
    gScreenSleepTimer = 0;
}

DScreenSleepThread::~DScreenSleepThread()
{

}

void DScreenSleepThread::run()
{
    m_timerID = startTimer(iTimeF);

    exec();
}

void DScreenSleepThread::stop()
{
    killTimer(m_timerID);
    quit();
}

void DScreenSleepThread::timerEvent(QTimerEvent *event)
{
    if(m_timerID == event->timerId())
    {
        QMutexLocker locker(&ex_gMutex);
        gScreenSleepTimer++;

        if(gScreenSleepTimer == gAdditionalCfgParam.additionalParam.iScreenSleepTime * 6)
        {
            emit screenSleep(true);
        }
    }
}




