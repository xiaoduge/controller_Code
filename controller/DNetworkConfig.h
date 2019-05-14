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


#endif // DNETWORKCONFIG_H
