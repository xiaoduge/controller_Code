#ifndef WARNLISTWIDGTITEM_H
#define WARNLISTWIDGTITEM_H

#include <QWidget>
#include <QBitArray>

#define Type_X 27
#define Type_Y 12

#define Msg_X 100
#define Msg_Y 12

class QLabel;

class warnListWidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit warnListWidgtItem(QWidget *parent = 0);
    
    void WarnSet(warnListWidgtItem *);
    void WarnClear(warnListWidgtItem *);
    
signals:
    
public slots:
    
private:
    QLabel *lbType;
    QLabel *lbMsg;
    
};

#endif // WARNLISTWIDGTITEM_H
