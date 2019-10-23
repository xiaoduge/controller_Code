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

    void on_heartHttpPost(const QByteArray& xmlByte);
    void on_alarmHttpPost();

    void on_initHttp();

private slots:
    void onHeartReplyFinished();
    void onAlarmReplyFinished();


private:
    DXmlGenerator *m_xmlGenerator;
    DNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_pHeartNetworkReply;
    QNetworkReply *m_pAlarmNetworkReply;

    QList<DNetworkAlaramInfo> m_alarmList;
    QList<DNetworkAlaramInfo> m_alarmTempList;

    bool m_idleHeart;
    bool m_idleAlarm;

    QMutex m_mutex;

    int m_timerID;
};

#endif // DHTTPWORKER_H
