#include "historylistwidgtitem.h"

HistoryListWidgtItem::HistoryListWidgtItem(QWidget *parent) :
    QWidget(parent)
{

    this->lbName = new QLabel(this);
    this->lbName->setGeometry(QRect(Name_X, Name_Y ,150 , 20));


    this->lbDate = new QLabel(this);
    this->lbDate->setGeometry(QRect(Date_X, Date_Y ,150 , 20));


}

void HistoryListWidgtItem::HistoryClear()
{

}

void HistoryListWidgtItem::HistorySet( )
{
        this->lbName->setText("HistoryPage");

        this->lbDate->setText("Time");
}


