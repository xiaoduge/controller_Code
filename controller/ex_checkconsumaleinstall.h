#ifndef EX_CHECKCONSUMALEINSTALL_H
#define EX_CHECKCONSUMALEINSTALL_H

#include <QObject>
#include <QStringList>
#include "mainwindow.h"

class Ex_CheckConsumaleInstall : public QObject
{
    Q_OBJECT
public:
    explicit Ex_CheckConsumaleInstall(int id, QObject *parent = 0);

    void check(int iRfId);

    int consumableType();
    bool comparedWithSql();
    void updateSql();
    void insertSql();
    bool isCorrectRfId();
    bool ischeckBusy();
    
signals:
    void consumableMsg(int iType, const QString& catNo, const QString& lotNo);
    
public slots:
    void updateConsumaleMsg();
    void setBusystatus(bool isBusy);
    void updateConsumableType(int iType);

private:
    void initRfid();
    void parseType();
    bool isNewPack();
    bool writeInstallDate();
    bool clearVolofUse();

private:
    int m_instanceID;

    int m_iRfid[CAT_NUM];
    int m_curRfId;
    int m_iType;
    int m_category;
    QString m_catNo;
    QString m_lotNo;
    QDate m_installDate;
    bool m_isRfidType;
    int m_operateID;
    bool m_isBusy;
    
};

#endif // EX_CHECKCONSUMALEINSTALL_H
