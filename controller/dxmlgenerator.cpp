#include "dxmlgenerator.h"
#include "ExtraDisplay.h"
#include <QDateTime>
#include <QStringList>
#include <QDebug>

inline const QString toString1(float value)
{
    return QString::number(value, 'f', 1);
}

inline const QString toString3(float value)
{
    return QString::number(value, 'f', 3);
}

inline const QString toString(int value)
{
    return QString::number(value);
}

inline const QString toString(uint value)
{
    return QString::number(value);
}

DXmlGenerator::DXmlGenerator(QObject *parent) :
    QObject(parent)
{
    initMachineInfo();
}

const QByteArray DXmlGenerator::generator(const DNetworkData& data)
{
    QByteArray xmlArray;
    QXmlStreamWriter xmlWriter(&xmlArray);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("UploadData");

    xmlWriter.writeAttribute("Serial", ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);
    xmlWriter.writeAttribute("MachineType", m_machineInfo);

    createHeartData(xmlWriter, data);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return xmlArray;
}

const QByteArray DXmlGenerator::generator(QList<DNetworkAlaramInfo> &data)
{
    QByteArray xmlArray;
    QXmlStreamWriter xmlWriter(&xmlArray);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("UploadData");

    xmlWriter.writeAttribute("Serial", ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);
    xmlWriter.writeAttribute("MachineType", m_machineInfo);

    createAlarmAndConfig(xmlWriter, data);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return xmlArray;
}

void DXmlGenerator::createHeartData(QXmlStreamWriter &xmlWriter, const DNetworkData& data)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    xmlWriter.writeStartElement("heartData");
    xmlWriter.writeAttribute("Time", strDateTime);

    QList<QStringList> elementsList;

    createHeartDataList(elementsList, data);

    writeHeartElements(xmlWriter, elementsList);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createAlarmAndConfig(QXmlStreamWriter &xmlWriter, QList<DNetworkAlaramInfo>& data)
{
    xmlWriter.writeStartElement("AlarmUpload");

    createAlarm(xmlWriter, data);
    createLifetime(xmlWriter);
    createCalibration(xmlWriter);
    createAlarmPoint(xmlWriter);
    createSystemConfig(xmlWriter);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createAlarm(QXmlStreamWriter &xmlWriter, QList<DNetworkAlaramInfo>& data)
{
    QList<DNetworkAlaramInfo>::const_iterator iter = data.begin();
    for(; iter != data.end(); ++iter)
    {
        DNetworkAlaramInfo alarmInfo = *iter;
        QString strTime = alarmInfo.m_triggerTime.toString("yyyy-MM-dd hh:mm:ss");

        xmlWriter.writeStartElement("Alarm");
        xmlWriter.writeAttribute("Time", strTime);
        xmlWriter.writeTextElement("AlarmType", QString::number(alarmInfo.m_alarmType));
        xmlWriter.writeTextElement("AlarmContent", QString::number(alarmInfo.m_alarmContent));
        xmlWriter.writeTextElement("AlarmStatus", QString::number(alarmInfo.m_alarmStatus));
        xmlWriter.writeEndElement();
    }
}

void DXmlGenerator::createActiveUpload(QXmlStreamWriter &xmlWriter)
{
    xmlWriter.writeStartElement("ActiveUpload");

//    createHeartData(xmlWriter);
    createLifetime(xmlWriter);
    createCalibration(xmlWriter);
    createAlarmPoint(xmlWriter);
    createSystemConfig(xmlWriter);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createLifetime(QXmlStreamWriter &xmlWriter)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    xmlWriter.writeStartElement("LifeTime");
    xmlWriter.writeAttribute("Time", strDateTime);

    QList<QStringList> elementsList;
    createLifeTimeList(elementsList);

    writeLifeTimeElements(xmlWriter, elementsList);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createCalibration(QXmlStreamWriter &xmlWriter)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    xmlWriter.writeStartElement("Calibration");
    xmlWriter.writeAttribute("Time", strDateTime);

    QList<QStringList> elementsList;

    createCalibrationList(elementsList);

    writeCalibrationElements(xmlWriter, elementsList);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createAlarmPoint(QXmlStreamWriter &xmlWriter)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    xmlWriter.writeStartElement("AlarmPoint");
    xmlWriter.writeAttribute("Time", strDateTime);

    QList<QStringList> elementsList;
    createAlarmPointList(elementsList);

    writeAlarmPointElements(xmlWriter, elementsList);

    xmlWriter.writeEndElement();
}

void DXmlGenerator::createSystemConfig(QXmlStreamWriter &xmlWriter)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    xmlWriter.writeStartElement("SystemConfig");
    xmlWriter.writeAttribute("Time", strDateTime);

    int iVol = 0;
    float fHeight = 0.0;
    getTankInfo(DISP_PM_PM2, iVol, fHeight);

    xmlWriter.writeStartElement("PureTank");
    xmlWriter.writeTextElement("Volume", QString::number(iVol));
    xmlWriter.writeTextElement("Height", QString::number(fHeight,'f',2));
    xmlWriter.writeEndElement();

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if(DISP_WATER_BARREL_TYPE_NO != gGlobalParam.PmParam.aiBuckType[DISP_PM_PM3])
        {
            getTankInfo(DISP_PM_PM3, iVol, fHeight);
            xmlWriter.writeStartElement("FeedWaterTank");
            xmlWriter.writeTextElement("Volume", QString::number(iVol));
            xmlWriter.writeTextElement("Height", QString::number(fHeight,'f',2));
            xmlWriter.writeEndElement();
        }
        break;
    }
    default:
        break;
    }

    xmlWriter.writeEndElement();
}

