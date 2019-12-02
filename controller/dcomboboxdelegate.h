#ifndef DCOMBOBOXDELEGATE_H
#define DCOMBOBOXDELEGATE_H

#include <QItemDelegate>

class DComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DComboBoxDelegate(QObject *parent = 0);

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
