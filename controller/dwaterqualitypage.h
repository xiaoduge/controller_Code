#ifndef DWATERQUALITYPAGE_H
#define DWATERQUALITYPAGE_H

#include "Display.h"
#include "subpage.h"
#include "dwaterqualitywidget.h"


class DWaterQualityPage : public CSubPage
{
    Q_OBJECT
public:
    struct Show_History_Info
    {
        float value1;
        float value2;
    };
    enum MSG_ENUM
    {
        Tap_Cond,
        RO_Feed_Cond,
        RO_Product,
        RO_Rejection,
        EDI_Product,

        RO_Feed_Pressure,
        RO_Pressure,

        RO_Product_Rate,
        RO_Reject_Rate,
        RO_Feed_Rate,
        Tap_Rate,
        EDI_Product_Rate,
        EDI_Reject_Rate,

        Source_Tank_Level,
        Pure_Tank_Level,
        HP_Resis,
        HP_Disp_Rate,
        UP_IN,
        UP_Resis,
        TOC_Value,
        UP_Disp_Rate,

        MSG_NUM
    };

    enum UNIT_MSG
    {
        UNIT_OMG,
        UNIT_USCM,
        UNIT_CELSIUS,//Celsius
        UNIT_F,
        UNIT_L_H,
        UNIT_L_MIN,
        UNIT_G_H,
        UNIT_G_MIN,
        UNIT_BAR,
        UNIT_MPA,
        UNIT_PSI,
        UNIT_PPB,
        UNIT_PERCENTAGE,//percentage
        UNIT_VOLUME,    //volume
        UNIT_MSG_NUM
    };

public:
    DWaterQualityPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    void updEcoInfo(int iIndex,ECO_INFO_STRU *info);

    void updAllInfo(void);

    void updPressure(int iIndex,float fvalue);

    void updFlowInfo(int iIndex,int Value);

    void updTank(int iIndex,float fVolume);
    void updSourceTank(int iIndex, float fVolume);

    void updSwPressure(float fvalue);

    void updTOC(float fToc); //2018.11.13

private:
    void initAllValue();
    void initTagsArray();
    void initConfigList();
    void updateValue(const DTags& t, const QString& value1, const QString& value2 = "--");

    void updHistoryEcoInfo();
    void updHistoryPressure();
    void updHistoryFlowInfo();
    void updHistoryTank();
    void updHistoryTOC();

    void setBackColor();
    void buildTitles();

private:
    DWaterQualityWidget* m_pQualityWidget;
    QList<DTags> m_cfglist;
    QString strMsg[MSG_NUM];
    QString strUnitMsg[UNIT_MSG_NUM];

    DTags m_tags[MSG_NUM];

    unsigned int    m_aulFlowMeter[APP_FM_FLOW_METER_NUM];
    Show_History_Info m_historyInfo[MSG_NUM];

    bool m_showFirst;
};

#endif // EX_WATERQUALITYPAGE_H
