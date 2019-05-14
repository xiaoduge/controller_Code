#ifndef EX_COMBOBOXDELEGATE_H
#define EX_COMBOBOXDELEGATE_H

#include <QItemDelegate>

class Ex_ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit Ex_ComboBoxDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

signals:
    
public slots:
    
};

#endif // EX_COMBOBOXDELEGATE_H
