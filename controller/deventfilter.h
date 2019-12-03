/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-3
** @version   v0.0.1
** @brief     Interface for eventfilter
**
** 时间过滤器，当前主要用于过滤QEvent::MouseButtonPress事件，以计算无操作时间
****************************************************************************/

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
