#ifndef DNETWORKCONFIG_H
#define DNETWORKCONFIG_H

typedef struct
{
    float fG25x;
    unsigned short tx;
}WQualityMsg;

typedef struct
{
    int flowValue;
}FlowRateMsg;

typedef struct
{
    int sourceTank;
    int pureTank;
    float workPressure;
}PressureMsg;

struct UploadMsg
{
    unsigned long code;
    WQualityMsg waterQuality[5];
    FlowRateMsg flowRate[4];
    PressureMsg pressure;
    float fToc;
    unsigned short alarmCode;
};

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


#endif // DNETWORKCONFIG_H
