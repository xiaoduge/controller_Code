#include "ex_dsleepwidget.h"
#include <QPainter>

Ex_DSleepWidget::Ex_DSleepWidget(const QString& msg, int screenWidth, int screenHeight, QWidget *parent) :
    QWidget(parent),
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight)
{
    setMsg(msg);
    setPos(10, 30);
}

void Ex_DSleepWidget::setMsg(const QString &msg)
{
    m_msg = msg;
}

void Ex_DSleepWidget::setPos(int x, int y)
{
    m_pos.setX(x);
    m_pos.setY(y);
}

void Ex_DSleepWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    QFont font("Arial", 36, QFont::Bold);
    QFontMetrics fm(font);
    int strWidth = fm.width(m_msg);
    int strHeight = fm.height();
    if((m_pos.x() + strWidth) > m_screenWidth)
    {
        int x = m_screenWidth - strWidth - 5;
        m_pos.setX(x);
    }
    if((m_pos.y() + strHeight) > m_screenHeight)
    {
        int y = m_screenHeight - strHeight - 5;
        m_pos.setY(y);
    }
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(m_pos, m_msg);

    QWidget::paintEvent(event);
}
