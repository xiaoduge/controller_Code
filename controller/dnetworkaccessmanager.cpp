#include "dnetworkaccessmanager.h"
#include <QNetworkReply>

DNetworkAccessManager::DNetworkAccessManager(QObject* parent):QNetworkAccessManager(parent)
{
}

QNetworkReply* DNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                     const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkReply* reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    reply->ignoreSslErrors();
    return reply;

}
