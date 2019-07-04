#ifndef DLINEEDIT_H
#define DLINEEDIT_H

#include <QLineEdit>

class DLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit DLineEdit(QWidget *parent = 0);
    explicit DLineEdit(const QString & contents, QWidget * parent = 0 );

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    
signals:
    
public slots:
    
};

#endif // DLINEEDIT_H
