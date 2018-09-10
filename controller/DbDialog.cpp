#include "DbDialog.h"
#include "mainwindow.h"
#include "user.h"
#include <QInputDialog>
#include <QComboBox>
#include <QRect>
#include <QPushButton>
#include<QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QTextCodec>
#include <QCalendarWidget>
#include <QComboBox>
#include <QStringListModel>
#include <QDateTime>

extern QString select_sql_Water ;
extern QString delete_sql_Water ;

extern QString select_sql_Alarm ;
extern QString delete_sql_Alarm ;

extern QString select_sql_GetW  ;
extern QString delete_sql_GetW  ;

extern QString select_sql_PW ;
extern QString delete_sql_PW ;

extern QString select_sql_Log ;
extern QString delete_sql_Log ;

extern QString gastrDbTables[]; 

DbDlg::DbDlg(QWidget *parent) :
    QDialog(parent)
{
    int iLoop;
    setWindowTitle(tr("DB"));

    this->setFixedSize(700,500); 

    m_pTblWidget = new QTabWidget(); 

    QVBoxLayout *layout = new QVBoxLayout();  

    for (iLoop = 0; iLoop < 2; iLoop++)
    {
        /* for query page */
        QWidget *widget = new QWidget();  

        QGridLayout *mainLayout   = new QGridLayout(this);
        QGridLayout *leftLayout   = new QGridLayout();
        QGridLayout *rightLayout  = new QGridLayout();
        QHBoxLayout *bottomLayout = new QHBoxLayout();

        m_pLabel[iLoop]       = new QLabel();
        m_pLabel[iLoop]->setText("Type:");

        m_pCombBox[iLoop] = new QComboBox();

        for (int i = 0 ; i < DB_TBL_NUM; i++)
        {
            m_pCombBox[iLoop]->addItem(gastrDbTables[i]);
        }

        m_pBtnStart[iLoop] = new QPushButton();
        m_pBtnStart[iLoop]->setObjectName(QString::fromUtf8("m_pBtnStart"));
        m_pBtnStart[iLoop]->setText("StartTime:");

        m_pBtnend[iLoop] = new QPushButton();
        m_pBtnend[iLoop]->setObjectName(QString::fromUtf8("end"));
        m_pBtnend[iLoop]->setText("EndTime:");

        m_pBtnQuery[iLoop] = new QPushButton();
        m_pBtnQuery[iLoop]->setObjectName(QString::fromUtf8("query"));
        m_pBtnQuery[iLoop]->setText("Query");

        m_pBtnAction[iLoop] = new QPushButton();
        m_pBtnAction[iLoop]->setObjectName(QString::fromUtf8("action"));
        m_pBtnAction[iLoop]->setText("Action");

        m_pEditsta[iLoop] = new QLineEdit;
        m_pEditsta[iLoop]->setEnabled(false);

        m_pEditend[iLoop] = new QLineEdit;
        m_pEditend[iLoop]->setEnabled(false);

        leftLayout->addWidget(m_pLabel[iLoop],1,0);
        leftLayout->addWidget(m_pCombBox[iLoop],1,1);
        leftLayout->addWidget(m_pBtnStart[iLoop],1,2);
        leftLayout->addWidget(m_pEditsta[iLoop],1,4);
        leftLayout->addWidget(m_pBtnend[iLoop],2,2);
        leftLayout->addWidget(m_pEditend[iLoop],2,4);

        leftLayout->addWidget(m_pBtnQuery[iLoop],3,2);
        leftLayout->addWidget(m_pBtnAction[iLoop],3,4);

        m_pCalSta[iLoop] = new QCalendarWidget();
        m_pCalEnd[iLoop] = new QCalendarWidget();
    
        rightLayout->addWidget(m_pCalSta[iLoop],1,1);
        rightLayout->addWidget(m_pCalEnd[iLoop],1,1);
        m_pCalEnd[iLoop]->hide();
    
        m_pDataList[iLoop]     = new  QListView();
        m_pListmodel[iLoop]    = new QStringListModel(m_pDataList[iLoop]);
        m_pDataList[iLoop]->setModel(m_pListmodel[iLoop]);
    
        bottomLayout->addWidget(m_pDataList[iLoop]);
    
        mainLayout->addLayout(leftLayout,0,0);
        mainLayout->addLayout(rightLayout,0,1);
        mainLayout->addLayout(bottomLayout,1,0,1,2);
        widget->setLayout(mainLayout);     

        switch(iLoop)
        {
        case 0:
            {
                QIcon icon(":/pics/image/battery0.png");  
                m_pTblWidget->addTab(widget, icon, "QUERY");

                m_pBtnAction[iLoop]->hide();

                //connect(m_pCombBox[iLoop],SIGNAL(currentIndexChanged(int)),this,SLOT(QueryDb()));
                connect(m_pBtnStart[iLoop],SIGNAL(clicked()),this,SLOT(QStart()));
                connect(m_pBtnend[iLoop],SIGNAL(clicked()),this,SLOT(QEnd()));
                connect(m_pCalSta[iLoop],SIGNAL(clicked(QDate)),this,SLOT(QStaCal()));
                connect(m_pCalEnd[iLoop],SIGNAL(clicked(QDate)),this,SLOT(QEndCal()));
                connect(m_pBtnQuery[iLoop],SIGNAL(clicked()),this,SLOT(QQuery()));
            }
            break;
        case 1:
            {
                QIcon icon(":/pics/image/battery0.png");  
                m_pTblWidget->addTab(widget, icon, "DELETE");

                //connect(m_pCombBox[iLoop],SIGNAL(currentIndexChanged(int)),this,SLOT(DelDb()));
                connect(m_pBtnStart[iLoop],SIGNAL(clicked()),this,SLOT(DStart()));
                connect(m_pBtnend[iLoop],SIGNAL(clicked()),this,SLOT(DEnd()));
                connect(m_pCalSta[iLoop],SIGNAL(clicked(QDate)),this,SLOT(DStaCal()));
                connect(m_pCalEnd[iLoop],SIGNAL(clicked(QDate)),this,SLOT(DEndCal()));
                connect(m_pBtnQuery[iLoop],SIGNAL(clicked()),this,SLOT(DQuery()));
                connect(m_pBtnAction[iLoop],SIGNAL(clicked()),this,SLOT(DAction()));
            }
            break;
        }

    }

    /* END FOR CONFIG */
    layout->addWidget(m_pTblWidget);  

    /* common */
    m_pBtnExit = new QPushButton();
    m_pBtnExit->setObjectName(QString::fromUtf8("Exit"));
    m_pBtnExit->setText("Exit");
    layout->addWidget(m_pBtnExit);

    this->setLayout(layout);  

    connect(m_pBtnExit,SIGNAL(clicked()),this,SLOT(Exit()));

}

