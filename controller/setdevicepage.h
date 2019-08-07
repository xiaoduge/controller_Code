#ifndef SET_DEVICE_PAGE_H
#define SET_DEVICE_PAGE_H

#include "subpage.h"
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include "datatype.h"
#include "cminterface.h"

class MainWindow;
class QHBoxLayout;
class QListWidgetItem;
class QListWidget;

typedef enum
{
    DEVICE_ITEM_ELEC_ID = 0,
    DEVICE_ITEM_ADDRESS ,
    DEVICE_ITEM_DTYPE ,
    DEVICE_ITEM_VERSION ,
    DEVICE_ITEM_NUM

}DEVICE_ITEM_ENUM;

class DeviceItem : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceItem(CPage *parent, const QString& text0,const QString& text1, const QString& text2)
    {
        Q_UNUSED(parent);
        int iLoop;

        m_pMainLayout   =  new QGridLayout(this);
        m_pTopLayout    =  new QHBoxLayout; //QGridLayout
        
        for (iLoop = 0; iLoop < DEVICE_ITEM_NUM; iLoop++)
        {
            m_plbText[iLoop] = new QLabel(this);
            m_plbText[iLoop]->setAlignment(Qt::AlignCenter);
            switch(iLoop)
            {
            case DEVICE_ITEM_ELEC_ID:
                m_plbText[iLoop]->setObjectName(QString::fromUtf8("l0"));
                m_plbText[iLoop]->setText(text0);
                break;
            case DEVICE_ITEM_ADDRESS:
                m_plbText[iLoop]->setObjectName(QString::fromUtf8("l1"));
                m_plbText[iLoop]->setText(text1);
                break;
            case DEVICE_ITEM_DTYPE:
                m_plbText[iLoop]->setObjectName(QString::fromUtf8("l2"));
                m_plbText[iLoop]->setText(text2);
                break;
            case DEVICE_ITEM_VERSION:
                m_plbText[iLoop]->setObjectName(QString::fromUtf8("l3"));
                m_plbText[iLoop]->setText(tr("unkown"));
                break;
            }
            m_pTopLayout->addWidget(m_plbText[iLoop]);
        }
    
        m_pMainLayout->addLayout(m_pTopLayout,0,0);

    }
 
   ~DeviceItem() 
    {
        int iLoop;
        for (iLoop = 0; iLoop < DEVICE_ITEM_NUM; iLoop++)
        {
            delete m_plbText[iLoop];
        }
        delete m_pTopLayout;
        delete m_pMainLayout;
        
    };
    QString getItemText(int iIndex)
    {
        return m_plbText[iIndex]->text();
    }

    void setItemText(int iIndex,const QString& strInfo)
    {
        m_plbText[iIndex]->setText(strInfo);
    }    
signals:
 
public slots:
 
