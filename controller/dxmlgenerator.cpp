#include "dxmlgenerator.h"
#include "ExtraDisplay.h"
#include <QDateTime>
#include <QStringList>
#include <QDebug>

inline const QByteArray fCharStr(float value)
{
    QString str = QString::number(value, 'f', 1);
    return str.toLatin1();
}

inline const QByteArray charStr3(float value)
{
    QString str = QString::number(value, 'f', 3);
    return str.toLatin1();
}

inline const QByteArray iCharStr(int value)
{
    QString str = QString::number(value);
    return str.toLatin1();
}

inline const QByteArray charStr(unsigned int value)
{
    QString str = QString::number(value);
    return str.toLatin1();

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

void DXmlGenerator::appendElement(QList<QStringList> &elementlist, int count, ...)
{
    va_list ap;
    va_start(ap, count);
    QStringList strList;

    for(int i = 0; i < count; ++i)
    {
        char* str = va_arg(ap, char*);
        strList << str;
    }
    va_end(ap);

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
        appendElement(elementsList, 3, "TapCond", "0", fCharStr(data.m_tapWaterInfo.fG25x).data());
        appendElement(elementsList, 3, "TapTemp", "2", fCharStr(data.m_tapWaterInfo.tx).data());
        appendElement(elementsList, 3, "ROFeedCond", "0", fCharStr(data.m_waterQuality[APP_EXE_I1_NO].fG25x).data());
        appendElement(elementsList, 3, "ROFeedTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I1_NO].tx).data());
        appendElement(elementsList, 3, "ROProductCond", "0", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].fG25x).data());
        appendElement(elementsList, 3, "ROProductTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].tx).data());
        appendElement(elementsList, 3, "RORejectionRate", "7", fCharStr(data.m_otherInfo.fRej).data());
        appendElement(elementsList, 3, "ROFeedPressure", "4", fCharStr(data.m_otherInfo.fFeedPressure).data());
        appendElement(elementsList, 3, "ROPressure", "4", fCharStr(data.m_otherInfo.fROPressure).data());
        break;
    case MACHINE_PURIST:
        appendElement(elementsList, 3, "UPIN", "0", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].fG25x).data()); //new
        appendElement(elementsList, 3, "UPIN", "2", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].tx).data());
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
        appendElement(elementsList, 3, "ROProductRate", "5", fCharStr(data.m_flowRateInfo.value[ROProductRate]).data());
        appendElement(elementsList, 3, "RORejectRate", "5", fCharStr(data.m_flowRateInfo.value[RORejectRate]).data());
        appendElement(elementsList, 3, "ROFeedRate", "5", fCharStr(data.m_flowRateInfo.value[ROFeedRate]).data());
        appendElement(elementsList, 3, "TapRate", "5", fCharStr(data.m_flowRateInfo.value[TapRate]).data());
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
        appendElement(elementsList, 3, "EDIProductResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].fG25x).data());
        appendElement(elementsList, 3, "EDIProductTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].tx).data());
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        appendElement(elementsList, 3, "EDIProductRate", "5", fCharStr(data.m_flowRateInfo.value[EDIProductRate]).data());
        appendElement(elementsList, 3, "EDIRejectRate", "5", fCharStr(data.m_flowRateInfo.value[EDIRejectRate]).data());
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_Genie:
        appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I4_NO].fG25x).data());
        appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I4_NO].tx).data());
        appendElement(elementsList, 3, "HPDispRate", "5", fCharStr(data.m_flowRateInfo.value[HPDispRate]).data());
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_EDI:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I4_NO].fG25x).data());
            appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I4_NO].tx).data());
        }
        else
        {
            appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].fG25x).data());
            appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].tx).data());
        }
        appendElement(elementsList, 3, "HPDispRate", "5", fCharStr(data.m_flowRateInfo.value[HPDispRate]).data());
        break;

    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_RO:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].fG25x).data());
            appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I3_NO].tx).data());
        }
        else
        {
            appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].fG25x).data());
            appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].tx).data());
        }
        appendElement(elementsList, 3, "HPDispRate", "5", fCharStr(data.m_flowRateInfo.value[HPDispRate]).data());
        break;
    case MACHINE_ADAPT:
        appendElement(elementsList, 3, "HPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].fG25x).data());
        appendElement(elementsList, 3, "HPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I2_NO].tx).data());
        appendElement(elementsList, 3, "HPDispRate", "5", fCharStr(data.m_flowRateInfo.value[HPDispRate]).data());
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
        appendElement(elementsList, 3, "UPResis", "1", fCharStr(data.m_waterQuality[APP_EXE_I5_NO].fG25x).data());
        appendElement(elementsList, 3, "UPTemp", "2", fCharStr(data.m_waterQuality[APP_EXE_I5_NO].tx).data());
        appendElement(elementsList, 3, "UPDispRate", "5", fCharStr(data.m_flowRateInfo.value[UPDispRate]).data());
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            appendElement(elementsList, 3, "TOC", "3", charStr((uint)data.m_otherInfo.fToc).data());
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
            appendElement(elementsList, 3, "SourceTankPercent", "7", iCharStr(data.m_tankInfo[1].iPercent).data());
            appendElement(elementsList, 3, "SourceTankVolume", "8", fCharStr(data.m_tankInfo[1].fVolume).data());
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
        appendElement(elementsList, 3, "PureTankPercent", "7", iCharStr(data.m_tankInfo[0].iPercent).data());
        appendElement(elementsList, 3, "PureTankVolume", "8", fCharStr(data.m_tankInfo[0].fVolume).data());
        break;
    default:
        break;
    }
}

