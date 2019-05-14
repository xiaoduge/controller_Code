#include "dtextticker.h"
#include <QTimer>
#include <QPainter>


DTextTicker::DTextTicker(QWidget *parent, Qt::WindowFlags f):QLabel(parent, f)
{
    this->setMinimumSize(800, 35);

    m_posX = width();
    m_strList.clear();

    m_charWidth = fontMetrics().width('A');

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ticker()));
    m_timer->start(200);

}

void DTextTicker::updateShowText(const QString &msg, bool add)
{
    if(add)
    {
        m_strList.append(msg);
    }
    else
    {
        m_strList.removeOne(msg);
    }
    m_showText.clear();
    for(int i = 0; i < m_strList.size(); i++)
    {
        m_showText += m_strList[i];
        m_showText += "    ";
    }
}

void DTextTicker::paintEvent(QPaintEvent *event)
{
    QPen pen;

    QPainter painter(this);
    pen.setColor(QColor(255,0,255));
    painter.setPen(pen);

    m_posX -= m_charWidth;
    if(m_posX + fontMetrics().width(m_showText) < 0)
    {
        m_posX = 800;
    }
    painter.drawText(m_posX, 16, m_showText);

    QLabel::paintEvent(event);

}

void DTextTicker::ticker()
{
    update();
}
