#include "dcalcpackflow.h"
#include "mainwindow.h"
#include "exconfig.h"

DCalcPackFlow::DCalcPackFlow()
{
    m_startTime = QDateTime::currentDateTime();
    m_stopTime = m_startTime;
    m_isFlush = false;
    m_isProduct = false;
}

void DCalcPackFlow::setStartTime(const QDateTime& startTime)
{
    m_startTime = startTime;
}

void DCalcPackFlow::setStopTime(const QDateTime& stopTime)
{
    m_stopTime = stopTime;
}

unsigned int DCalcPackFlow::calcFlow(int state)
{
    int interval = 0;
    float hr = 0.0;
    unsigned int ulQuantity = 0;
    int rate = 0;
    unsigned int machineFlow = gAdditionalCfgParam.machineInfo.iMachineFlow;

    switch(state)
    {
    case 0:
        rate = 120;
        break;
    case 1:
    {
        rate = 39;
        switch(machineFlow)
        {
        case 5:
            rate += 8;
            break;
        case 10:
            rate += 16;
            break;
        case 12:
            rate += 12;
            break;
        case 15:
            rate += 24;
            break;
        case 24:
            rate += 24;
            break;
        case 32:
            rate += 32;
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    interval = timeInterval();
    hr = (interval*1.0)/(timeConver*1.0);
    ulQuantity = (rate * 1000 * hr) + 0.5;

    return ulQuantity;
}

void DCalcPackFlow::setFlushState(bool state)
{
    m_isFlush = state;
}

bool DCalcPackFlow::isFlushing()
{
    return m_isFlush;
}

void DCalcPackFlow::setProductState(bool state)
{
    m_isProduct = state;
}

bool DCalcPackFlow::isProducing()
{
    return m_isProduct;
}

int DCalcPackFlow::timeInterval()
{
    int tmp = m_startTime.secsTo(m_stopTime);
    return tmp;
}
