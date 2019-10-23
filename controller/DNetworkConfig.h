#ifndef DNETWORKCONFIG_H
#define DNETWORKCONFIG_H

#include <QMetaType>
#include <QDateTime>
#include "datatype.h"
#include "cminterface.h"

enum XmlGeneratorCode
{
    XmlHeartData = 0,
    XmlAlarmData,
    XmlActiveData,
    XmlGeneratorCode_Num
};

enum FlowRateCate
{
    ROProductRate = 0,
    RORejectRate,
    ROFeedRate,
    TapRate,
    EDIProductRate,
    EDIRejectRate,
    HPDispRate,
    UPDispRate,
    FlowRateNum
};

struct DNetworkData
{
    struct WaterQuality
    {
        float fG25x;
        float tx;
    }m_waterQuality[APP_EXE_ECO_NUM];

    WaterQuality m_tapWaterInfo;

    struct FlowRateInfo
    {
        float value[FlowRateNum];
    }m_flowRateInfo;

    struct TankInfo
    {
        int iPercent;
        float fVolume;
    }m_tankInfo[2];

    struct OtherInfo
    {
        float fToc;
        float fRej;
        float fFeedPressure;
        float fROPressure;
    }m_otherInfo;

    explicit DNetworkData()
    {
        clear();
    }

    void clear()
    {
        for(int i = 0; i < APP_EXE_ECO_NUM; ++i)
        {
            m_waterQuality[i].fG25x = 0;
            m_waterQuality[i].tx = 0;
        }
        m_tapWaterInfo.fG25x = 0;
        m_tapWaterInfo.tx = 0;

        for(int i = 0; i < FlowRateNum; ++i)
        {
            m_flowRateInfo.value[i] = 0;
        }

        for(int i = 0; i < 2; ++i)
        {
            m_tankInfo[i].iPercent = 0;
            m_tankInfo[i].fVolume = 0;
        }
        m_otherInfo.fFeedPressure = 0;
        m_otherInfo.fRej = 0;
        m_otherInfo.fROPressure = 0;
        m_otherInfo.fToc = 0;
    }
};

struct DNetworkAlaramInfo
{
    uint m_alarmType;
    uint m_alarmContent;
    uint m_alarmStatus;
    QDateTime m_triggerTime;
};

Q_DECLARE_METATYPE(DNetworkData)
Q_DECLARE_METATYPE(DNetworkAlaramInfo)

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
