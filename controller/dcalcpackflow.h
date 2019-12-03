/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-3
** @version   v0.0.1
** @brief     Interface for water usage calculation
**
****************************************************************************/

#ifndef DCALCPACKFLOW_H
#define DCALCPACKFLOW_H

#include <QDateTime>

const int timeConver = 60*60;

class DCalcPackFlow
{
public:
    DCalcPackFlow();

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
