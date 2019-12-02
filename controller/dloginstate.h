#ifndef DLOGINSTATE_H
#define DLOGINSTATE_H

#include <QObject>

class DLoginState : public QObject
{
    Q_OBJECT
public:
    explicit DLoginState(QObject *parent = 0);
    ~DLoginState();

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
