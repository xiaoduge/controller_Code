/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-3
** @version   v0.0.1
** @brief     Interface for xml bytearray generator
**
** 用于将特定格式数据打包成XML QByteArray
****************************************************************************/

#ifndef DXMLGENERATOR_H
#define DXMLGENERATOR_H

#include <QObject>
#include <QXmlStreamWriter>
#include "DNetworkConfig.h"
#include "Display.h"

class DXmlGenerator : public QObject
{
    Q_OBJECT
public:
    explicit DXmlGenerator(QObject *parent = 0);

    const QByteArray generator(const DNetworkData &data);
    const QByteArray generator(QList<DNetworkAlaramInfo> &data);
    const QByteArray generator(const DDispenseData &data);

private:   
    void createHeartData(QXmlStreamWriter &xmlWriter, const DNetworkData& data);
    void createAlarmAndConfig(QXmlStreamWriter &xmlWriter, QList<DNetworkAlaramInfo>& data);
    void createAlarm(QXmlStreamWriter &xmlWriter, QList<DNetworkAlaramInfo> &data);
    void createDispenseData(QXmlStreamWriter &xmlWriter, const DDispenseData &data);

    void createActiveUpload(QXmlStreamWriter &xmlWriter);
    void createLifetime(QXmlStreamWriter &xmlWriter);
    void createCalibration(QXmlStreamWriter &xmlWriter);
    void createAlarmPoint(QXmlStreamWriter &xmlWriter);
    void createSystemConfig(QXmlStreamWriter &xmlWriter);
    void getTankInfo(int index, int &v,float& h);

    void appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2);
    void appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3);//3
    void appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3, const QString &s4);//4
    void appendElement(QList<QStringList> &elementlist, const QString &s1, const QString &s2, const QString &s3, const QString &s4, const QString &s5, const QString &s6);//6

    void writeHeartElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist);
    void writeLifeTimeElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist);
    void writeCalibrationElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist);
    void writeAlarmPointElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist);
    void writeDispenseElements(QXmlStreamWriter &xmlWriter, const QList<QStringList> &elementlist);

    void initMachineInfo();

    void createHeartDataList(QList<QStringList> &elementsList, const DNetworkData& data);
    void createLifeTimeList(QList<QStringList> &elementsList);
    void createCalibrationList(QList<QStringList> &elementsList);
    void createAlarmPointList(QList<QStringList> &elementsList);
    void createDispenseDataList(QList<QStringList> &elementsList, const DDispenseData& data);

private:
    QString m_machineInfo;
    
};

#endif // DXMLGENERATOR_H
