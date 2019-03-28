#ifndef EX_USERINFO_H
#define EX_USERINFO_H

#include <QObject>

class Ex_UserInfo : public QObject
{
    Q_OBJECT
public:
    explicit Ex_UserInfo(QObject *parent = 0);

    int checkUserInfo(const QString& userName, const QString& passWord);
    bool checkManagerInfo(const QString &userName);
    
signals:
    
public slots:
    
};

#endif // EX_USERINFO_H
