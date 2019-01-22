#ifndef EX_SCREENSLEEPTHREAD_H
#define EX_SCREENSLEEPTHREAD_H

#include <QThread>
#include <QMutex>


class Ex_ScreenSleepThread : public QThread
{
    Q_OBJECT
public:
    explicit Ex_ScreenSleepThread(QObject *parent = 0);
    ~Ex_ScreenSleepThread();

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