private:
    QLabel *m_plbText[DEVICE_ITEM_NUM];

    QGridLayout *m_pMainLayout;
    QHBoxLayout *m_pTopLayout;
    
};
 
 typedef enum
 {
     HANDLER_ITEM_SN = 0,
     HANDLER_ITEM_ADDRESS ,
     HANDLER_ITEM_TYPE,
     HANDLER_ITEM_DEFAULT,
     HANDLER_ITEM_NUM
 
 }HANDLER_ITEM_ENUM;

 #define HANDLER_ITEM_TEXT_NUMBER (HANDLER_ITEM_NUM - 2)
 
 class HandlerItem : public QWidget
 {
     Q_OBJECT
 public:
     explicit HandlerItem(CPage *parent, const QString& text0,const QString& text1)
     {
         int iLoop;
         (void)parent;
         m_pMainLayout   =  new QGridLayout(this);
         m_pTopLayout    =  new QHBoxLayout; //QGridLayout
         
         for (iLoop = 0; iLoop < HANDLER_ITEM_TEXT_NUMBER; iLoop++)
         {
             m_plbText[iLoop] = new QLabel(this);
             m_plbText[iLoop]->setAlignment(Qt::AlignCenter);
             switch(iLoop)
             {
             case HANDLER_ITEM_SN:
                 m_plbText[iLoop]->setObjectName(QString::fromUtf8("l0"));
                 m_plbText[iLoop]->setText(text0);
                 break;
             case HANDLER_ITEM_ADDRESS:
                 m_plbText[iLoop]->setObjectName(QString::fromUtf8("l1"));
                 m_plbText[iLoop]->setText(text1);
                 break;
             }
             m_pTopLayout->addWidget(m_plbText[iLoop]);
         }

         /* handler type */
         m_pcombType[0]  = new QComboBox;

         m_pcombType[0]->addItem(tr("UP"));
         m_pcombType[0]->addItem(tr("HP"));
         m_pcombType[0]->setCurrentIndex(0);

         /* checkbox */
         m_pcbDefault[0] = new QCheckBox;

         
         connect(m_pcbDefault[0], SIGNAL(stateChanged(int)), parent, SLOT(on_checkBox_changeState(int)));

         /*button*/
         m_pBtnDeleteHandler = new QPushButton;
         m_pBtnDeleteHandler->setText(tr("Delete"));
         
         connect(m_pBtnDeleteHandler,SIGNAL(clicked()),parent,SLOT(on_pushButton_DeleteHandler()));
         
         m_pTopLayout->addWidget(m_pcbDefault[0]);
     
         m_pTopLayout->addWidget(m_pcombType[0]);

         m_pTopLayout->addWidget(m_pBtnDeleteHandler);
     
         m_pMainLayout->addLayout(m_pTopLayout,0,0);
 
     }
  
    ~HandlerItem() 
     {
         int iLoop;
         for (iLoop = 0; iLoop < HANDLER_ITEM_NUM - 2; iLoop++)
         {
             delete m_plbText[iLoop];
         }
         delete m_pcombType[0];
         delete m_pcbDefault[0];
         delete m_pTopLayout;
         delete m_pMainLayout;
         
     };
     QString getItemText(int iIndex)
     {
         return m_plbText[iIndex]->text();
     }
 
     void setItemText(int iIndex,const QString& strInfo)
     {
         m_plbText[iIndex]->setText(strInfo);
     }  

     int getItemIndex(int iIndex)
     {
         return m_pcombType[iIndex]->currentIndex();
     }
 

     void setItemIndex(int iIndex,int iSel)
     {
         if (iSel < 2) m_pcombType[iIndex]->setCurrentIndex(iSel);
     }

     void setCheckBoxStyle(const QString &strStyle){m_pcbDefault[0]->setStyleSheet(strStyle);}

     int getCheckState() {return (Qt::Checked == m_pcbDefault[0]->checkState()) ? 1 : 0;}

     void setCheckState(bool bChecked) {m_pcbDefault[0]->setChecked(bChecked);}

     int getAddress() {return m_plbText[HANDLER_ITEM_ADDRESS]->text().toInt();}

     
     int getType() { return (getItemIndex(0) == 0 )? APP_DEV_HS_SUB_HYPER : APP_DEV_HS_SUB_REGULAR ;}

     QCheckBox *getCheck(){return m_pcbDefault[0];}
 signals:
  
 public slots:
  
 private:
     QLabel *m_plbText[HANDLER_ITEM_TEXT_NUMBER];
 
     QCheckBox *m_pcbDefault[1];
     QComboBox *m_pcombType[1];
 
     QGridLayout *m_pMainLayout;
     QHBoxLayout *m_pTopLayout;
     QPushButton *m_pBtnDeleteHandler;
     
 };

 typedef enum
 {
     RFREADER_ITEM_SN = 0,
     RFREADER_ITEM_ADDRESS ,
     RFREADER_ITEM_NUM
 
 }RFREADER_ITEM_ENUM;

 class RfReaderItem : public QWidget
 {
     Q_OBJECT
 public:
     explicit RfReaderItem(CPage *parent, const QString& text0,const QString& text1)
     {
         int iLoop;
 
         m_pMainLayout   =  new QGridLayout(this);
         m_pTopLayout    =  new QHBoxLayout; //QGridLayout
         
         for (iLoop = 0; iLoop < RFREADER_ITEM_NUM; iLoop++)
         {
             m_plbText[iLoop] = new QLabel(this);
             m_plbText[iLoop]->setAlignment(Qt::AlignCenter);
             switch(iLoop)
             {
             case RFREADER_ITEM_SN:
                 m_plbText[iLoop]->setObjectName(QString::fromUtf8("l0"));
                 m_plbText[iLoop]->setText(text0);
                 break;
             case RFREADER_ITEM_ADDRESS:
                 m_plbText[iLoop]->setObjectName(QString::fromUtf8("l1"));
                 m_plbText[iLoop]->setText(text1);
                 break;
             }
             m_pTopLayout->addWidget(m_plbText[iLoop]);
         }
     
         m_pMainLayout->addLayout(m_pTopLayout,0,0);
 
     }
  
    ~RfReaderItem() 
     {
         int iLoop;
         for (iLoop = 0; iLoop < RFREADER_ITEM_NUM; iLoop++)
         {
             delete m_plbText[iLoop];
         }
         delete m_pTopLayout;
         delete m_pMainLayout;
         
     };
     QString getItemText(int iIndex)
     {
         return m_plbText[iIndex]->text();
     }
 
     void setItemText(int iIndex,const QString& strInfo)
     {
         m_plbText[iIndex]->setText(strInfo);
     }    

     int getAddress() {return m_plbText[HANDLER_ITEM_ADDRESS]->text().toInt();}
     
 signals:
  
 public slots:
  
 private:
     QLabel *m_plbText[RFREADER_ITEM_NUM];
 
     QGridLayout *m_pMainLayout;
     QHBoxLayout *m_pTopLayout;
     
 };

