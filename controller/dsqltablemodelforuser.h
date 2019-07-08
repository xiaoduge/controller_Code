#ifndef DSQLTABLEMODELFORUSER_H
#define DSQLTABLEMODELFORUSER_H

#include <QSqlTableModel>

class DSqlTableModelForUser : public QSqlTableModel
{
public:
    DSqlTableModelForUser(QObject * parent = 0, QSqlDatabase db = QSqlDatabase());

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif // DSQLTABLEMODELFORUSER_H
