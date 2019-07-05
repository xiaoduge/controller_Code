#ifndef EX_LOGINSTATE_H
#define EX_LOGINSTATE_H

#include <QObject>
#include <QDateTime>
#include <QDateTime>

class Ex_LoginState : public QObject
{
    Q_OBJECT
public:
    explicit Ex_LoginState(QObject *parent = 0);
    ~Ex_LoginState();

public:
    void setLoginState(bool login);
    bool loginState();

    void checkAutoLogout();
    
signals:
    
public slots:

private:

    bool m_loginState;
};

#endif // EX_LOGINSTATE_H