DbDlg::~DbDlg()
{

}

void DbDlg::DelDb()
{
    QSqlQuery query;
    bool bDbResult = false;
    QString DateS;
    QString DateE;

    m_strList[1].clear();
    m_pListmodel[1]->removeRows(0,m_pListmodel[1]->rowCount());
    m_pListmodel[1]->setStringList(m_strList[1]);

    DateS = m_pEditsta[1]->text();
    DateE = m_pEditend[1]->text();

    if(m_pCombBox[1]->currentText() == gastrDbTables[DB_TBL_BEST_WATER_QUALITY])
    {

        query.prepare(delete_sql_Water);
        query.addBindValue(DateS);
        query.addBindValue(DateE);
        bDbResult = query.exec();
        qDebug() << "delete_sql_Water " << bDbResult;
    }
    if(m_pCombBox[1]->currentText() == gastrDbTables[DB_TBL_ALARM])
    {
        query.prepare(delete_sql_Alarm);
        query.addBindValue(DateS);
        query.addBindValue(DateE);
        bDbResult = query.exec();
        qDebug() << "delete_sql_Alarm " << bDbResult;
    }

    if(m_pCombBox[1]->currentText() == gastrDbTables[DB_TBL_TAKE_WATER])
    {
        query.prepare(delete_sql_GetW);
        query.addBindValue(DateS);
        query.addBindValue(DateE);
        bDbResult = query.exec();
        qDebug() << "delete_sql_GetW " << bDbResult;
    }
    
    if(m_pCombBox[1]->currentText() == gastrDbTables[DB_TBL_PRODUCE_WATER])
    {
        query.prepare(delete_sql_PW);
        query.addBindValue(DateS);
        query.addBindValue(DateE);
        bDbResult = query.exec();
        qDebug() << "delete_sql_GetW " << bDbResult;

    }
    if(m_pCombBox[1]->currentText() == gastrDbTables[DB_TBL_LOG])
    {
        query.prepare(delete_sql_Log);
        query.addBindValue(DateS);
        query.addBindValue(DateE);
        bDbResult = query.exec();
        qDebug() << "delete_sql_Log " << bDbResult;

    }
    
}
void DbDlg::QueryDb(int iIdx)
{
    QSqlQuery query;
    QString Date;
    bool bDbResult = false;

    m_strList[iIdx].clear();
    m_pListmodel[iIdx]->removeRows(0,m_pListmodel[iIdx]->rowCount());
    m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);

    if(m_pCombBox[iIdx]->currentText() == gastrDbTables[DB_TBL_BEST_WATER_QUALITY])
    {
        query.prepare(select_sql_Water);
        query.addBindValue(m_pEditsta[iIdx]->text());
        query.addBindValue(m_pEditend[iIdx]->text());
        bDbResult = query.exec();
        qDebug()<<"select_sql_Water "<< bDbResult;
        while (query.next())
        {
            m_strList[iIdx] += query.value(0).toString()+"ecoid:"+query.value(1).toString()+"  quality:"+query.value(2).toString()+"  time:"+query.value(3).toString();;
        }
        m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);
        m_pDataList[iIdx]->setModel(m_pListmodel[iIdx]);
        
    }

    if(m_pCombBox[iIdx]->currentText() == gastrDbTables[DB_TBL_ALARM])
    {
        query.prepare(select_sql_Alarm);
        query.addBindValue(m_pEditsta[iIdx]->text());
        query.addBindValue(m_pEditend[iIdx]->text());
        bDbResult = query.exec();
        qDebug()<<"select_sql_Alarm "<<bDbResult;
        while (query.next())
        {
            m_strList[iIdx] += query.value(0).toString()+ "type:"+query.value(1).toString()+"  status:"+query.value(2).toString()+"  time:"+query.value(3).toString();
        }
        m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);
        m_pDataList[iIdx]->setModel(m_pListmodel[iIdx]);
    }

    if(m_pCombBox[iIdx]->currentText() == gastrDbTables[DB_TBL_TAKE_WATER])
    {
        query.prepare(select_sql_GetW);
        query.addBindValue(m_pEditsta[iIdx]->text());
        query.addBindValue(m_pEditend[iIdx]->text());
        bDbResult = query.exec();
        qDebug()<<"select_sql_GetW : "<< m_pEditsta[iIdx]->text() << " " << m_pEditend[iIdx]->text() << " " << bDbResult;
        while (query.next())
        {
            QString strResult = query.value(0).toString()+ "name:"+query.value(1).toString()+"  quantity:"+query.value(2).toString()+"  quality:"+query.value(3).toString()+"  tmp:"+query.value(4).toString()+"  time:"+query.value(5).toString();        

            qDebug()<<"TW data: "<< strResult;
            
            m_strList[iIdx] += strResult;
        }
        m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);
        m_pDataList[iIdx]->setModel(m_pListmodel[iIdx]);
    }

    if(m_pCombBox[iIdx]->currentText() == gastrDbTables[DB_TBL_PRODUCE_WATER])
    {
        query.prepare(select_sql_PW);
        query.addBindValue(m_pEditsta[iIdx]->text());
        query.addBindValue(m_pEditend[iIdx]->text());
        bDbResult = query.exec();
        qDebug()<<"select_sql_PW : "<< m_pEditsta[iIdx]->text() << " " << m_pEditend[iIdx]->text() << " " << bDbResult;
        while (query.next())
        {
            QString strResult = query.value(0).toString()+ "  time:"+query.value(1).toString()+\
                                "  ecoroin:"+query.value(2).toString()+"  tmproin:"+query.value(3).toString()+\
                                "  ecorores:"+query.value(4).toString()+"  ecoropw:"+query.value(5).toString() +\
                                "  tmpropw:"+query.value(6).toString()+"  ecoedi:"+query.value(7).toString()+\
                                "  tmpedi:"+query.value(8).toString()+ "duration:"+query.value(9).toString();

            qDebug()<<"PW data: "<< strResult;
            
            m_strList[iIdx]  += strResult;
        }
        m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);
        m_pDataList[iIdx]->setModel(m_pListmodel[iIdx]);
    }   

    if(m_pCombBox[iIdx]->currentText() == gastrDbTables[DB_TBL_LOG])
    {
        query.prepare(select_sql_Log);
        query.addBindValue(m_pEditsta[iIdx]->text());
        query.addBindValue(m_pEditend[iIdx]->text());
        bDbResult = query.exec();
        qDebug()<<"select_sql_Log : "<< m_pEditsta[iIdx]->text() << " " << m_pEditend[iIdx]->text() << " " << bDbResult;
        while (query.next())
        {
            QString strResult = query.value(0).toString()+ "name:"+query.value(1).toString()+"  action:"+query.value(2).toString()+"  time:"+query.value(3).toString();;

            qDebug()<<"Log data: "<< strResult;
            
            m_strList[iIdx]  += strResult;
        }
        m_pListmodel[iIdx]->setStringList(m_strList[iIdx]);
        m_pDataList[iIdx]->setModel(m_pListmodel[iIdx]);
    }       
}
void DbDlg::QStart()
{
    m_pCalEnd[0]->hide();
    m_pCalSta[0]->show();


}
void DbDlg::QEnd()
{
    m_pCalSta[0]->hide();
    m_pCalEnd[0]->show();
}