void DXmlGenerator::createLifeTimeList(QList<QStringList> &elementsList)
{
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
    {
        appendElement(elementsList, 4, "PrePack", charStr(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY]).data(),
                                                  charStr(gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEL]).data(), "L");
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
        appendElement(elementsList, 4, "ACPack", charStr(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY]).data(),
                                                 charStr(gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEL]).data(),
                                                 "L");
        break;
    default:
        break;
    }

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        appendElement(elementsList, 2, "TPack", charStr(gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY]).data());
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
        appendElement(elementsList, 4, "PPack", charStr(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY]).data(),
                                                charStr(gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEL]).data(), "L");
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
        appendElement(elementsList, 4, "UPack", charStr(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY]).data(),
                                                charStr(gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEL]).data(), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        appendElement(elementsList, 4, "ATPack", charStr(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY]).data(),
                                                 charStr(gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEL]).data(), "L");
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        appendElement(elementsList, 4, "HPack", charStr(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY]).data(),
                                                charStr(gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEL]).data(), "L");
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
        appendElement(elementsList, 4, "UV254", charStr(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY]).data(),
                                                charStr(gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEHOUR]).data(), "h");
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
        appendElement(elementsList, 4, "UV185", charStr(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY]).data(),
                                                charStr(gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEHOUR]).data(), "h");
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
        appendElement(elementsList, 4, "TankUV", charStr(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY]).data(),
                                                 charStr(gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEHOUR]).data(), "h");
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
        appendElement(elementsList, 4, "LoopUV", charStr(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY]).data(),
                                                 charStr(gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEHOUR]).data(),
                                                 "h");
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
        appendElement(elementsList, 2, "TankVentFilter", charStr(gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE]).data());
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
        appendElement(elementsList, 2, "RephibioFilter", charStr(gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE]).data());
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
        appendElement(elementsList, 2, "umFinalFilter", charStr(gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE]).data());
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
        appendElement(elementsList, 2, "LoopFilter", charStr(gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE]).data());
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
        appendElement(elementsList, 2, "LoopDI", charStr(gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE]).data());
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
        appendElement(elementsList, 2, "ROClCleaning", charStr(gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY]).data());
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
        appendElement(elementsList, 2, "ROFeedConductivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk).data());
        appendElement(elementsList, 2, "ROFeedTempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk).data());
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
        appendElement(elementsList, 2, "ROConductivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk).data());
        appendElement(elementsList, 2, "ROTempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk).data());
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
        appendElement(elementsList, 2, "EDIResistivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk).data());
        appendElement(elementsList, 2, "EDITempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk).data());
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_RO:
    case MACHINE_UP:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            appendElement(elementsList, 2, "EDIResistivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk).data());
            appendElement(elementsList, 2, "EDITempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk).data());
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
        appendElement(elementsList, 2, "UPResistivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk).data());
        appendElement(elementsList, 2, "UPTempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk).data());
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
        appendElement(elementsList, 2, "HPResistivity", charStr3(ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk).data());
        appendElement(elementsList, 2, "HPTempertature", charStr3(ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk).data());
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
        appendElement(elementsList, 2, "FlowMeter", charStr3(ex_global_Cali.pc[DISP_PC_COFF_S1].fk).data());
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
        appendElement(elementsList, 2, "ROFeedRate", charStr3(ex_global_Cali.pc[DISP_PC_COFF_S2].fk).data());
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
        appendElement(elementsList, 2, "ROProductRate", charStr3(ex_global_Cali.pc[DISP_PC_COFF_S3].fk).data());
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
        appendElement(elementsList, 2, "RORejectRate", charStr3(ex_global_Cali.pc[DISP_PC_COFF_S4].fk).data());
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
        appendElement(elementsList, 2, "PureTankLevel", charStr3(ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk).data());
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
        appendElement(elementsList, 2, "FeedTankLevel", charStr3(ex_global_Cali.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk).data());
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
        appendElement(elementsList, 2, "ROPressure", charStr3(ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk).data());
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
        appendElement(elementsList, 4, "FeedWaterPressure",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP1]).data(), "4");
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
        appendElement(elementsList, 4, "ROPressure",
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP33]).data(), "4");
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
        appendElement(elementsList, 4, "TapFeedConductivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP13]).data(), "0");
        appendElement(elementsList, 6, "TapFeedTemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP19]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP18]).data(), "2");
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
        appendElement(elementsList, 4, "ROConductivity",
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP3]).data(), "0");
        appendElement(elementsList, 6, "ROTemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP21]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP20]).data(), "2");
        appendElement(elementsList, 4, "RORejectionRate",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP2]).data(), "7");
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
        appendElement(elementsList, 4, "EDIResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP4]).data(), "1");
        appendElement(elementsList, 6, "EDITemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP23]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP22]).data(), "2");
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
        appendElement(elementsList, 4, "UPResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP7]).data(), "1");
        appendElement(elementsList, 6, "UPTemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP25]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP24]).data(), "2");
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
        appendElement(elementsList, 4, "HPResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP32]).data(), "1");
        appendElement(elementsList, 4, "RecirWaterResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP31]).data(), "1");
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
        appendElement(elementsList, 6, "TankResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP10]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP12]).data(), "1");
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
        appendElement(elementsList, 6, "TOCTemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP29]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP28]).data(), "2");
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
        appendElement(elementsList, 4, "TOCFeedResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP30]).data(), "2");
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
        appendElement(elementsList, 4, "LoopResistivity",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP17]).data(), "1");
        appendElement(elementsList, 6, "LoopTemperature",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP27]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP26]).data(), "2");
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
        appendElement(elementsList, 6, "PureTankLevel",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP6]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP5]).data(), "7");
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
        appendElement(elementsList, 6, "FeedTankLevel",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP9]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP8]).data(), "7");
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
        appendElement(elementsList, 6, "ROProductRate",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP15]).data(),
                      "Max", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP14]).data(), "5");
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
        appendElement(elementsList, 4, "RORejectRate",
                      "Min", fCharStr(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP16]).data(), "5");
        break;
    default:
        break;
    }

}

