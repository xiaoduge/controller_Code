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

signals:
    void feedback(const QByteArray&);

public slots:
    void on_updateMsgList(const QString&, int code, int index);
    void on_heartHttpPost();

    void on_initHttp();

private slots:
    void onReplyFinished();

private:
    void updateOperatingData(const QString&, int index);


private:
    DNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_pNetworkReply;

    QStringList strAlarmList;
    QStringList strOperatingList; //Operating parameters
    QString strOperatingData[NETWORK_DATA_NUM]; //Operating parameters
    QStringList strHistoryList;
    QString strRunStatus;

    bool m_idle;
    QMutex m_mutex;
};

#endif // DHTTPWORKER_H
