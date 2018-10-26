#ifndef EX_CALCPACKFLOW_H
#define EX_CALCPACKFLOW_H

#include <QDateTime>

const int timeConver = 60*60;

class Ex_CalcPackFlow
{
public:
    Ex_CalcPackFlow();

    void setStartTime(const QDateTime& startTime);
    void setStopTime(const QDateTime& stopTime);
    unsigned int calcFlow(int state);

    void setFlushState(bool state);
    bool isFlushing();

    void setProductState(bool state);
    bool isProducing();

private:
    int timeInterval();

private:
    QDateTime m_startTime;
    QDateTime m_stopTime;

    bool m_isFlush;
    bool m_isProduct;
};

#endif // EX_CALCPACKFLOW_H
