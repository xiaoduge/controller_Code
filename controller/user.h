#ifndef USE_H
#define USE_H

#include <QObject>
#include <QLinkedList>
#include <QList>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

    QString strName;
    QString strPassWord;
    int iPermission;
    float fQuantity;
    QString strTime;

    static int UserNum;
    
signals:
    
public slots:
    
};

extern User *gpActiveU;

extern QLinkedList<User *>gUserList;

#endif // USE_H
