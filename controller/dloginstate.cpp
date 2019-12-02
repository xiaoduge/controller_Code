#include "dloginstate.h"
#include "exconfig.h"
#include <QDateTime>

DLoginState::DLoginState(QObject *parent) :
    QObject(parent),
    m_loginState(false)
{

}

DLoginState::~DLoginState()
{

}

void DLoginState::setLoginState(bool login)
{
    m_loginState = login;
}

bool DLoginState::loginState()
{
    return m_loginState;
}

void DLoginState::checkAutoLogout()
{
    if(gAutoLogoutTimer >= (gGlobalParam.MiscParam.iAutoLogoutTime * 60))
    {
        setLoginState(false);
    }
}
