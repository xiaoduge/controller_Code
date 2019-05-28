#include "dhttpworker.h"
#include "dnetworkaccessmanager.h"
#include "ExtraDisplay.h"
#include <QMutexLocker>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QTimerEvent>

DHttpWorker::DHttpWorker(QObject *parent) : QObject(parent)
{
    m_idleHeart = true;
    m_idleAlarm = true;
    m_idleRunMsg = true;

    for(int i = 0; i < NETWORK_DATA_NUM; i++)
    {
        strOperatingData[i].clear();
    }
    initJsonFormat();
}

DHttpWorker::~DHttpWorker()
{
    m_networkAccessManager->deleteLater();
}

void DHttpWorker::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerID)
    {
        if(!strAlarmList.empty())
        {
            on_alarmHttpPost();
        }
    }
}

void DHttpWorker::on_updateRunMsgList(const QString &msg, int index)
{
    QMutexLocker locker(&m_mutex);
    updateOperatingData(msg, index);
    on_runMsgHttpPost();
}

void DHttpWorker::on_updateAlarmList(const QString &data)
{
    QMutexLocker locker(&m_mutex);
    strAlarmList.append(data);
    strAlarmTempList = strAlarmList; //backup
}

void DHttpWorker::on_updateHeartList(const NetworkData &data)
{
    QMutexLocker locker(&m_mutex);
    strHeartMsg = m_heartJson.arg(data.waterQuality[0].fG25x)
                             .arg(data.waterQuality[0].tx)
                             .arg(data.waterQuality[1].fG25x)
                             .arg(data.waterQuality[1].tx)
                             .arg(data.waterQuality[2].fG25x)
                             .arg(data.waterQuality[2].tx)
                             .arg(data.waterQuality[3].fG25x)
                             .arg(data.waterQuality[3].tx)
                             .arg(data.waterQuality[4].fG25x)
                             .arg(data.waterQuality[4].tx)
                             .arg(data.fResidue)
                             .arg(data.fToc)
                             .arg(data.runStatus);
    on_heartHttpPost();
}

void DHttpWorker::on_heartHttpPost()
{
    if(!m_idleHeart)
    {
        return;
    }
    QString strSerial = QString("\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);;

    QString strContent  = QString("{");
    strContent += strSerial;
    strContent += strHeartMsg;
    strContent += QString("}");

    qDebug() << strContent;

    QByteArray msgArray = strContent.toLatin1();

    QNetworkRequest request;
    request.setUrl(QUrl("https://s.yzzhushu.com/api/client"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pHeartNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idleHeart = false;
    connect(m_pHeartNetworkReply, SIGNAL(finished()), this, SLOT(onHeartReplyFinished()));
}

void DHttpWorker::on_alarmHttpPost()
{
    if(!m_idleAlarm)
    {
            return;
    }

    QString strSerial = QString("\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);;

    QString strContent  = QString("{");
    strContent += strSerial;

    if(!strAlarmList.isEmpty())
    {
        strContent += QString("\"alarm\":[");
        for(int i = 0; i < strAlarmList.size(); i++)
        {
                strContent += strAlarmList.at(i);
                if(i < (strAlarmList.size() - 1))
                {
                    strContent += QString(",");
                }
        }
        strContent += QString("]");
        strAlarmList.clear();
    }


    strContent += QString("}");

    qDebug() << strContent;

    QByteArray msgArray = strContent.toLatin1();

    QNetworkRequest request;
    request.setUrl(QUrl("https://s.yzzhushu.com/api/client"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pAlarmNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idleAlarm = false;
    connect(m_pAlarmNetworkReply, SIGNAL(finished()), this, SLOT(onAlarmReplyFinished()));
}

void DHttpWorker::on_runMsgHttpPost()
{
    //
}

void DHttpWorker::on_initHttp()
{
     m_networkAccessManager = new DNetworkAccessManager;
     m_timerID = this->startTimer(2000);
}

void DHttpWorker::onHeartReplyFinished()
{
    if(m_pHeartNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pHeartNetworkReply->readAll();
        emit feedback(array);
    }
    else
    {
        QString strError = QString("Heart NetworkReply Error:%1").arg(m_pHeartNetworkReply->error());
        QByteArray array = strError.toLatin1();
        emit feedback(array);
    }
    m_pHeartNetworkReply->deleteLater();
    m_idleHeart = true;
}

void DHttpWorker::onAlarmReplyFinished()
{
    if(m_pAlarmNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pAlarmNetworkReply->readAll();
        emit feedback(array);
    }
    else
    {
        QString strError = QString("Alarm NetworkReply Error:%1").arg(m_pAlarmNetworkReply->error());
        QByteArray array = strError.toLatin1();
        emit feedback(array);
        strAlarmList.append(strAlarmTempList);
    }
    m_pAlarmNetworkReply->deleteLater();
    m_idleAlarm = true;
}

void DHttpWorker::onRunMsgReplyFinished()
{
}

void DHttpWorker::updateOperatingData(const QString & msg, int index)
{
    if(index < NETWORK_DATA_NUM)
    {
        strOperatingData[index] = msg;

        strOperatingList.clear();
        for(int i = 0; i < NETWORK_DATA_NUM; i++)
        {
            if(!strOperatingData[i].isEmpty())
            {
                strOperatingList.append(strOperatingData[i]);
            }
        }
    }
}

void DHttpWorker::initJsonFormat()
{
    QFile jsonFile(":/json/heartJson.json");
    jsonFile.open(QFile::ReadOnly);
    m_heartJson = QLatin1String(jsonFile.readAll());
    jsonFile.close();
}


