#ifndef WATER_QUALITY_PAGE_H
#define WATER_QUALITY_PAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include "Display.h"

typedef enum
{   
    WQ_TYPE_TANK,
    WQ_TYPE_UP,
    WQ_TYPE_EDI,
    WQ_TYPE_NUM,
}WQ_TYPE_ENUM;

typedef enum
{
    UP_WATER,
    UP_TOC,
    UP_PRODUCT_TEMT,
    UP_IN_QUA,
    UP_SPEED,
    UP_NUM,
}UP_MSG_ENUM;

typedef enum
{
    TANK_LEVEL,
    TANK_VALUE,   
    CIR_WATER_QUA,
    CIR_SPEED,
    TANK_NUM
}TANK_MSG_ENUM;

typedef enum
{
    EDI_PRODUCT_QUALITY,
    EDI_PRODUCT_T,
    RO_PRODUCT_QUALITY,
    RO_PRODUCT_T,  
    RO_RESIDUE_RATIO,     
    TAP_WATER_QUALITY,
    RO_WATER_IN_QUALITY, 
    RO_WATER_IN_TEMPT, 
    TAP_WATER_PRESSURE,
    RO_WORK_PRESSURE,
    RO_PRODUCT_WATER_SPEED,
    RO_DISCARD_WATER_SPEED,
    RO_WATER_IN_SPEED,
    TAP_WATER_SPEED,  
    EDI_PROC_WATER_SPEED,
    EDI_DISCARD_WATER_SPEED,
    EDI_NUM,
}EDI_MSG_ENUM;


class MainWindow;


class WaterQualityPage : public CSubPage
{
    Q_OBJECT

public:
    WaterQualityPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    void updEcoInfo(int iIndex,ECO_INFO_STRU *info,bool bForceUpd=FALSE);

    void updAllInfo(void);
    
    void updPressure(int iIndex,float fvalue);
    
    void updFlowInfo(int iIndex,int Value);
    
    void updTank(int iIndex,float fVolume);
    
    void updSwPressure(float fvalue);

    void updTOC(float fToc); //2018.11.13
private:

    void update_edi_msg(int iIdx,float value);

    void update_up_msg(int iIdx,float fValue);

    void update_tank_msg(int iIdx,float value);

    void init_up_msg();

    void init_tank_msg();

    void init_edi_msg();

    void setBackColor();

    void buildTitles();

    QLabel        *m_lbFrame[WQ_TYPE_NUM];

    QTextEdit     *m_edit4Up[UP_NUM];
    QTextEdit     *m_edit4Tank[TANK_NUM];
    QTextEdit     *m_edit4Edi[EDI_NUM];

    QString       upmsg[UP_NUM];
    QString       tankmsg[TANK_NUM];
    QString       edimsg[EDI_NUM];
    QTextCharFormat charFormatName;
    QTextCharFormat charFormatValue;

    struct UTE_ITEM_TYPE {
        int iId;
    }aTsIds[TANK_NUM],aUsIds[UP_NUM],aEsIds[EDI_NUM];

    int aTsIndex[TANK_NUM],aUsIndex[UP_NUM],aEsIndex[EDI_NUM];

    unsigned int    m_aulFlowMeter[APP_FM_FLOW_METER_NUM];

    int             m_aiRealNum[3];

    
    SHOW_INFO_STRU   m_aUpHistoryEco[UP_NUM];
    SHOW_INFO_STRU   m_aTankHistoryEco[TANK_NUM];
    SHOW_INFO_STRU   m_aEDIHistoryEco[EDI_NUM];

public slots:

};
#endif //  WATER_QUALITY_PAGE_H
