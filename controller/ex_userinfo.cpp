#include "ex_userinfo.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "dloginwarningdialog.h"

Ex_UserInfo::Ex_UserInfo(QObject *parent) :
    QObject(parent)
{
}

int Ex_UserInfo::checkUserInfo(const QString &userName, const QString &passWord)
{
    //超级用户，内部使用
    if((userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Super_Name]) == 0))
    {
        return 4;
    }
    //工程师权限，
    if((userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Service_Name]) == 0))
    {
        return 3;
    }

    //VWR， 超级用户
    if((userName.compare(managerName[Super_Service_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Super_Service_Name]) == 0))
    {
        return 31;
    }

    //用户管理权限，
    if((userName.compare(managerName[Manager_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Manager_Name]) == 0))
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

        if((userName.compare(name, Qt::CaseInsensitive) == 0) && (passWord.compare(pass, Qt::CaseInsensitive) == 0))
        {
            return 1;
        }
    }
    return 0;
}

bool Ex_UserInfo::checkManagerInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Manager_Name], Qt::CaseInsensitive) == 0)
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

bool Ex_UserInfo::checkEngineerInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    return false;
}

bool Ex_UserInfo::checkSuperService(const QString &userName, const QString& password)
{
    //超级用户，内部使用
    if((userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
        && (0 == password.compare(userPassword[Super_Name])))
    {
        return true;
    }

    //VWR， 超级用户
    if((userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
        && (0 == password.compare(userPassword[Super_Service_Name])))
    {
        return true;
    }
    return false;
}

bool Ex_UserInfo::checkSuperInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    return false;
}
