#include "dlineedit.h"

DLineEdit::DLineEdit(QWidget *parent) : QLineEdit(parent)
{
}

DLineEdit::DLineEdit(const QString &contents, QWidget *parent) : QLineEdit(contents, parent)
{
}

void DLineEdit::mousePressEvent(QMouseEvent *event)
{
    if(this->hasFocus())
    {
        this->clearFocus();
    }

    return QLineEdit::mousePressEvent(event);
}

void DLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    this->setFocus();
    return QLineEdit::mouseReleaseEvent(event);
}
