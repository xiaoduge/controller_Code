#ifndef DNETWORKCONFIG_H
#define DNETWORKCONFIG_H

#include <QMetaType>

typedef struct
{
    float fG25x;
    float tx;
}WQualityMsg;

typedef struct
{
    int flowValue[4];
}FlowRateMsg;

typedef struct
{
//    int sourceTank;
    int pureTank;
    float workPressure;
}PressureMsg;


struct NetworkData
{
    WQualityMsg waterQuality[5];
    float fResidue;
    float fToc;
    int runStatus;
};

Q_DECLARE_METATYPE(NetworkData)

enum Network_Operating_Data
{
    I1_DATA = 0,
    I2_DATA,
    I3_DATA,
    I4_DATA,
    I5_DATA,

    S1_DATA,
    S2_DATA,
    S3_DATA,
    S4_DATA,

    SOURCE_TANK_DATA,
    PURE_TANK_DATA,
    WORK_PRESSURE_DATA,

    NETWORK_DATA_NUM

};


enum Consumables_Notify  //NOTIFY
{
    DISP_PRE_NOTIFY = 0,   //预处理柱:
    DISP_AC_NOTIFY,        //AC PACK
    DISP_T_NOTIFY,         //T PACK:
    DISP_P_NOTIFY ,        //P Pack:
    DISP_U_NOTIFY ,        //U Pack:
    DISP_H_NOTIFY ,        //H Pack:
    DISP_AT_NOTIFY ,       //At Pack:
    DISP_N1_NOTIFY ,       //  254UV
    DISP_N2_NOTIFY,        // 185UV
    DISP_N3_NOTIFY,        // 水箱UV
    DISP_N4_NOTIFY,        //  管路UV
    DISP_N5_NOTIFY,        //  TOC UV
    DISP_W_NOTIFY,         //水箱空气过滤器寿命
    DISP_T_B_NOTIFY,       //终端过滤器寿命
    DISP_T_A_NOTIFY,       //终端过滤器寿命
    DISP_TUBE_NOTIFY,      //管路过滤器
    DISP_TUBE_DI_NOTIFY,   //管路DI
    DISP_ROC12_NOTIFY,     //RO Cl2清洗

    HTTP_NOTIFY_NUM
};


#endif // DNETWORKCONFIG_H
