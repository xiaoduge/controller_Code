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
                                            m_xmlGenerator(0),
                                            m_idleHeart(true),
                                            m_idleAlarm(true),
                                            m_idleDispense(true)
{

}

DHttpWorker::~DHttpWorker()
{
    if(m_networkAccessManager) m_networkAccessManager->deleteLater();

    if(m_xmlGenerator) m_xmlGenerator->deleteLater();
}

/**
 * 检查是否有报警信息上传，默认每2s检查一次
 * @param event [description]
 */
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

/**
 * 槽函数：接收报警信息，并添加到待上传报警信息列表
 * @param alarmInfo [报警内容]
 */
void DHttpWorker::on_updateAlarmList(const DNetworkAlaramInfo &alarmInfo)
{
    QMutexLocker locker(&m_mutex);
    m_alarmList.append(alarmInfo);
    m_alarmTempList = m_alarmList; //backup
}

/**
 * 槽函数：接收、打包、上传心跳数据
 * @param data [心跳数据]
 */
void DHttpWorker::on_updateHeartList(const DNetworkData &data)
{
    QMutexLocker locker(&m_mutex);
    QByteArray xmlArray = m_xmlGenerator->generator(data);

    xmlArray.replace("\"", "\'");

    on_heartHttpPost(xmlArray);
}

/**
 * 槽函数：接收、打包、上传取水数据
 * @param data [取水数据]
 */
void DHttpWorker::on_updateDispenseList(const DDispenseData &data)
{
    QMutexLocker locker(&m_mutex);
    QByteArray xmlArray = m_xmlGenerator->generator(data);

    xmlArray.replace("\"", "\'");

    on_dispenseHttpPost(xmlArray);
}

/**
 * 通过http post将心跳数据上传到指定地址
 * @param data [xml格式的心跳数据]
 */
void DHttpWorker::on_heartHttpPost(const QByteArray& xmlByte)
{
    if(!m_idleHeart)
    {
        return;
    }

    QString strContent = QString("{\"data\" : \"%1\"} ").arg(QString(xmlByte));

    QByteArray msgArray = strContent.toLatin1();

    qDebug() << msgArray;

    msgArray.replace("\n", "");

//    qDebug() << msgArray;

    QNetworkRequest request;
    request.setUrl(QUrl("http://106.14.204.27:8080/app/rest/v2/services/rephile_HeartDataService/uploadHeartData"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pHeartNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idleHeart = false;
    connect(m_pHeartNetworkReply, SIGNAL(finished()), this, SLOT(onHeartReplyFinished()));
}

/**
 * 通过http post将报警信息上传到指定地址
 * @param data [xml格式的报警信息]
 */
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

    qDebug() << msgArray;

    msgArray.replace("\n", "");

//    qDebug() << msgArray;

    QNetworkRequest request;
    request.setUrl(QUrl("http://106.14.204.27:8080/app/rest/v2/services/rephile_AlarmDataService/uploadAlarmData"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pAlarmNetworkReply = m_networkAccessManager->post(request, msgArray); //msgArray
    m_idleAlarm = false;
    connect(m_pAlarmNetworkReply, SIGNAL(finished()), this, SLOT(onAlarmReplyFinished()));

    m_alarmList.clear();
}

/**
 * 通过http post将取水数据上传到指定地址
 * @param data [xml格式的取水数据]
 */
void DHttpWorker::on_dispenseHttpPost(const QByteArray& xmlByte)
{
    if(!m_idleDispense)
    {
        return;
    }

    QString strContent = QString("{\"data\" : \"%1\"} ").arg(QString(xmlByte));

    QByteArray msgArray = strContent.toLatin1();
    qDebug() << msgArray;

    msgArray.replace("\n", "");

//    qDebug() << msgArray;

    QNetworkRequest request;
    request.setUrl(QUrl("http://106.14.204.27:8080/app/rest/v2/services/rephile_DispenseDataService/uploadDispenseData"));  //https
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_pDispenseNetworkReply = m_networkAccessManager->post(request, msgArray);
    m_idleDispense = false;
    connect(m_pDispenseNetworkReply, SIGNAL(finished()), this, SLOT(onDispenseReplyFinished()));
}

void DHttpWorker::on_initHttp()
{
    m_xmlGenerator = new DXmlGenerator;
    m_networkAccessManager = new DNetworkAccessManager;
    m_timerID = this->startTimer(2000);
}

/**
 * 处理上传心跳数据的返回数据
 */
void DHttpWorker::onHeartReplyFinished()
{
    if(m_pHeartNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pHeartNetworkReply->readAll();
        qDebug() << "heartData Feed back: " << array;
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

/**
 * 处理上传报警信息的返回数据，报警信息上传失败，则重新发送
 */
void DHttpWorker::onAlarmReplyFinished()
{
    if(m_pAlarmNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pAlarmNetworkReply->readAll();
        qDebug() << "AlarmInfo Feed back: " << array;
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

/**
 * 处理上传取水数据的返回数据
 */
void DHttpWorker::onDispenseReplyFinished()
{
    if(m_pDispenseNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray array = m_pDispenseNetworkReply->readAll();
        qDebug() << "DispenseData Feed back: " << array;
        emit feedback(array);
    }
    else
    {
        QString strError = QString("DispenseData NetworkReply Error:%1").arg(m_pDispenseNetworkReply->error());
        qDebug() << "DispenseData Error: " << strError;
        QByteArray array = strError.toLatin1();
        emit feedback(array);
    }
    m_pDispenseNetworkReply->deleteLater();
    m_idleDispense = true;
}



