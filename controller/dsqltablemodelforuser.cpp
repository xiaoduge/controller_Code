#include "dsqltablemodelforuser.h"

DSqlTableModelForUser::DSqlTableModelForUser(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

QVariant DSqlTableModelForUser::data(const QModelIndex &index, int role) const
{
    QVariant variant = QSqlTableModel::data(index, role);
    if(variant.isValid())
    {
        switch(index.column())
        {
        case 2:
        {
            if(0 != variant.toString().size())
            {
                return QVariant("******");
            }
            break;
        }
        case 3:
        {
            switch(variant.toInt())
            {
            case 1:
                return QVariant("User");
            case 2:
                return QVariant("Manager");
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
    return variant;
}
