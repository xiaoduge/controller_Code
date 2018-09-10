#ifndef CBASEWIDGET_H
#define CBASEWIDGET_H

#include <QWidget>
#include <QEvent>

class CPage;

class CBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CBaseWidget(QWidget *parent = 0);
    void setPage(CPage *page);
signals:

public slots:
    bool eventFilter(QObject *, QEvent *);
public:
    CPage *m_pPage;
    
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // CBASEWIDGET_H
