#ifndef DCHECKCONSUMALEINSTALL_H
#define DCHECKCONSUMALEINSTALL_H

#include <QObject>
#include <QStringList>
#include "mainwindow.h"
#include <QMap>
#include <QSqlDatabase>

class DCheckConsumaleInstall : public QObject
{
    Q_OBJECT
    enum InstallAction
    {
        InvalidAction = 0,             //0 : do nothing
        InsertAction,            //1 : insert new
        UpdateAction,            //2 : update     
        WriteInstallDataAction,  //3 : only write install date to RFID
        ActionNum
    };

public:
    explicit DCheckConsumaleInstall(int id, QObject *parent = 0);
    ~DCheckConsumaleInstall();

    bool check(int iRfId);

    int consumableType();
    bool comparedWithSql();
    void updateSql();
    void insertSql();
    bool isCorrectRfId();
    bool ischeckBusy();
    
signals:
    void consumableMsg(int iType, const QString& catNo, const QString& lotNo);

    void consumableInstallFinished(int);
    
public slots:
    void updateConsumaleMsg();
    void setBusystatus(bool isBusy);
    void updateConsumableType(int iType);

private:
    void initRfid();
    void parseType();
    bool newPack();
    bool writeInstallDate();
    bool clearVolofUse();
    void initTypeMap();
    void initCategoryMap();

    bool checkDatabaseConnect();
    bool initSqlDatabase();

    void retriveCMInfoWithRFID(int type);

private:
    int m_instanceID;

    int m_iRfid[CAT_NUM];
    int m_curRfId;
    int m_iType;
    int m_category;
    QString m_catNo;
    QString m_lotNo;
    QDate m_installDate;
    int m_volUsed;
    bool m_isRfidType;
    InstallAction m_operateID;
    bool m_isBusy;
    QMap<short int, short int> m_typeMap;
    QMap<short int, short int> m_categoryMap;

    QSqlDatabase m_db;
};

#endif // EX_CHECKCONSUMALEINSTALL_H
