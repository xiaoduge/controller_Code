#include "ex_loginstate.h"
#include "ExtraDisplay.h"
#include <QDateTime>

Ex_LoginState::Ex_LoginState(QObject *parent) :
    QObject(parent),
    m_loginState(false)
{

}

Ex_LoginState::~Ex_LoginState()
{

}

void Ex_LoginState::setLoginState(bool login)
{
    m_loginState = login;
}

bool Ex_LoginState::loginState()
{
    return m_loginState;
}

void Ex_LoginState::checkAutoLogout()
{
    if(g_AutoLogoutTimer >= (gGlobalParam.MiscParam.iAutoLogoutTime * 60))
    {
        setLoginState(false);
    }
}
