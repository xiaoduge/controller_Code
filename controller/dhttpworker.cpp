#include "dhttpworker.h"
#include "dnetworkaccessmanager.h"
#include "ExtraDisplay.h"
#include <QMutexLocker>

DHttpWorker::DHttpWorker(QObject *parent) : QObject(parent)
{
    m_idle = true;

    for(int i = 0; i < NETWORK_DATA_NUM; i++)
    {
        strOperatingData[i].clear();
    }
    strRunStatus.clear();

}

DHttpWorker::~DHttpWorker()
{
    m_networkAccessManager->deleteLater();
}

void DHttpWorker::on_updateMsgList(const QString &msg, int code, int index)
{
    QMutexLocker locker(&m_mutex);
    switch (code)
    {
    case 0:
        strAlarmList.append(msg);
        break;
    case 1:
        updateOperatingData(msg, index);
        break;
    case 2:
        strHistoryList.append(msg);
        break;
    case 3:
        strRunStatus = msg;
        break;
    default:
        break;
    }
}

void DHttpWorker::on_heartHttpPost()
{
    QMutexLocker locker(&m_mutex);
    if(!m_idle)
    {
        return;
    }

    if(strAlarmList.isEmpty() && strOperatingList.isEmpty() && strHistoryList.isEmpty() && strRunStatus.isEmpty())
    {
        return;
    }

    QString strContent  = QString("{\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);

    if(!strRunStatus.isEmpty())
    {
        strContent += QString("\"workStatus\":{");
        strContent += strRunStatus;
        strContent += QString("}");
        strRunStatus.clear();
    }

    if(!strAlarmList.isEmpty())
    {
        if(strContent != QString("{\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo))
        {
            strContent += QString(",");
        }

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

    if(!strOperatingList.isEmpty())
    {
        if(strContent != QString("{\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo))
        {
            strContent += QString(",");
        }

        strContent += QString("\"operating\":[");
        for(int i = 0; i < strOperatingList.size(); i++)
        {
            strContent += strOperatingList.at(i);
            if(i < (strOperatingList.size() - 1))
            {
                strContent += QString(",");
            }
        }
        strContent += QString("]");
        strOperatingList.clear();
    }
    if(!strHistoryList.isEmpty())
    {
        if(strContent != QString("{\"serial\":{\"serialNo\":\"%1\"},").arg(ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo))
        {
            strContent += QString(",");
        }

        strContent += QString("\"history\":[");
        for(int i = 0; i < strHistoryList.size(); i++)
        {
            strContent += strHistoryList.at(i);
            if(i < (strHistoryList.size() - 1))
            {
                strContent += QString(",");
            }

        }
        strContent += QString("]");
        strHistoryList.clear();
    }

    strContent += QString("}");

    QByteArray msgArray = strContent.toLatin1();

    QNetworkRequest request;
    request.setUrl(QUrl("https://s.yzzhushu.com/api/client"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idle = false;
    connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void DHttpWorker::on_initHttp()
{
     m_networkAccessManager = new DNetworkAccessManager;
}

void DHttpWorker::onReplyFinished()
{
    if(m_pNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pNetworkReply->readAll();
        emit feedback(array);
    }
    else
    {
        QString strError = QString("NetworkReply Error:%1").arg(m_pNetworkReply->error());
        QByteArray array = strError.toLatin1();
        emit feedback(array);
    }
    m_pNetworkReply->deleteLater();
    m_idle = true;
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


