#ifndef MAINPAGE_H
#define MAINPAGE_H


#include "page.h"
#include "Display.h"

class MainWindow;
class navigatorBar;
class CBitmapButton;
class DPushButton;

enum BTN_NAME
{
    BTN_NAVI_UP_INC_VOLAGE = 0, 
    BTN_NAVI_UP_DEC_VOLTAGE, 
    BTN_NAVI_UP_DEC_VOLUME, 
    BTN_NAVI_UP_INC_VOLUME,
    
    BTN_NAVI_EDI_INC_VOLAGE, 
    BTN_NAVI_EDI_DEC_VOLTAGE, 
    BTN_NAVI_EDI_DEC_VOLUME, 
    BTN_NAVI_EDI_INC_VOLUME,
    
    BTN_CIRCULATION_UP, 
    BTN_CIRCULATION_GENERAL, 

    BTN_NAVI_UP_TW, 
    BTN_NAVI_EDI_TW, 
    
    BTN_ALARM_NOTIFY, 
    
    BTN_NUMBER
};

enum LABEL_NAME
{
    LABEL_NAVI_UP_WQ_VALUE = 0,
    LABEL_NAVI_UP_WQ_UNIT, 
    LABEL_NAVI_UP_TOC_LBL, 
    LABEL_NAVI_UP_TOC_VALUE, 
    LABEL_NAVI_UP_TOC_UNIT,
    LABEL_NAVI_UP_TEMP_VALUE, 
    LABEL_NAVI_UP_TEMP_UNIT, 
    LABEL_NAVI_UP_LOGO, 
    LABEL_NAVI_UP_STATE,
    
    LABEL_NAVI_EDI_WQ_VALUE, 
    LABEL_NAVI_EDI_WQ_UNIT, 
    LABEL_NAVI_EDI_LOOP_LBL, 
    LABEL_NAVI_EDI_LOOP_VALUE, 
    LABEL_NAVI_EDI_LOOP_UNIT,
    LABEL_NAVI_EDI_TEMP_VALUE, 
    LABEL_NAVI_EDI_TEMP_UNIT, 
    LABEL_NAVI_EDI_LOGO, 
    LABEL_NAVI_EDI_STATE,
    
    LABEL_NAVI_WT_VOL_VALUE, 
    LABEL_NAVI_WT_VOL_UNIT, 
    LABEL_NAVI_WT_WQ_VALUE, 
    LABEL_NAVI_WT_WQ_UNIT, 
    LABEL_NAVI_TANK_STATE,

    LABEL_NAVI_UPWQUANTITY_VALUE, 
    LABEL_NAVI_UPWQUANTITY_UNIT, 

    LABEL_NAVI_EDIWQUANTITY_VALUE, 
    LABEL_NAVI_EDIWQUANTITY_UNIT, 

    LABEL_TANK_CALIBRATION,

    LABEL_TANK_VALUE,

    LABEL_UP_SPEED,

    LABEL_EDI_SPEED,

    LABEL_NUMBER
};

enum MAINPAGE_NOTIFY_STATE_ENUM
{
    MAINPAGE_NOTIFY_STATE_NONE  = 0,
    MAINPAGE_NOTIFY_STATE_ALARM = APP_PACKET_HO_ALARM_TYPE_ALM,
    MAINPAGE_NOTIFY_STATE_NOT   = APP_PACKET_HO_ALARM_TYPE_NOT
};

class MainPage : public CPage
{
    Q_OBJECT
public:
    MainPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow *wndMain=0 );

    virtual void creatTitle();

    virtual  void update();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    void    updTankState(int iLevel);
    void    updEcoInfo(int iIndex,ECO_INFO_STRU *info,bool bForceUpd = false);
    
    void    updIsInfo(int iIndex,ECO_INFO_STRU *info);
    
    void    updTank(int iIndex,float fVolume);

    void    updQtwState(int iType,bool bStart);
    
    void    updateRunInfo(bool bRun);
    
    void    updAlarmState(int iType);
    
    void    updSpeed(int itype,int iLevel);
    
    void    updDecPressureState(int itype,int iAction);
    
    void    updMainpageState(void);
    
    void    updOtherState(void);

    void    updToc(float fToc);

    void    InitNames(void);

    void  updRealTimeQtwVolume(unsigned int uIValue);

    void  showLogoutBtn(bool bShow);

private:
    void buildTitles();

    void DrawTank(int index);
    void DrawSpeed(int index , int type);

    void SetSpeed(int iType,int index);
    
    void updQtwInfo(int iType,float fValue);
    void startTimer(bool bStart);
    void changeWaterQuantity(int iType,bool bAdd,float fValue);

    CBitmapButton *m_pBtns[BTN_NUMBER];
    
    QLabel        *m_pLabels[LABEL_NUMBER];
    
    QPixmap       *m_pBmpTankWaterLevels[TANK_WATER_LEVEL_SCALE_NUM]; 
    QPixmap       *m_pBmpPumpSpeeds[PUMP_SPEED_NUM];
    
    navigatorBar *m_pNaviBar;
    
    QLabel        *m_pLbPageId[3];



    int           m_iVolumeBtnId;
    QTimer        *m_timeTimer;

    int           m_iTimerCnt;
    
    float         m_afWQuantity[APP_DEV_HS_SUB_NUM];
    bool          m_abQtwFlag[APP_DEV_HS_SUB_NUM];

    int           m_iNotState;

    float         m_fToc;

    bool          m_bSingleMachine;

    int           m_aiLblMap[LABEL_NUMBER];
    int           m_aiBtnMap[BTN_NUMBER];

    QString       m_aWaterType[MACHINE_NUM][2]; /* EDI / UP / RO */
    QString       m_aWaterTLD[MACHINE_NUM][2];  /* TOC / LOOP / DIST */
    
    QString       m_aWaterUnit[MACHINE_NUM][2];  /* ppb/ omg */

    ECO_INFO_STRU m_aHistoryEco[APP_EXE_ECO_NUM];

    int           m_abDecFlag[APP_DEV_HS_SUB_NUM];

    DPushButton   *m_pLogoutBtn;

public slots:
    void on_btn_clicked(int tmp);
    void on_navi_clicked(int tmp);
    void on_btn_clicking(int index);
    void on_timerEvent();

    void on_logoutBtn_clicked();
};

#endif // RUNPAGE_H
