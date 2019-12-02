#ifndef DSCREENSLEEPTHREAD_H
#define DSCREENSLEEPTHREAD_H

#include <QThread>
#include <QMutex>


class DScreenSleepThread : public QThread
{
    Q_OBJECT
public:
    explicit DScreenSleepThread(QObject *parent = 0);
    ~DScreenSleepThread();

public:
    void run();
    void stop();

protected:
    void timerEvent(QTimerEvent *event);
    
signals:
    void screenSleep(bool isSleep);
    
public slots:

private:
    int m_timerID;

    
};

#endif // EX_SCREENSLEEPTHREAD_H