void DXmlGenerator::getTankInfo(int index, int &v, float &h)
{
    int iTank = gGlobalParam.PmParam.aiBuckType[index];

    switch(iTank)
    {
    case DISP_WATER_BARREL_TYPE_030L:
        v = 30;
        h = 30.0;
        break;
    case DISP_WATER_BARREL_TYPE_060L:
        v = 60;
        h = 60.0;
        break;
    case DISP_WATER_BARREL_TYPE_100L:
        v = 100;
        h = 100.0;
        break;
    case DISP_WATER_BARREL_TYPE_200L:
        v = 200;
        h = 100.0;
        break;
    case DISP_WATER_BARREL_TYPE_350L:
        v = 350;
        h = 100.0;
        break;
    case DISP_WATER_BARREL_TYPE_UDF:
        v = gGlobalParam.PmParam.afCap[DISP_PM_PM2];
        h = gGlobalParam.PmParam.afDepth[DISP_PM_PM2];
        break;
    case DISP_WATER_BARREL_TYPE_NO:
        v = 0;
        h = 0;
        break;
    default:
        v = 0;
        h = 0;
        break;
    }
}

void DXmlGenerator::appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2)
{
    QStringList strList;
    strList << s1 << s2;
    elementlist << strList;
}

void DXmlGenerator::appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3)
{
    QStringList strList;
    strList << s1 << s2 << s3;
    elementlist << strList;
}

void DXmlGenerator::appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3, const QString &s4)
{
    QStringList strList;
    strList << s1 << s2 << s3 << s4;
    elementlist << strList;
}

void DXmlGenerator::appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3, const QString &s4, const QString &s5, const QString &s6)
{
    QStringList strList;
    strList << s1 << s2 << s3 << s4 << s5 << s6;
    elementlist << strList;
}

void DXmlGenerator::writeHeartElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist)
{
    QList<QStringList>::const_iterator iter = elementlist.begin();
    for(;iter != elementlist.end(); ++iter)
    {
        QStringList strList = *iter;
        QString elementName = strList[0];
        QString attriValue = strList[1];
        QString elementValue = strList[2];

        xmlWriter.writeStartElement(elementName);

        xmlWriter.writeAttribute("unit", attriValue);
        xmlWriter.writeCharacters(elementValue);
        xmlWriter.writeEndElement();
    }
}
void DXmlGenerator::writeLifeTimeElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist)
{
    QList<QStringList>::const_iterator iter = elementlist.begin();
    for(;iter != elementlist.end(); ++iter)
    {
        QStringList strList = *iter;
        int listSize = strList.size();

        switch (listSize)
        {
        case 2:
        {
            QString elementName = strList[0];
            QString value1 = strList[1];

            xmlWriter.writeStartElement(elementName);

            xmlWriter.writeStartElement("value1");
            xmlWriter.writeCharacters(value1);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            break;
        }
        case 4:
        {
            QString elementName = strList[0];
            QString value1 = strList[1];
            QString value2 = strList[2];
            QString valueUint = strList[3];

            xmlWriter.writeStartElement(elementName);

            xmlWriter.writeStartElement("value1");
            xmlWriter.writeCharacters(value1);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("value2");
            xmlWriter.writeAttribute("unit", valueUint);
            xmlWriter.writeCharacters(value2);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            break;
        }
        default:
            break;
        }
    }
}

