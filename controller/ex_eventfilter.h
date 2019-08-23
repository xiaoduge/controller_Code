#ifndef EX_EVENTFILTER_H
#define EX_EVENTFILTER_H

#include <QObject>
#include <QMutex>
#include "mainwindow.h"

class Ex_EventFilter : public QObject
{
    Q_OBJECT
public:
    explicit Ex_EventFilter(QObject *parent = 0);

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
