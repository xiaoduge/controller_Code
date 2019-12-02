#ifndef DFLOWCHARTWIDGET_H
#define DFLOWCHARTWIDGET_H

#include <QWidget>
#include "Display.h"

class DFlowChartWidget : public QWidget
{
    Q_OBJECT
public:
    enum BACKGROUND_IMAGE
    {
        GENIE_G,
        GENIE_G_HPCIR,
        GENIE_E,
        GENIE_E_HPCIR,
        GENIE_U,
        GENIE_U_TOC,
        GENIE_U_HPCIR,
        GENIE_U_TOC_HPCIR,
        GENIE_R,
        GENIE_R_HPCIR,
        GENIE_PURIST,
        GENIE_PURIST_TOC,
        GENIE_A,
        GENIE_A_TOC,
        BACK_IMAGE_NUM
    };
    enum DEVICE_IMAGE
    {
        VALVE_ON,
        VALVE_E3_H1,
        VALVE_E3_H2,
        VALVE_E3_H3,
        VALVE_E3_H4,
        VALVE_E3_H5,
        VALVE_E3_V1,
        VALVE_E3_V2,
        PUMP_ON1,
        PUMP_ON2,
        UV_ON_V,
        UV_OFF_V,
        UV_ON_H,
        EDI_ON,
        DEVICE_IMAGE_NUM
    };
    enum DEVICE_STATE
    {
        E1_STATE,
        E2_STATE,
        E3_STATE,
        E4_STATE,
        E5_STATE,
        E6_STATE,
        E7_STATE,
        E8_STATE,
        E9_STATE,
        E10_STATE,
        C1_STATE,
        C2_STATE,
        C3_STATE,
        C4_STATE,
        N1_STATE,
        N2_STATE,
        N3_STATE,
        EDI_STATE,
        DEVICE_STATE_NUM
    };

    enum WATER_QUALITY
    {
        I1_VALUE,
        I2_VALUE,
        I3_VALUE,
        I4_VALUE,
        I5_VALUE,
        I_NUM
    };
    enum FLOW_RATE
    {
        S1_VALUE,
        S2_VALUE,
        S3_VALUE,
        S4_VALUE,
        S_NUM
    };

    enum PRESSURE_VALUE
    {
        SOURCE_TANK_VALUE,
        PURE_TANK_VALUE,
        WORK_PRESSURE_VALUE,
        PRESSURE_NUM
    };

    enum I_V_DETECTION
    {
        C1_DETECTION,
        C2_DETECTION,
        C3_DETECTION,
        C4_DETECTION,
        N1_DETECTION,
        N2_DETECTION,
        N3_DETECTION,
        EDI_DETECTION,
        DETECTION_NUM
    };
    enum UNIT_DISPLAY
    {
        RES_COND_UNIT,
        TEMP_UNIT,
        PRESSURE_UNIT,
        UNIT_NUM
    };

public:
    struct WaterInfo
    {
        float fQuality;
        float fTemp;
    };
    struct Detection_Para
    {
        int iValueI;
        int iValueV;
    };

public:
    explicit DFlowChartWidget(QWidget *parent = 0);

    void updateSwitchInfo();
    void updateRpumpInfo(int iChl);
    void updateGpumpInfo(int iChl);
    void updateRectInfo(int iChl);
    void updateEdiInfo(int iChl);

    void updTank(int iIndex,float fVolume);
    void updEcoInfo(int iIndex, ECO_INFO_STRU *info);
    void updPressure(int iIndex,float fvalue);
    void updFlowInfo(int iIndex,int Value);
    void updSourceTank(float fvalue);
    void updTOC(float fToc);

    void setTitleText(const QString& text);
    void setInfo1(const QString& text);
    void setInfo2(const QString& text);
    void setInfo3(const QString& text);

    void updateUnits();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    
private:
    void initUI();
    void initDefaultValue();
    void loadPixmap();


    void paintGenie_G(QPainter &painter);
    void paintGenie_G_HPCIR(QPainter &painter);
    void paintGenie_G_Tags(QPainter &painter);
    void painterGenie_G_Value(QPainter &painter);
    void paintGenie_G_TankLevel(QPainter &painter);


    void paintGenie_E(QPainter &painter);
    void paintGenie_E_HPCIR(QPainter &painter);
    void paintGenie_E_Tags(QPainter &painter);
    void painterGenie_E_Value(QPainter &painter,  bool isHPCir = false);
    void paintGenie_E_TankLevel(QPainter &painter);

    void paintGenie_U(QPainter &painter);
    void paintGenie_U_TOC(QPainter &painter);
    void paintGenie_U_HPCIR(QPainter &painter);
    void paintGenie_U_TOC_HPCIR(QPainter &painter);
    void paintGenie_U_Tags(QPainter &painter, bool isHaveTOC = false);
    void painterGenie_U_Value(QPainter &painter, bool isHPCir = false, bool isHaveTOC = false);
    void paintGenie_U_TankLevel(QPainter &painter);

    void paintGenie_R(QPainter &painter);
    void paintGenie_R_HPCIR(QPainter &painter);
    void paintGenie_R_Tags(QPainter &painter);
    void painterGenie_R_Value(QPainter &painter,  bool isHPCir = false);
    void paintGenie_R_TankLevel(QPainter &painter);

    void paintGenie_PURIST(QPainter &painter);
    void paintGenie_PURIST_TOC(QPainter &painter);
    void paintGenie_PURIST_Tags(QPainter &painter, bool isHaveTOC = false);
    void painterGenie_PURIST_Value(QPainter &painter, bool isHaveTOC = false);

    void paintGenie_A(QPainter &painter);
    void paintGenie_A_TOC(QPainter &painter);
    void paintGenie_A_Tags(QPainter &painter, bool isHaveTOC = false);
    void painterGenie_A_Value(QPainter &painter, bool isHaveTOC = false);

    void paintWorkStatus(QPainter &painter);

signals:

public slots:

private:
    QPixmap m_backgroundPix[BACK_IMAGE_NUM];
    QPixmap m_devicePix[DEVICE_IMAGE_NUM];

    bool m_deviceState[DEVICE_STATE_NUM];
    int m_updateTimerID;
    bool m_isUpdate;
    int m_updPump;
    
    QString m_strWorkStatus[4];

    int   m_iTankLevel;
    float m_fTankLevel;

    WaterInfo    m_waterInfo[I_NUM];
    Detection_Para m_detectionPara[DETECTION_NUM];
    float m_flowRate[S_NUM];
    float m_fWorkPressure;
    float m_fResidue;
    float m_fToc;

    QString m_strUnit[UNIT_NUM];
};

#endif // EX_FLOWCHARTWIDGET_H
