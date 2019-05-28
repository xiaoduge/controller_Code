#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/vt.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <semaphore.h>
#include <linux/fb.h>

#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>

#include "ctrlapplication.h"
#include "user.h"
#include "keyboard.h"
#include "LoginDlg.h"

#include "syszuxim.h"
#include "syszuxpinyin.h"
#include "cminterface.h"

#include "ex_eventfilter.h"

CtrlApplication *gApp;


QString gastrDbTables[DB_TBL_NUM] = 
{
    "Water",
    "Alarm",
    "User",
    "GetW",
    "PWater",
    "Log",
};


QString CREATE_TABLE_Water = "CREATE TABLE IF NOT EXISTS  Water (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "ecoid INTEGER NULL,"
                                                                  "quality double NULL,"
                                                                  "time VARCHAR(20) NOT NULL)";

QString CREATE_TABLE_Alarm = "CREATE TABLE IF NOT EXISTS  Alarm (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "type VARCHAR(20) NOT NULL,"
                                                                  "status INTEGER NULL,"
                                                                  "time VARCHAR(20) NOT NULL)";

QString CREATE_TABLE_User = "CREATE TABLE IF NOT EXISTS  User(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "name VARCHAR(20) UNIQUE NOT NULL ,"
                                                                  "Password VARCHAR(20) NOT NULL,"
                                                                  "Permission INTEGER NOT NULL)";

QString CREATE_TABLE_GetW = "CREATE TABLE IF NOT EXISTS  GetW (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "name VARCHAR(20) NOT NULL,"
                                                                  "quantity double NULL,"
                                                                  "quality double NULL,"
                                                                  "tmp double NULL,"
                                                                  "time VARCHAR(20) NOT NULL)";

QString CREATE_TABLE_Produce_Water = "CREATE TABLE IF NOT EXISTS  PWater (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "duration INTEGER NULL,"
                                                                  "ecoroin double NULL,"
                                                                  "tmproin double NULL,"
                                                                  "ecorores double NULL,"
                                                                  "ecoropw double NULL,"
                                                                  "tmpropw double NULL,"
                                                                  "ecoedi double NULL,"
                                                                  "tmpedi double NULL,"
                                                                  "time VARCHAR(20) NOT NULL)";

QString CREATE_TABLE_Log = "CREATE TABLE IF NOT EXISTS  Log (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "name VARCHAR(20) NOT NULL,"
                                                                  "action VARCHAR(40) NOT NULL,"
                                                                  "info VARCHAR(40) NOT NULL,"
                                                                  "time VARCHAR(20) NOT NULL)";

QString CREATE_TABLE_Handler = "CREATE TABLE IF NOT EXISTS  Handler (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "name VARCHAR(20) UNIQUE NOT NULL,"
                                                                  "address INTEGER NOT NULL,"
                                                                  "type INTEGER NOT NULL,"
                                                                  "def INTEGER NOT NULL)";

QString CREATE_TABLE_Rfid     = "CREATE TABLE IF NOT EXISTS  Rfid (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                  "name VARCHAR(20) UNIQUE NOT NULL,"
                                                                  "address INTEGER NOT NULL,"
                                                                  "type INTEGER NOT NULL)";

QString CREATE_TABLE_Consumable = "CREATE TABLE IF NOT EXISTS  Consumable (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                          "iPackType INTEGER NOT NULL,"
                                                                          "CatNo VARCHAR(20) NOT NULL,"
                                                                          "LotNO VARCHAR(20) NOT NULL,"
                                                                          "category INTEGER NOT NULL,"
                                                                          "time VARCHAR(20) NOT NULL)";

QString select_sql_Water = "SELECT id, ecoid, quality, time FROM Water where time > ? and time < ?";
QString delete_sql_Water = "delete from Water where time > ? and time < ?";

QString select_sql_Alarm = "SELECT id, type, status, time FROM Alarm  where time > ? and time < ?";
QString delete_sql_Alarm = "delete from Alarm where time > ? and time < ?";

QString select_sql_GetW  = "SELECT id, name, quantity, quality, tmp, time FROM GetW where time > ? and time < ?";
QString delete_sql_GetW  = "delete from GetW where time > ? and time < ?";

QString select_sql_PW    = "SELECT id, time, ecoroin, tmproin, ecorores, ecoropw, tmpropw, ecoedi, tmpedi, duration FROM PWater where time > ? and time < ?";
QString delete_sql_PW    = "delete from PWater where time > ? and time < ?";

QString select_sql_Log    = "SELECT id, name, action, info, time FROM Log where time > ? and time < ?";
QString delete_sql_Log    = "delete from Log where time > ? and time < ?";

