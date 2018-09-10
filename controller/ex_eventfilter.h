#ifndef EX_EVENTFILTER_H
#define EX_EVENTFILTER_H

#include <QObject>
#include <QMutex>

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
    QMutex m_mutex;
    
};

#endif // EX_EVENTFILTER_H
