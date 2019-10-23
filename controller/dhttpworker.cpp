#include "dhttpworker.h"
#include "dnetworkaccessmanager.h"
#include "dxmlgenerator.h"
#include "ExtraDisplay.h"
#include <QMutexLocker>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QTimerEvent>

DHttpWorker::DHttpWorker(QObject *parent) : QObject(parent),
                                            m_networkAccessManager(0),
                                            m_xmlGenerator(0)
{
    m_idleHeart = true;
    m_idleAlarm = true;
}

DHttpWorker::~DHttpWorker()
{
    if(m_networkAccessManager) m_networkAccessManager->deleteLater();

    if(m_xmlGenerator) m_xmlGenerator->deleteLater();
}

void DHttpWorker::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerID)
    {
        if(!m_alarmList.empty())
        {
            on_alarmHttpPost();
        }
    }
}

void DHttpWorker::on_updateAlarmList(const DNetworkAlaramInfo &alarmInfo)
{
    QMutexLocker locker(&m_mutex);
    m_alarmList.append(alarmInfo);
    m_alarmTempList = m_alarmList; //backup
}

void DHttpWorker::on_updateHeartList(const DNetworkData &data)
{
    QMutexLocker locker(&m_mutex);
    QByteArray xmlArray = m_xmlGenerator->generator(data);

    xmlArray.replace("\"", "\'");

    on_heartHttpPost(xmlArray);
}

void DHttpWorker::on_heartHttpPost(const QByteArray& xmlByte)
{
    if(!m_idleHeart)
    {
        return;
    }

    QString strContent = QString("{\"data\" : \"%1\"} ").arg(QString(xmlByte));

    QByteArray msgArray = strContent.toLatin1();
//    qDebug() << msgArray;

    msgArray.replace("\n", "");

    qDebug() << msgArray;

    QNetworkRequest request;
    request.setUrl(QUrl("http://106.14.204.27:8080/app/rest/v2/services/rephile_HeartDataService/uploadHeartData"));  //https
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
    QByteArray xmlArray = m_xmlGenerator->generator(m_alarmList);

    xmlArray.replace("\"", "\'");
    QString strContent = QString("{\"data\" : \"%1\"} ").arg(QString(xmlArray));

    QByteArray msgArray = strContent.toLatin1();
//    qDebug() << msgArray;

    msgArray.replace("\n", "");

    qDebug() << msgArray;

    QNetworkRequest request;
    request.setUrl(QUrl("http://106.14.204.27:8080/app/rest/v2/services/rephile_AlarmDataService/uploadAlarmData"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pAlarmNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idleAlarm = false;
    connect(m_pAlarmNetworkReply, SIGNAL(finished()), this, SLOT(onAlarmReplyFinished()));

    m_alarmList.clear();
}

void DHttpWorker::on_initHttp()
{
    m_xmlGenerator = new DXmlGenerator;
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
        qDebug() << "HeartData Error: " << strError;
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
        qDebug() << "AlarmData Error: " << strError;
        QByteArray array = strError.toLatin1();
        emit feedback(array);
        m_alarmList.append(m_alarmTempList);
    }
    m_pAlarmNetworkReply->deleteLater();
    m_idleAlarm = true;
}


