#ifndef HISTORYLISTWIDGTITEM_H
#define HISTORYLISTWIDGTITEM_H

#include <QWidget>

#include <QWidget>
#include <QLabel>
#include <QBitArray>

#define Name_X 1
#define Name_Y 20

#define Date_X 160
#define Date_Y 20

class HistoryListWidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryListWidgtItem(QWidget *parent = 0);
    void HistorySet();
    void HistoryClear();

signals:

public slots:

private:

    QLabel *lbName;
    QLabel *lbDate;
};

#endif // HISTORYLISTWIDGTITEM_H
