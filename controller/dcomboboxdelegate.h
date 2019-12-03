/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-3
** @version   v0.0.1
** @brief     Item delegate for user configuration view
**
** 用户配置界面用户等级列代理，完成<int, string>间的映射
**
****************************************************************************/

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
