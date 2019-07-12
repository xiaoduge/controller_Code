#include "ex_checkconsumaleinstall.h"
#include "Display.h"
#include "mainwindow.h"
#include "Ex_Display_c.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

extern QString select_sql_Consumable;
extern QString insert_sql_Consumable;
extern QString update_sql_Consumable;

Ex_CheckConsumaleInstall::Ex_CheckConsumaleInstall(int id, QObject *parent) :
    QObject(parent),
    m_instanceID(id)
{
    initSqlDatabase();

    initRfid();
    initTypeMap();
    initCategoryMap();
    m_isRfidType = false;
    m_isBusy = false;
}

Ex_CheckConsumaleInstall::~Ex_CheckConsumaleInstall()
{
    if(m_db.isOpen())
    {
        m_db.close();
    }
}

int Ex_CheckConsumaleInstall::consumableType()
{
    return m_iType;
}

void Ex_CheckConsumaleInstall::initRfid()
{
    m_iRfid[PREPACK_CATNO] = APP_RFID_SUB_TYPE_PREPACK;
    m_iRfid[ACPACK_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[TPACK_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[PPACK_CATNO] = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
    m_iRfid[ATPACK_CATNO] =  APP_RFID_SUB_TYPE_HPACK_ATPACK;

    if(MACHINE_L_EDI_LOOP == gGlobalParam.iMachineType)
    {
        m_iRfid[HPACK_CATNO] = APP_RFID_SUB_TYPE_UPACK_HPACK;
    }
    else
    {
        m_iRfid[HPACK_CATNO] = APP_RFID_SUB_TYPE_HPACK_ATPACK;
    }
    m_iRfid[UPACK_CATNO] = APP_RFID_SUB_TYPE_UPACK_HPACK;
    m_iRfid[UV254_CATNO] =  APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[UV185_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[UVTANK_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[TANKVENTFILTER_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[FINALFILTER_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[UPPUMP_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[ROPACK_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[ROPUMP_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    m_iRfid[EDI_CATNO] = APP_RFID_SUB_TYPE_ROPACK_OTHERS;

}

bool Ex_CheckConsumaleInstall::check(int iRfId)
{
    m_isBusy = true;
    int iRet;
    m_curRfId = iRfId;
    CATNO cn;
    LOTNO ln;

    memset(cn,0,sizeof(CATNO));
    memset(ln,0,sizeof(LOTNO));

    iRet = gpMainWnd->readRfid(m_curRfId);
    if (iRet)
    {
        qDebug() << "readRfid failed";
    }


    gpMainWnd->getRfidCatNo(m_curRfId, cn);
    gpMainWnd->getRfidLotNo(m_curRfId, ln);
    gpMainWnd->getRfidInstallDate(m_curRfId, &m_installDate);

    m_catNo = cn;
    m_lotNo = ln;

    parseType();
    qDebug() << QString("CheckConsumale instanceID: %1 ,RfID: %2, cat: %3, lot: %4")
                .arg(m_instanceID).arg(m_curRfId).arg(m_catNo).arg(m_lotNo);
    return true;
}

void Ex_CheckConsumaleInstall::parseType()
{
    m_iType = DISP_CM_NAME_NUM;

    for(int i = 0; i < CAT_NUM; i++)
    {
        if (MainWindow::consumableCatNo(static_cast<CONSUMABLE_CATNO>(i)).contains(m_catNo))
        {
            m_iType = m_typeMap.value(i);
            m_category = m_categoryMap.value(i);
            if(m_iRfid[i] == m_curRfId)
            {
                m_isRfidType = true;
            }
            else
            {
                m_isRfidType = false;
            }
            break;
        }
    }
}

bool Ex_CheckConsumaleInstall::newPack()
{
    if(m_installDate.toString("yyyy-MM-dd") == gpMainWnd->consumableInitDate())
    {
        return true;
    }
    return false;
}

bool Ex_CheckConsumaleInstall::writeInstallDate()
{
    QString curDate = QDate::currentDate().toString("yyyy-MM-dd");
    int iRet = gpMainWnd->writeRfid(m_curRfId, RF_DATA_LAYOUT_INSTALL_DATE, curDate);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write install date error"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool Ex_CheckConsumaleInstall::clearVolofUse()
{
    int iRet = gpMainWnd->writeRfid(m_curRfId, RF_DATA_LAYOUT_UNKNOW_DATA, "0");
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write vol data error"), QMessageBox::Ok);
        return false;
    }
    return true;
}

void Ex_CheckConsumaleInstall::initTypeMap()
{
    m_typeMap.insert(PPACK_CATNO, DISP_P_PACK);
    m_typeMap.insert(ACPACK_CATNO, DISP_AC_PACK);
    m_typeMap.insert(UPACK_CATNO, DISP_U_PACK);
    m_typeMap.insert(HPACK_CATNO, DISP_H_PACK);
    m_typeMap.insert(PREPACK_CATNO, DISP_PRE_PACK);
    m_typeMap.insert(CLEANPACK_CATNO, DISP_P_PACK | (1 <<16));
    m_typeMap.insert(ATPACK_CATNO, DISP_AT_PACK);
    m_typeMap.insert(TPACK_CATNO, DISP_T_PACK);
    m_typeMap.insert(ROPACK_CATNO, DISP_MACHINERY_RO_MEMBRANE);
    m_typeMap.insert(UV185_CATNO, DISP_N2_UV);
    m_typeMap.insert(UV254_CATNO, DISP_N1_UV);
    m_typeMap.insert(UVTANK_CATNO, DISP_N3_UV);
    m_typeMap.insert(ROPUMP_CATNO, DISP_MACHINERY_RO_BOOSTER_PUMP);
    m_typeMap.insert(UPPUMP_CATNO, DISP_MACHINERY_CIR_PUMP);
    m_typeMap.insert(FINALFILTER_CATNO, DISP_T_A_FILTER);
    m_typeMap.insert(EDI_CATNO, DISP_MACHINERY_EDI);
    m_typeMap.insert(TANKVENTFILTER_CATNO, DISP_W_FILTER);
}

void Ex_CheckConsumaleInstall::initCategoryMap()
{
    m_categoryMap.insert(PPACK_CATNO, 0);
    m_categoryMap.insert(ACPACK_CATNO, 0);
    m_categoryMap.insert(UPACK_CATNO, 0);
    m_categoryMap.insert(HPACK_CATNO, 0);
    m_categoryMap.insert(PREPACK_CATNO, 0);
    m_categoryMap.insert(CLEANPACK_CATNO, 0);
    m_categoryMap.insert(ATPACK_CATNO, 0);
    m_categoryMap.insert(TPACK_CATNO, 0);
    m_categoryMap.insert(ROPACK_CATNO, 1);
    m_categoryMap.insert(UV185_CATNO, 0);
    m_categoryMap.insert(UV254_CATNO, 0);
    m_categoryMap.insert(UVTANK_CATNO, 0);
    m_categoryMap.insert(ROPUMP_CATNO, 1);
    m_categoryMap.insert(UPPUMP_CATNO, 1);
    m_categoryMap.insert(FINALFILTER_CATNO, 0);
    m_categoryMap.insert(EDI_CATNO, 1);
    m_categoryMap.insert(TANKVENTFILTER_CATNO, 0);
}

bool Ex_CheckConsumaleInstall::checkDatabaseConnect()
{
    if(m_db.isOpen())
    {
        return true;
    }
    else
    {
        return initSqlDatabase();
    }

}

bool Ex_CheckConsumaleInstall::initSqlDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", QString("CIC%1").arg(m_instanceID));
    m_db.setHostName("SHZN");
    m_db.setDatabaseName("SHZN");
    m_db.setUserName("SHZN");
    m_db.setPassword("SHZN");
    if(m_db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
  return 0 : do nothing
  return 1 : insert new
  return 2 : update
  return 3 : only write install date to RFID
*/
bool Ex_CheckConsumaleInstall::comparedWithSql()
{
    if(!checkDatabaseConnect())
    {
        return false;
    }

    QSqlQuery query(m_db);
    bool ret;

    query.prepare(select_sql_Consumable);
    query.addBindValue(m_iType);
    query.addBindValue(m_category);
    ret = query.exec();
    qDebug() << QString("%1, consumable compared with Sql: %2").arg(m_instanceID).arg(ret);
    if(query.next())
    {
        QString lotno = query.value(0).toString();
        if(m_lotNo == lotno)
        {
            if(newPack())
            {
                m_operateID = 3;
                emit consumableMsg(m_iType, m_catNo, m_lotNo);
                qDebug() << QString("%1, consumable compared with Sql: write install date").arg(m_instanceID);
                return true; //update
            }
            else
            {
                m_isBusy = false;
                qDebug() << QString("%1, consumable compared with Sql: existence").arg(m_instanceID);
                return false; // do nothing
            }
        }
        else
        {
            m_operateID = 2;
            emit consumableMsg(m_iType, m_catNo, m_lotNo);
            qDebug() << QString("%1, consumable compared with Sql: update").arg(m_instanceID);
            return true; //update
        }
    }
    else
    {
        m_operateID = 1;
        emit consumableMsg(m_iType, m_catNo, m_lotNo);
        qDebug() << QString("%1, consumable compared with Sql: insert new").arg(m_instanceID);
        return true;   //insert new
    }

}

void Ex_CheckConsumaleInstall::updateSql()
{
    QString strCurDate = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query(m_db);
    query.prepare(update_sql_Consumable);
    query.addBindValue(m_catNo);
    query.addBindValue(m_lotNo);
    query.addBindValue(strCurDate);
    query.addBindValue(m_iType);
    query.addBindValue(m_category);
    bool ret = query.exec();
    qDebug() << QString("%1, consumable update Sql: %1").arg(m_instanceID).arg(ret);

    if(ret)
    {
        if((DISP_P_PACK == m_iType) && (!ex_isPackNew))
        {
            ex_isPackNew = 1;
        }
    }

}

void Ex_CheckConsumaleInstall::insertSql()
{
    QString strCurDate = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query(m_db);
    query.prepare(insert_sql_Consumable);
    query.bindValue(":iPackType", m_iType);
    query.bindValue(":CatNo", m_catNo);
    query.bindValue(":LotNo", m_lotNo);
    query.bindValue(":category", m_category);
    query.bindValue(":time", strCurDate);
    bool ret = query.exec();
    qDebug() << QString("%1, consumable insert Sql: %1").arg(m_instanceID).arg(ret);

    if(ret)
    {
        if((DISP_P_PACK == m_iType) && (!ex_isPackNew))
        {
            ex_isPackNew = 1;
        }
    }

}

bool Ex_CheckConsumaleInstall::isCorrectRfId()
{
    if(!m_isRfidType)
    {
        m_isBusy = false;
    }
    return m_isRfidType;
}

bool Ex_CheckConsumaleInstall::ischeckBusy()
{
    return m_isBusy;
}

void Ex_CheckConsumaleInstall::setBusystatus(bool isBusy)
{
    m_isBusy = isBusy;
}

void Ex_CheckConsumaleInstall::updateConsumableType(int iType)
{
    m_iType = iType;
}

void Ex_CheckConsumaleInstall::updateConsumaleMsg()
{
    if(!checkDatabaseConnect())
    {
        return;
    }

    switch(m_operateID)
    {
    case 1:
    {
        insertSql();
        if(newPack())
        {
            writeInstallDate();
            clearVolofUse();
        }
        MainResetCmInfo(m_iType);
        break;
    }
    case 2:
    {
        updateSql();
        if(newPack())
        {
            writeInstallDate();
            clearVolofUse();
        }
        MainResetCmInfo(m_iType);
        break;
    }
    case 3:
    {
        writeInstallDate();
        clearVolofUse();

        MainResetCmInfo(m_iType);
        break;
    }
    default:
        break;
    }
    m_isBusy = false;
    MainRetriveExConsumableMsg(gGlobalParam.iMachineType, gGlobalParam.cmSn, gGlobalParam.macSn);

    CATNO cat;
    LOTNO lot;
    memset(cat, 0, sizeof(CATNO));
    memset(lot, 0, sizeof(LOTNO));

    strncpy(cat, m_catNo.toAscii(), APP_CAT_LENGTH);
    strncpy(lot, m_lotNo.toAscii(), APP_LOT_LENGTH);

    if(m_category)
    {
        gpMainWnd->MainWriteMacInstallInfo2Db(m_iType, 0, cat, lot);
    }
    else
    {
        gpMainWnd->MainWriteCMInstallInfo2Db(m_iType, 0, cat, lot);
    }

    emit consumableInstallFinished(m_iType);
}