//QString UPDATE_sql_Water = "UPDATE Water SET quality = ? , time = ? WHERE id = ? and quality > ? and time > ? and time < ?";

QString INSERT_sql_Water = "INSERT INTO Water (id, ecoid, quality, time) VALUES (:id, :ecoid, :quality, :time)";
QString INSERT_sql_Alarm = "INSERT INTO Alarm (id, type, status ,time) VALUES (:id, :type, :status, :time)";
QString INSERT_sql_GetW  = "INSERT INTO GetW (id, name, quantity, quality, tmp, time) VALUES (:id, :name, :quantity, :quality, :tmp, :time)";
QString INSERT_sql_PW    = "INSERT INTO PWater (id, duration, ecoroin, tmproin, ecorores, ecoropw, tmpropw, ecoedi, tmpedi, time) VALUES (:id, :duration, :ecoroin, :tmproin, :ecorores, :ecoropw, :tmpropw, :ecoedi, :tmpedi, :time)";
QString INSERT_sql_Log   = "INSERT INTO Log (id, name, action, info, time) VALUES (:id, :name, :action, :info, :time)";


QString select_sql_Handler    = "SELECT id, name, address, type, def FROM Handler";
QString delete_sql_Handler    = "delete from Handler";
QString INSERT_sql_Handler    = "INSERT INTO Handler (id, name, address, type, def) VALUES (:id, :name, :address, :type, :def)";


QString select_sql_Rfid    = "SELECT id, name, address, type FROM Rfid";
QString delete_sql_Rfid    = "delete from Rfid";
QString INSERT_sql_Rfid    = "INSERT INTO Rfid (id, name, address, type) VALUES (:id, :name, :address, :type)";

QString select_sql_User    = "SELECT id , Name , PassWord , Permission FROM User";

//ex
QString update_sql_Water = "update Water SET quality = ?, time = ? where ecoid = ? and time > ? and time < ?";

//consumable
QString select_sql_Consumable = "select LotNo from Consumable  where iPackType = ?";
QString insert_sql_Consumable = "insert into Consumable (id, iPackType, CatNo, LotNo, category, time) values(:id, :iPackType, :CatNo, :LotNo, :category, :time)";
QString update_sql_Consumable = "update Consumable set CatNo = ?, LotNo = ?, time = ? where iPackType = ?";
//end

User *gpActiveU;
QLinkedList<User *>gUserList;



int help_aton(const char *cp)
{
  int           val;
  unsigned char base;
            char c;

  c = *cp;
  /*
   * Collect number up to ``.''.
   * Values are specified as for C:
   * 0x=hex, 0=octal, 1-9=decimal.
   */
  if (!isdigit(c))
    return (0);
  val = 0;
  base = 10;
  if (c == '0') {
    c = *++cp;
    if (c == 'x' || c == 'X') {
      base = 16;
      c = *++cp;
    } else
      base = 8;
  }
  for (;;) {
    if (isdigit(c)) {
      val = (val * base) + (int)(c - '0');
      c = *++cp;
    } else if (base == 16 && isxdigit(c)) {
      val = (val << 4) | (int)(c + 10 - (islower(c) ? 'a' : 'A'));
      c = *++cp;
    } else
      break;
  }
  return val;
}


void Write_sys_int(char *sysfilename,int value)
{
    FILE *fp;
    char buf[20];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"w+");

    sprintf(buf,"%d",value);
    
    if (fp)
    {
        fwrite(buf,strlen(buf),1,fp);
        fclose(fp);
    }
    else
    {
        qDebug() << "open " << sysfilename << " fail" <<endl;
    }

}

void Get_sys_int(char *sysfilename, int& value)
{
    FILE *fp;
    char buf[20];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"r+");
    
    if (fp)
    {
        fread(buf,sizeof(buf),1,fp);

        value = help_aton(buf);
        
        fclose(fp);
    }
    else
    {
        qDebug() << "open " << sysfilename << " fail" <<endl;
    }
}

void Write_sys_string(char *sysfilename,char* value)
{
    FILE *fp;
    char buf[20];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"w+");

    sprintf(buf,"%s",value);
    
    if (fp)
    {
        fwrite(buf,strlen(buf),1,fp);
        fclose(fp);
    }
    else
    {
        qDebug() << "open " << sysfilename << " fail" <<endl;
    }

}


void bb_signals(int sigs, void (*f)(int))
{
    int sig_no = 0;
    int bit = 1;

    while (sigs) {
        if (sigs & bit) {
            sigs &= ~bit;
            signal(sig_no, f);
        }
        sig_no++;
        bit <<= 1;
    }
}

