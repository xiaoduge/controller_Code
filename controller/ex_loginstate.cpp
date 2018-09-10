#include "ex_loginstate.h"
#include <QDateTime>

Ex_LoginState::Ex_LoginState(QObject *parent) :
    QObject(parent),
    m_startTime(QDateTime::fromString("1.30.1", "M.d.s")),
    m_loginState(false)
{

}

Ex_LoginState::~Ex_LoginState()
{

}

void Ex_LoginState::setStartTime(QDateTime &startTime)
{
    m_startTime = startTime;
}

void Ex_LoginState::setEndTime(QDateTime &endTime)
{
    m_endTime = endTime;
}

int Ex_LoginState::getInterval()
{
    return m_startTime.secsTo(m_endTime);
}

bool Ex_LoginState::getLoginState(int minute)
{
    if(m_startTime == QDateTime::fromString("1.30.1", "M.d.s"))
    {
        m_loginState = false;
        return m_loginState;
    }
    if(((minute*60) > getInterval()))
    {
        m_loginState = true;
    }
    else
    {
        m_loginState = false;
    }
    return m_loginState;
}
