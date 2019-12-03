/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-3
** @version   v0.0.1
** @brief     Interface for data remote upload
**
****************************************************************************/

#ifndef DHTTPWORKER_H
#define DHTTPWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>
#include <QStringList>
#include "DNetworkConfig.h"

class DNetworkAccessManager;
class DXmlGenerator;

class DHttpWorker : public QObject
{
    Q_OBJECT
public:
    explicit DHttpWorker(QObject *parent = 0);
    ~DHttpWorker();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void feedback(const QByteArray&);

public slots:
    void on_updateAlarmList(const DNetworkAlaramInfo &alarmInfo);
    void on_updateHeartList(const DNetworkData&);
    void on_updateDispenseList(const DDispenseData&);

    void on_heartHttpPost(const QByteArray& xmlByte);
    void on_alarmHttpPost();
    void on_dispenseHttpPost(const QByteArray& xmlByte);

    void on_initHttp();

private slots:
    void onHeartReplyFinished();
    void onAlarmReplyFinished();
	void onDispenseReplyFinished();


private:
    DNetworkAccessManager *m_networkAccessManager;
    DXmlGenerator *m_xmlGenerator;

    QNetworkReply *m_pHeartNetworkReply;
    QNetworkReply *m_pAlarmNetworkReply;
    QNetworkReply *m_pDispenseNetworkReply;

    QList<DNetworkAlaramInfo> m_alarmList;
    QList<DNetworkAlaramInfo> m_alarmTempList;

    bool m_idleHeart;
    bool m_idleAlarm;
    bool m_idleDispense;

    QMutex m_mutex;

    int m_timerID;
};

#endif // DHTTPWORKER_H
