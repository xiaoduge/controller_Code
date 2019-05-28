#include "dwificonfigwidget.h"
#include <QPainter>
#include <QMouseEvent>

DWifiConfigWidget::DWifiConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(300, 180);
}

DWifiConfigWidget::~DWifiConfigWidget()
{

}

void DWifiConfigWidget::showEvent(QShowEvent *event)
{
    updatePage();
    QWidget::showEvent(event);
}


void DWifiConfigWidget::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 10, 10);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(38, 102, 192)));  //38, 102, 192

    painter.setPen(Qt::white);
    painter.drawLine(0, 140, 300, 140);
    painter.drawLine(150, 140, 150, 180);
    return QWidget::paintEvent(event);
}

void DWifiConfigWidget::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_startMovePos = event->globalPos();

    return QWidget::mousePressEvent(event);
}

void DWifiConfigWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->pos();
        m_startMovePos = event->globalPos();
        this->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void DWifiConfigWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}


