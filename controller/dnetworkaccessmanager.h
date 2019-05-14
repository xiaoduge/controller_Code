#ifndef DNETWORKACCESSMANAGER_H
#define DNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class DNetworkAccessManager : public QNetworkAccessManager
{
public:
    DNetworkAccessManager(QObject *parent = 0);
protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // DNETWORKACCESSMANAGER_H
