#ifndef DWIFICONFIGWIDGET_H
#define DWIFICONFIGWIDGET_H

#include <QWidget>

class DWifiConfigWidget : public QWidget
{
    Q_OBJECT

public:
    DWifiConfigWidget(QWidget *parent = 0);
    ~DWifiConfigWidget();

    virtual void updatePage() = 0;

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool m_isPressed;
    QPoint m_startMovePos;
};

#endif // BASEWINDOW_H
