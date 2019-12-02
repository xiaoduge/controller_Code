#include "dcomboboxdelegate.h"
#include <QComboBox>

DComboBoxDelegate::DComboBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    QStringList strList;
    strList << "User" << "Manager";
    editor->addItems(strList);
    return editor;
}

void DComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(value - 1);
}

void DComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    int value = comboBox->currentIndex() + 1;
    model->setData(index, value, Qt::EditRole);
}

void DComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