void preMainCalbrate(void)
{
    int pid;

    int status;

    if (access("/etc/pointercal", F_OK) == 0)
    {
        return ;
    }

    if (access("/usr/local/tslib/bin/ts_calibrate", F_OK) != 0)
    {
        _exit(EXIT_FAILURE);
    }

    pid = vfork();
    if (pid < 0) {
        /* TODO: log perror? */
        _exit(EXIT_FAILURE);
    }

    if (!pid) {

        char *argv[3];

        bb_signals(0
            | (1 << SIGCHLD)
            | (1 << SIGPIPE)
            | (1 << SIGHUP)
            , SIG_DFL);

        argv[0] = "/usr/local/tslib/bin/ts_calibrate";
        argv[1] = NULL;

        execv(argv[0], argv);

    }

    wait(&status);

}

//2018.10.23 add, delete data two years ago
void DeleteExpiredData()
{
    int iLoop;
    QStringList tableName;
    QSqlQuery query;
    tableName << "Alarm" << "GetW" << "PWater" << "Log"; //Water;

    QDateTime nowDate = QDateTime::currentDateTime();
    QDateTime deadLine = nowDate.addYears(-2);
    QString   strLine = deadLine.toString("yyyy-MM-dd hh:mm:ss");

    for(iLoop = 0; iLoop < 4; iLoop++)
    {
        QString strQuery = QString("Delete from %1 where time < '%2'")
                .arg(tableName.at(iLoop)).arg(strLine);
        bool ret = query.exec(strQuery);
        if(ret)
            qDebug() << QString("Delete from %1 where time < '%2' : success").arg(tableName.at(iLoop)).arg(strLine);
        else
            qDebug() << QString("Delete from %1 where time < '%2' : fail").arg(tableName.at(iLoop)).arg(strLine);
    }

}

int main(int argc, char *argv[])
{

#ifndef QT_WEBKIT
    preMainCalbrate();
#endif

    CtrlApplication a(argc, argv);
    gApp = &a;

    //ex
    Ex_EventFilter eventF;
    a.installEventFilter(&eventF);
    //Register a custom type
    qRegisterMetaType<NetworkData>("NetworkData");
    qRegisterMetaType<NetworkData>("NetworkData&");
    //end

    chdir(dirname(argv[0]));
    
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

#ifndef QT_WEBKIT
    QWSInputMethod *im = new SyszuxIM();
    QWSServer::setCurrentInputMethod(im);
    //QWSServer::setCursorVisible(false);
#endif

    a.installTranslators(gGlobalParam.MiscParam.iLan,true); // chinese

    qDebug() << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    db.setHostName("SHZN");
    db.setDatabaseName("SHZN");
    db.setUserName("SHZN");
    db.setPassword("SHZN");
    db.open();

    QSqlQuery query;
    bool success;
#if 0
    success = query.exec(CREATE_TABLE_Water);
    if(success)
        qDebug()<<"create Water table successed\r\n";
    else
        qDebug()<<"create Water table failed\r\n";
#endif

    success = query.exec(CREATE_TABLE_Handler);
    if(success)
        qDebug()<<"create handler table successed\r\n";
    else
        qDebug()<<"create handler table failed\r\n";

    success = query.exec(CREATE_TABLE_Rfid);
    if(success)
        qDebug()<<"create rfid table successed\r\n";
    else
        qDebug()<<"create rfid table failed\r\n";
    

    success = query.exec(CREATE_TABLE_Alarm);
    if(success)
        qDebug()<<"create Alarm table successed\r\n";
    else
        qDebug()<<"create Alarm table failed\r\n";

    success = query.exec(CREATE_TABLE_User);
    if(success)
        qDebug()<<"create User table successed\r\n";
    else
        qDebug()<<"create User table failed\r\n";

    success = query.exec(CREATE_TABLE_GetW);
    if(success)
        qDebug()<<"create gw table successed\r\n";
    else
        qDebug()<<"create gw table failed\r\n";

    success = query.exec(CREATE_TABLE_Produce_Water);
    if(success)
        qDebug()<<"create pw table successed\r\n";
    else
        qDebug()<<"create pw table failed\r\n";

    success = query.exec(CREATE_TABLE_Log);
    if(success)
        qDebug()<<"create log table successed\r\n";
    else
        qDebug()<<"create log table failed\r\n";

    //2019.1.17 add
    success = query.exec(CREATE_TABLE_Consumable);
    if(success)
        qDebug()<<"create Consumable table successed\r\n";
    else
        qDebug()<<"create Consumable table failed\r\n";

    DeleteExpiredData(); //删除两年前的数据

    MainWindow w;

    w.show();
    
    return a.exec();
 
}
