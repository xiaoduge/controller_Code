#include "ex_comboboxdelegate.h"
#include <QComboBox>

Ex_ComboBoxDelegate::Ex_ComboBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *Ex_ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    QStringList strList;
    strList << "User" << "Manager";
    editor->addItems(strList);
    return editor;
}

void Ex_ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(value - 1);
}

void Ex_ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    int value = comboBox->currentIndex() + 1;
    model->setData(index, value, Qt::EditRole);
}

void Ex_ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