void DbDlg::QStaCal()
{
    QDate Caldate = m_pCalSta[0]->selectedDate();
    QString date=Caldate.toString("yyyy-MM-dd");
    date += " 00:00:00";

    m_pEditsta[0]->setText(date);
}

void DbDlg::QEndCal()
{
    QDate Caldate = m_pCalEnd[0]->selectedDate();
    QString date =Caldate.toString("yyyy-MM-dd");
    date += " 23:59:59";

    m_pEditend[0]->setText(date);
}
void DbDlg::QQuery()
{
    QueryDb(0);

}

void DbDlg::DStart()
{
    m_pCalEnd[1]->hide();
    m_pCalSta[1]->show();

    QSqlQuery query;

    QString dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

}
void DbDlg::DEnd()
{
    m_pCalSta[1]->hide();
    m_pCalEnd[1]->show();
}

void DbDlg::DStaCal()
{
    QDate Caldate = m_pCalSta[1]->selectedDate();
    QString date  =Caldate.toString("yyyy-MM-dd");

    date += " 00:00:00";

    m_pEditsta[1]->setText(date);
}

void DbDlg::DEndCal()
{
    QDate Caldate = m_pCalEnd[1]->selectedDate();
    QString date =Caldate.toString("yyyy-MM-dd");

    date += " 23:59:59";

    m_pEditend[1]->setText(date);
}

void DbDlg::DQuery()
{
    QueryDb(1);
}

void DbDlg::DAction()
{
    DelDb();
}

void DbDlg::Exit()
{
    close();
}




