#ifndef DEVENTFILTER_H
#define DEVENTFILTER_H

#include <QObject>
#include <QMutex>
#include "mainwindow.h"

class DEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit DEventFilter(QObject *parent = 0);

public:
    bool eventFilter(QObject *, QEvent *);
    
signals:
    
public slots:

private:
#ifdef TOUCHTEST
    void touchTest(QEvent *e);
#endif
    
};

#endif // EX_EVENTFILTER_H
