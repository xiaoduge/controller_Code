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
    void setStartTime(QDateTime& startTime);
    void setEndTime(QDateTime& endTime);
    int getInterval();
    bool getLoginState(int minute);
    
signals:
    
public slots:

private:
    QDateTime m_startTime;
    QDateTime m_endTime;
    bool m_loginState;
};

#endif // EX_LOGINSTATE_H