void DXmlGenerator::writeCalibrationElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist)
{
    QList<QStringList>::const_iterator iter = elementlist.begin();
    for(;iter != elementlist.end(); ++iter)
    {
        QStringList strList = *iter;
        QString elementName = strList[0];
        QString value = strList[1];

        xmlWriter.writeStartElement(elementName);
        xmlWriter.writeCharacters(value);
        xmlWriter.writeEndElement();
    }
}

void DXmlGenerator::writeAlarmPointElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist)
{
    QList<QStringList>::const_iterator iter = elementlist.begin();
    for(;iter != elementlist.end(); ++iter)
    {
        QStringList strList = *iter;
        int listSize = strList.size();

        switch (listSize)
        {
        case 4:
        {
            QString elementName = strList[0];
            QString valueName = strList[1];
            QString value = strList[2];
            QString strUnit = strList[3];

            xmlWriter.writeStartElement(elementName);

            xmlWriter.writeAttribute("unit", strUnit);
            xmlWriter.writeStartElement(valueName);
            xmlWriter.writeCharacters(value);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            break;
        }
        case 6:
        {
            QString elementName = strList[0];
            QString minValueName = strList[1];
            QString minValue = strList[2];
            QString maxValueName = strList[3];
            QString maxValue = strList[4];
            QString strUint = strList[5];

            xmlWriter.writeStartElement(elementName);
            xmlWriter.writeAttribute("unit", strUint);

            xmlWriter.writeStartElement(minValueName);
            xmlWriter.writeCharacters(minValue);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement(maxValueName);
            xmlWriter.writeCharacters(maxValue);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            break;
        }
        default:
            break;
        }
    }
}

