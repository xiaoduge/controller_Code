#include "ex_userinfo.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include <QSqlDatabase>
#include <QSqlQuery>

Ex_UserInfo::Ex_UserInfo(QObject *parent) :
    QObject(parent)
{
}

int Ex_UserInfo::checkUserInfo(const QString &userName, const QString &passWord)
{
    //超级用户，内部使用
    if((ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer.right(5) == QString("debug"))
        && (!userName.isEmpty()))
    {
        return 4;
    }
    if((userName.compare("Super", Qt::CaseInsensitive) == 0) && (passWord.compare("888888") == 0))
    {
        return 4;
    }
    //工程师权限，
    if((userName.compare("Service", Qt::CaseInsensitive) == 0) && (passWord.compare("860860") == 0))
    {
        return 3;
    }

    //用户管理权限，
    if((userName.compare("Manager", Qt::CaseInsensitive) == 0) && (passWord.compare("111111") == 0))
    {
        return 2;
    }

    QString strQuery = "select * from User where Permission = 2";
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        QString pass = query.value(2).toString();
        qDebug() << "Name: " << name;
        qDebug() << "Password: " << pass;
        if((userName.compare(name, Qt::CaseInsensitive) == 0) && (passWord.compare(pass, Qt::CaseInsensitive) == 0))
        {
            return 2;
        }
    }

    strQuery = "select * from User where Permission = 1";
    query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        QString pass = query.value(2).toString();
        qDebug() << "Name: " << name;
        qDebug() << "Password: " << pass;
        if((userName.compare(name, Qt::CaseInsensitive) == 0) && (passWord.compare(pass, Qt::CaseInsensitive) == 0))
        {
            return 1;
        }
    }
    return 0;
}

bool Ex_UserInfo::checkManagerInfo(const QString &userName)
{
    if(userName.compare("Super", Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare("Service", Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare("Manager", Qt::CaseInsensitive) == 0)
    {
        return true;
    }

    QString strQuery = "select * from User where Permission = 2";
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        if(userName.compare(name, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }

    return false;
}
