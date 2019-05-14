#ifndef DEVICE_DLG_H
#define DEVICE_DLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include "datatype.h"

#include "cminterface.h"

typedef enum
{
    DEVICE_ITEM_ELEC_ID = 0,
    DEVICE_ITEM_ADDRESS ,
    DEVICE_ITEM_DTYPE ,
    DEVICE_ITEM_VERSION ,
    DEVICE_ITEM_NUM ,

}DEVICE_ITEM_ENUM;

class DeviceItem : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceItem(QWidget *parent, const QString& text0,const QString& text1, const QString& text2)
    {
        int iLoop;

        (void)parent;
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
                m_plbText[iLoop]->setText("unkown");
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
     HANDLER_ITEM_NUM ,
 
 }HANDLER_ITEM_ENUM;
 
 class HandlerItem : public QWidget
 {
     Q_OBJECT
 public:
     explicit HandlerItem(QWidget *parent, const QString& text0,const QString& text1,const QString &text2)
     {
         int iLoop;
         (void)parent;
         m_pMainLayout   =  new QGridLayout(this);
         m_pTopLayout    =  new QHBoxLayout; //QGridLayout
         
         for (iLoop = 0; iLoop < HANDLER_ITEM_NUM - 1; iLoop++)
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
             case HANDLER_ITEM_TYPE:
                 m_plbText[iLoop]->setObjectName(QString::fromUtf8("l2"));
                 m_plbText[iLoop]->setText(text2);
                 break;
             }
             m_pTopLayout->addWidget(m_plbText[iLoop]);
         }

         /* checkbox */
         m_pcbDefault[0] = new QCheckBox;
         
         connect(m_pcbDefault[0], SIGNAL(stateChanged(int)), parent, SLOT(on_checkBox_changeState(int)));
         
         m_pTopLayout->addWidget(m_pcbDefault[0]);
     
         m_pMainLayout->addLayout(m_pTopLayout,0,0);
 
     }
  
    ~HandlerItem() 
     {
         int iLoop;
         for (iLoop = 0; iLoop < HANDLER_ITEM_NUM - 1; iLoop++)
         {
             delete m_plbText[iLoop];
         }
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

     int getCheckState() {return (Qt::Checked == m_pcbDefault[0]->checkState()) ? 1 : 0;}

     void setCheckState(bool bChecked) {m_pcbDefault[0]->setChecked(bChecked);}

     int getAddress() {return m_plbText[HANDLER_ITEM_ADDRESS]->text().toInt();}

     
     int getType() { return QString::compare(m_plbText[HANDLER_ITEM_TYPE]->text(),"UP") == 0 ? APP_DEV_HS_SUB_HYPER : APP_DEV_HS_SUB_REGULAR ;}
 signals:
  
 public slots:
  
 private:
     QLabel *m_plbText[HANDLER_ITEM_NUM-1];
 
     QCheckBox *m_pcbText[1];
 
     QGridLayout *m_pMainLayout;
     QHBoxLayout *m_pTopLayout;
     
 };

 typedef enum
 {
     RFREADER_ITEM_SN = 0,
     RFREADER_ITEM_ADDRESS ,
     RFREADER_ITEM_NUM ,
 
 }RFREADER_ITEM_ENUM;

 class RfReaderItem : public QWidget
 {
     Q_OBJECT
 public:
     explicit RfReaderItem(QWidget *parent, const QString& text0,const QString& text1)
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
 signals:
  
 public slots:
  
 private:
     QLabel *m_plbText[RFREADER_ITEM_NUM];
 
     QGridLayout *m_pMainLayout;
     QHBoxLayout *m_pTopLayout;
     
 };


class DeviceDlg : public QDialog {
    Q_OBJECT
public:
    DeviceDlg(QWidget *parent = 0);
    ~DeviceDlg();
    void addDevice(const QString& text0,const QString& text1, const QString& text2);
    
    void deviceVersion(const QString& addr,const QString& version);
    
    void addHandler(const QString& text0,const QString& text1,const QString& text2);

    void addRfReader(const QString& text0,const QString& text1);

public slots:
    bool eventFilter(QObject *, QEvent *);
private slots:
    void on_pushButton_QueryId();
    void on_pushButton_Cancel();
    void on_pushButton_QueryVersion();
    void on_pushButton_RmvDevice();
    
    void on_pushButton_QueryHandler();
    void on_pushButton_CfgHandler();
    void on_pushButton_ResetHandler();
    void on_pushButton_RmvHandler();
    void on_pushButton_SaveHandler();

    
    void on_checkBox_changeState(int state);

    void on_pushButton_QueryRfReader();
    void on_pushButton_CfgRfReader();
    void on_pushButton_ResetRfReader();
    void on_pushButton_RmvRfReader();
    void on_pushButton_SaveRfReader();

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

private:

	/* for device */
    QPushButton *m_pBtnQueryId;
    QPushButton *m_pBtnQueryVersion;
    QPushButton *m_pBtnRmvDevice;
    QListWidget *m_pListWgtDevice;

    /* for handler */
    QPushButton *m_pBtnQueryHandler;
    QPushButton *m_pBtnCfgHandler;
    QPushButton *m_pBtnResetHandler;
    QPushButton *m_pBtnRmvHandler;
    QPushButton *m_pBtnSaveHandler;
    QListWidget *m_pListWgtHandler;

    /* for RfReader */
    QPushButton *m_pBtnQueryRfReader;
    QPushButton *m_pBtnCfgRfReader;
    QPushButton *m_pBtnResetRfReader;
    QPushButton *m_pBtnRmvRfReader;
    QPushButton *m_pBtnSaveRfReader;
    QListWidget *m_pListWgtRfReader;

	/* common */
    QTabWidget	*m_pTblWidget;
    QWidget     *m_pParent; 
    QPushButton *m_pBtnCanCel;

};
#endif // DEVICE_H