void DXmlGenerator::initMachineInfo()
{
    unsigned int machineFlow = ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow;
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_machineInfo = QString("Super-Genie G %1").arg(machineFlow);
        break;
    case MACHINE_L_UP:
        m_machineInfo = QString("Super-Genie U %1").arg(machineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_machineInfo = QString("Super-Genie E %1").arg(machineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_machineInfo = QString("Super-Genie R %1").arg(machineFlow);
        break;
    case MACHINE_Genie:
        m_machineInfo = QString("Genie G %1").arg(machineFlow);
        break;
    case MACHINE_UP:
        m_machineInfo = QString("Genie U %1").arg(machineFlow);
        break;
    case MACHINE_EDI:
        m_machineInfo = QString("Genie E %1").arg(machineFlow);
        break;
    case MACHINE_RO:
        m_machineInfo = QString("Genie R %1").arg(machineFlow);
        break;
    case MACHINE_PURIST:
        m_machineInfo = QString("Genie PURIST");
        break;
    case MACHINE_ADAPT:
        m_machineInfo = QString("Genie A %1").arg(machineFlow);
        break;
    default:
        m_machineInfo = QString("unknow");
        break;
    }
}

void DXmlGenerator::createHeartDataList(QList<QStringList> &elementsList, const DNetworkData &data)
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "TapCond", "0", toString1(data.m_tapWaterInfo.fG25x));
        appendElement(elementsList, "TapTemp", "2", toString1(data.m_tapWaterInfo.tx));
        appendElement(elementsList, "ROFeedCond", "0", toString1(data.m_waterQuality[APP_EXE_I1_NO].fG25x));
        appendElement(elementsList, "ROFeedTemp", "2", toString1(data.m_waterQuality[APP_EXE_I1_NO].tx));
        appendElement(elementsList, "ROProductCond", "0", toString1(data.m_waterQuality[APP_EXE_I2_NO].fG25x));
        appendElement(elementsList, "ROProductTemp", "2", toString1(data.m_waterQuality[APP_EXE_I2_NO].tx));
        appendElement(elementsList, "RORejectionRate", "7", toString1(data.m_otherInfo.fRej));
        appendElement(elementsList, "ROFeedPressure", "4", toString1(data.m_otherInfo.fFeedPressure));
        appendElement(elementsList, "ROPressure", "4", toString1(data.m_otherInfo.fROPressure));
        break;
    case MACHINE_PURIST:
        appendElement(elementsList, "UPIN", "0", toString1(data.m_waterQuality[APP_EXE_I2_NO].fG25x)); //new
        appendElement(elementsList, "UPIN", "2", toString1(data.m_waterQuality[APP_EXE_I2_NO].tx));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "ROProductRate", "5", toString1(data.m_flowRateInfo.value[ROProductRate]));
        appendElement(elementsList, "RORejectRate", "5", toString1(data.m_flowRateInfo.value[RORejectRate]));
        appendElement(elementsList, "ROFeedRate", "5", toString1(data.m_flowRateInfo.value[ROFeedRate]));
        appendElement(elementsList, "TapRate", "5", toString1(data.m_flowRateInfo.value[TapRate]));
        break;

    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        appendElement(elementsList, "EDIProductResis", "1", toString1(data.m_waterQuality[APP_EXE_I3_NO].fG25x));
        appendElement(elementsList, "EDIProductTemp", "2", toString1(data.m_waterQuality[APP_EXE_I3_NO].tx));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        appendElement(elementsList,  "EDIProductRate", "5", toString1(data.m_flowRateInfo.value[EDIProductRate]));
        appendElement(elementsList,  "EDIRejectRate", "5", toString1(data.m_flowRateInfo.value[EDIRejectRate]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_Genie:
        appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I4_NO].fG25x));
        appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I4_NO].tx));
        appendElement(elementsList, "HPDispRate", "5", toString1(data.m_flowRateInfo.value[HPDispRate]));
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_EDI:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I4_NO].fG25x));
            appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I4_NO].tx));
        }
        else
        {
            appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I3_NO].fG25x));
            appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I3_NO].tx));
        }
        appendElement(elementsList, "HPDispRate", "5", toString1(data.m_flowRateInfo.value[HPDispRate]));
        break;

    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_RO:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I3_NO].fG25x));
            appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I3_NO].tx));
        }
        else
        {
            appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I2_NO].fG25x));
            appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I2_NO].tx));
        }
        appendElement(elementsList, "HPDispRate", "5", toString1(data.m_flowRateInfo.value[HPDispRate]));
        break;
    case MACHINE_ADAPT:
        appendElement(elementsList, "HPResis", "1", toString1(data.m_waterQuality[APP_EXE_I2_NO].fG25x));
        appendElement(elementsList, "HPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I2_NO].tx));
        appendElement(elementsList, "HPDispRate", "5", toString1(data.m_flowRateInfo.value[HPDispRate]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "UPResis", "1", toString1(data.m_waterQuality[APP_EXE_I5_NO].fG25x));
        appendElement(elementsList, "UPTemp", "2", toString1(data.m_waterQuality[APP_EXE_I5_NO].tx));
        appendElement(elementsList, "UPDispRate", "5", toString1(data.m_flowRateInfo.value[UPDispRate]));
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            appendElement(elementsList, "TOC", "3", toString((uint)data.m_otherInfo.fToc));
        }
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
        {
            appendElement(elementsList, "SourceTankPercent", "7", toString(data.m_tankInfo[1].iPercent));
            appendElement(elementsList, "SourceTankVolume", "8", toString1(data.m_tankInfo[1].fVolume));
        }
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        appendElement(elementsList, "PureTankPercent", "7", toString(data.m_tankInfo[0].iPercent));
        appendElement(elementsList, "PureTankVolume", "8", toString1(data.m_tankInfo[0].fVolume));
        break;
    default:
        break;
    }
}

