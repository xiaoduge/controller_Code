#ifndef DHTTPWORKER_H
#define DHTTPWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>
#include <QStringList>
#include "DNetworkConfig.h"

class DNetworkAccessManager;

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
    void on_updateRunMsgList(const QString&, int index);
    void on_updateAlarmList(const QString&);
    void on_updateHeartList(const NetworkData&);

    void on_heartHttpPost();
    void on_alarmHttpPost();
    void on_runMsgHttpPost();

    void on_initHttp();

private slots:
    void onHeartReplyFinished();
    void onAlarmReplyFinished();
    void onRunMsgReplyFinished();

private:
    void updateOperatingData(const QString&, int index);
    void initJsonFormat();


private:
    DNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_pHeartNetworkReply;
    QNetworkReply *m_pAlarmNetworkReply;

    QString m_heartJson;

    QStringList strAlarmList;
    QStringList strAlarmTempList;

    QStringList strOperatingList; //Operating parameters
    QString strOperatingData[NETWORK_DATA_NUM]; //Operating parameters
    QString strHeartMsg;

    bool m_idleHeart;
    bool m_idleAlarm;
    bool m_idleRunMsg;

    QMutex m_mutex;

    int m_timerID;
};

#endif // DHTTPWORKER_H
