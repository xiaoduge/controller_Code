#include "dpushbutton.h"
#include <QPainter>
#include <QDebug>

DPushButton::DPushButton(QWidget *parent, int id):QPushButton(parent),
                         m_containText(false),
                         m_containPixmap(false),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("", 13, QFont::Bold)),
                         m_id(id),
                         m_moveRule(CenterMove),
                         m_initSize(false)
{

}

DPushButton::DPushButton(const QString &text, QWidget *parent, int id):QPushButton(text, parent),
                         m_text(text),
                         m_containText(true),
                         m_containPixmap(false),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("", 13, QFont::Bold)),
                         m_id(id),
                         m_moveRule(CenterMove),
                         m_initSize(false)

{

}

DPushButton::DPushButton(const QPixmap &pixmap, QWidget *parent, int id):QPushButton(parent),
                         m_pixmap(pixmap),
                         m_containText(false),
                         m_containPixmap(true),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("", 13, QFont::Bold)),
                         m_id(id),
                         m_moveRule(CenterMove),
                         m_initSize(false)
{

}

DPushButton::DPushButton(const QPixmap &pixmap, const QString &text, QWidget *parent, int id):QPushButton(text, parent),
                         m_text(text),
                         m_pixmap(pixmap),
                         m_containText(true),
                         m_containPixmap(true),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("", 13, QFont::Bold)),
                         m_id(id),
                         m_moveRule(CenterMove),
                         m_initSize(false)
{

}

void DPushButton::setPixmapScale(double i)
{
    m_size = i;
}

void DPushButton::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    m_containPixmap = true;
}

void DPushButton::setTextColor(const QColor &color)
{
    m_penColor = color;
}

void DPushButton::setTextFont(const QFont &font)
{
    m_font = font;
}

void DPushButton::setText(const QString &text)
{
    m_text = text;
    m_containText = true;
}

void DPushButton::setID(int id)
{
    m_id = id;
}

void DPushButton::setMoveRule(DPushButton::MoveRule rule)
{
    m_moveRule = rule;
}

void DPushButton::cmove(int x, int y)
{
    this->cmove(QPoint(x, y));
}

void DPushButton::cmove(const QPoint &point)
{
    m_point.setX(point.x());
    m_point.setY(point.y());

    if(!m_initSize)
    {
        initSize();
    }

    switch (m_moveRule)
    {
    case NormalMove:
        this->move(point);
        break;
    case CenterMove:
        this->move(QPoint(point.x() - m_width/2, point.y()));
        break;
    default:
        break;
    }
}

int DPushButton::id() const
{
    return m_id;
}

const QPoint &DPushButton::originalPos() const
{
    return m_point;
}

void DPushButton::paintEvent(QPaintEvent* e)
{
    if(m_containText && !m_containPixmap)
    {
        QPushButton::setText(m_text);
        return QPushButton::paintEvent(e);
    }

    int pw = m_pixmap.width();
    int ph = m_pixmap.height();

    QPixmap pixmap =m_pixmap.scaled(QSize(pw*m_size, ph*m_size), Qt::KeepAspectRatio);

    QFontMetrics fontMetrics(m_font);

    int wid = fontMetrics.width(m_text);
    int h = fontMetrics.height();

    if(m_containText)
    {
        if(wid >= pixmap.width())
        {
            m_width = wid + 2;
            m_height = pixmap.height() + h + 2;
        }
        else
        {
            m_width = pixmap.width() + 2;
            m_height = pixmap.height() + h + 2;
        }
    }
    else
    {
        m_width = pixmap.width() + 2;
        m_height = pixmap.height() + 2;
    }
    this->resize(m_width, m_height);

    QPainter painter(this);
    painter.setFont(m_font);

    if(m_containPixmap)
    {
        QRect rect((m_width-pixmap.width())/2, 1, pixmap.width(), pixmap.height());
        painter.drawPixmap(rect, pixmap);
    }

    painter.setPen(m_penColor);
    if(m_containText)
    {
        painter.drawText(QRect((m_width - wid) /2, pixmap.height() + 2, wid, h), m_text);
    }

    if(m_isPress)
    {
        painter.fillRect(this->rect(), QColor(100, 100, 100, 60));
    }

    this->cmove(m_point);
}

void DPushButton::mousePressEvent(QMouseEvent* e)
{
    if(m_containText && !m_containPixmap)
    {
        return QPushButton::mousePressEvent(e);
    }

    m_isPress = true;
    repaint();

    emit pressed();
}

void DPushButton::mouseReleaseEvent(QMouseEvent* e)
{
    if(m_containText && !m_containPixmap)
    {
        return QPushButton::mouseReleaseEvent(e);
    }

    m_isPress = false;
    repaint();

    emit clicked();
    emit released();
}

void DPushButton::initSize()
{
    if(m_containText && !m_containPixmap)
    {
        return;
    }

    int pw = m_pixmap.width();
    int ph = m_pixmap.height();

    QPixmap pixmap =m_pixmap.scaled(QSize(pw*m_size, ph*m_size), Qt::KeepAspectRatio);

    QFontMetrics fontMetrics(m_font);

    int wid = fontMetrics.width(m_text);
    int h = fontMetrics.height();

    if(m_containText)
    {
        if(wid >= pixmap.width())
        {
            m_width = wid + 2;
            m_height = pixmap.height() + h + 2;
        }
        else
        {
            m_width = pixmap.width() + 2;
            m_height = pixmap.height() + h + 2;
        }
    }
    else
    {
        m_width = pixmap.width() + 2;
        m_height = pixmap.height() + 2;
    }
    this->resize(m_width, m_height);
    m_initSize = true;
}

