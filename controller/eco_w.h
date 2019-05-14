#ifndef ECO_W_H
#define ECO_W_H

#include <QObject>
#include <QString>

class ECO_W : public QObject
{
    Q_OBJECT
public:
    ECO_W(QObject *parent = 0);
    int iECOid;
    float iQuality;
    int   iTemp;
    QString time;
    
signals:
    
public slots:
    
};

#endif // ECO_W_H