void DXmlGenerator::createLifeTimeList(QList<QStringList> &elementsList)
{
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
    {
        appendElement(elementsList, "PrePack", toString(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY]),
                                               toString(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEL]), "L");
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        appendElement(elementsList, "ACPack", toString(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY]),
                                              toString(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEL]), "L");
        break;
    default:
        break;
    }

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        appendElement(elementsList, "TPack", toString(gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY]));
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "PPack", toString(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY]),
                                             toString(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEL]), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "UPack", toString(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY]),
                                             toString(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEL]), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        appendElement(elementsList, "ATPack", toString(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY]),
                                              toString(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEL]), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        appendElement(elementsList, "HPack", toString(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY]),
                                             toString(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEL]), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        appendElement(elementsList, "UV254", toString(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY]),
                                             toString(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEHOUR]), "h");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "UV185", toString(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY]),
                                             toString(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEHOUR]), "h");
         break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        appendElement(elementsList, "TankUV", toString(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY]),
                                              toString(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEHOUR]), "h");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "LoopUV", toString(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY]),
                                              toString(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEHOUR]), "h");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        appendElement(elementsList, "TankVentFilter", toString(gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        appendElement(elementsList, "RephibioFilter", toString(gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "umFinalFilter", toString(gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "LoopFilter", toString(gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "LoopDI", toString(gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE]));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "ROClCleaning", toString(gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY]));
        break;
    default:
        break;
    }
    //    appendElement(elementsList, 4, "TOCUV", "180", "60000", "h");
}

void DXmlGenerator::createCalibrationList(QList<QStringList> &elementsList)
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "ROFeedConductivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk));
        appendElement(elementsList, "ROFeedTempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "ROConductivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk));
        appendElement(elementsList, "ROTempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        appendElement(elementsList, "EDIResistivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk));
        appendElement(elementsList, "EDITempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk));
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_RO:
    case MACHINE_UP:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, "EDIResistivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk));
            appendElement(elementsList, "EDITempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk));
        }
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "UPResistivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk));
        appendElement(elementsList, "UPTempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "HPResistivity", toString3(ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk));
        appendElement(elementsList, "HPTempertature", toString3(ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "FlowMeter", toString3(ex_global_Cali.pc[DISP_PC_COFF_S1].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "ROFeedRate", toString3(ex_global_Cali.pc[DISP_PC_COFF_S2].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "ROProductRate", toString3(ex_global_Cali.pc[DISP_PC_COFF_S3].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "RORejectRate", toString3(ex_global_Cali.pc[DISP_PC_COFF_S4].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        appendElement(elementsList, "PureTankLevel", toString3(ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "FeedTankLevel", toString3(ex_global_Cali.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk));
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "ROPressure", toString3(ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk));
        break;
    default:
        break;
    }
}

void DXmlGenerator::createAlarmPointList(QList<QStringList> &elementsList)
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "FeedWaterPressure",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP1]), "4");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "ROPressure",
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP33]), "4");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        appendElement(elementsList, "TapFeedConductivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP13]), "0");
        appendElement(elementsList, "TapFeedTemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP19]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP18]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
    case MACHINE_PURIST:
        appendElement(elementsList, "ROConductivity",
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP3]), "0");
        appendElement(elementsList, "ROTemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP21]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP20]), "2");
        appendElement(elementsList, "RORejectionRate",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP2]), "7");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        appendElement(elementsList, "EDIResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP4]), "1");
        appendElement(elementsList, "EDITemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP23]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP22]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "UPResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP7]), "1");
        appendElement(elementsList, "UPTemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP25]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP24]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    default:
        appendElement(elementsList, "HPResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP32]), "1");
        appendElement(elementsList, "RecirWaterResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP31]), "1");
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        appendElement(elementsList, "TankResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP10]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP12]), "1");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "TOCTemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP29]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP28]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        appendElement(elementsList, "TOCFeedResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP30]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "LoopResistivity",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP17]), "1");
        appendElement(elementsList, "LoopTemperature",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP27]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP26]), "2");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        appendElement(elementsList, "PureTankLevel",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP6]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP5]), "7");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "FeedTankLevel",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP9]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP8]), "7");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "ROProductRate",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP15]),
                      "Max", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP14]), "5");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        appendElement(elementsList, "RORejectRate",
                      "Min", toString1(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP16]), "5");
        break;
    default:
        break;
    }

}