class SetDevicePage : public CSubPage
{
    Q_OBJECT

public:
    SetDevicePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    void addDevice(const QString& text0,const QString& text1, const QString& text2);
    
    void deviceVersion(const QString& addr,const QString& version);
    
    void addHandler(int iType,const QString& text0,const QString& text1);
    
    void addRfReader(const QString& text0,const QString& text1);

    void zigbeeUpdResult(int iResult,int iPercent);
    
    void cfgHandlerRsp();

private:
    void buildTitles();
    void setBackColor();
    void finishSave();
private:
    QListWidgetItem * searchDeviceByAddress(const QString& info);
    QListWidgetItem * searchDeviceByElecId(const QString& info);
    QListWidgetItem * searchDevice(const QString& info,int iIndex);
    QString getDevice(int iDevIdx,int iInfoIdx);
    
    QListWidgetItem * searchHandler(const QString& info,int iIndex);
    QString getHandler(int iDevIdx,int iInfoIdx);
    QListWidgetItem * searchHandlerBySN(const QString& info);
    QListWidgetItem * searchHandlerByAddress(const QString& info);

    QListWidgetItem * searchRfReader(const QString& info,int iIndex);
    QString getRfReader(int iDevIdx,int iInfoIdx);
    QListWidgetItem * searchRfReaderBySN(const QString& info);
    QListWidgetItem * searchRfReaderByAddress(const QString& info);

	/* for device */
    QPushButton *m_pBtnQueryId;
    QPushButton *m_pBtnQueryVersion;
    QPushButton *m_pBtnRmvDevice;
    QListWidget *m_pListWgtDevice;
    QLabel      *m_plbDeviceSN;
    QLabel      *m_plbDeviceAdr;
    QLabel      *m_plbDeviceType;
    QLabel      *m_plbDeviceVers;

    /* for handler */
    QPushButton *m_pBtnQueryHandler;
    QPushButton *m_pBtnCfgHandler;
    QPushButton *m_pBtnResetHandler;
//    QPushButton *m_pBtnRmvHandler;
    QPushButton *m_pBtnSaveHandler;
    QListWidget *m_pListWgtHandler;
    QComboBox   *m_pcombTrxType;
    QLabel      *m_plbHandlerSN;
    QLabel      *m_plbHandlerAdr;
    QLabel      *m_plbHandlerDef;
    QLabel      *m_plbHandlerType;
    QLabel      *m_plbHandlerOper;

    /* for RfReader */
    QPushButton *m_pBtnQueryRfReader;
    QLabel      *m_plbRfReaderSN;
    QLabel      *m_plbRfReaderAdr;
#ifdef AUTO_CFG_RF_READER    
    QPushButton *m_pBtnCfgRfReader;
    QPushButton *m_pBtnResetRfReader;
#endif
    QPushButton *m_pBtnRmvRfReader;
    QPushButton *m_pBtnSaveRfReader;
    QListWidget *m_pListWgtRfReader;

    /* for Maintenance */
    QPushButton *m_pBtnZigbeeUpd;
    QLabel      *m_plbZigbeeUpd;
    

	/* common */
    QTabWidget	*m_pTblWidget;
    QWidget     *m_widgetLayout; 
    QString     m_strQss4Chk;
public slots:
    void on_btn_clicked(int);
    void on_pushButton_QueryId();
    void on_pushButton_QueryVersion();
    void on_pushButton_RmvDevice();
    
    void on_pushButton_QueryHandler();
    void on_pushButton_CfgHandler();
    void on_pushButton_ResetHandler();
    void on_pushButton_RmvHandler();
    void on_pushButton_SaveHandler();

    
    void on_checkBox_changeState(int state);
    void on_pushButton_DeleteHandler();

    void on_pushButton_QueryRfReader();
#ifdef AUTO_CFG_RF_READER    
    void on_pushButton_CfgRfReader();
    void on_pushButton_ResetRfReader();
#endif
    void on_pushButton_RmvRfReader();
    void on_pushButton_SaveRfReader();
    void on_pushButton_ZigbeeUpd();

    void on_CmbIndexChange_trx_type(int index);
};

#endif // SET_DEVICE_PAGE_H
