#include "dpushbutton.h"
#include <QPainter>

DPushButton::DPushButton(QWidget *parent):QPushButton(parent),
                         m_containText(false),
                         m_containPixmap(false),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("Arial", 10))
{

}

DPushButton::DPushButton(const QString &text, QWidget *parent):QPushButton(text, parent),
                         m_text(text),
                         m_containText(true),
                         m_containPixmap(false),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("Arial", 10))

{

}

DPushButton::DPushButton(const QPixmap &pixmap, QWidget *parent):QPushButton(parent),
                         m_pixmap(pixmap),
                         m_containText(false),
                         m_containPixmap(true),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("Arial", 10))
{

}

DPushButton::DPushButton(const QPixmap &pixmap, const QString &text, QWidget *parent):QPushButton(text, parent),
                         m_text(text),
                         m_pixmap(pixmap),
                         m_containText(true),
                         m_containPixmap(true),
                         m_isPress(false),
                         m_size(1),
                         m_penColor(Qt::black),
                         m_font(QFont("Arial", 10))
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

void DPushButton::paintEvent(QPaintEvent* e)
{
    if(m_containText && !m_containPixmap)
    {
        return QPushButton::paintEvent(e);
    }

    QPixmap spixmap(m_pixmap);

    int pw = spixmap.width();
    int ph = spixmap.height();

    QPixmap pixmap =spixmap.scaled(QSize(pw*m_size, ph*m_size),Qt::KeepAspectRatio);

    QFontMetrics fontMetrics(m_font);

    int wid = fontMetrics.width(m_text);
    int h = fontMetrics.height();

    if(m_containText)
    {
        if(wid > pixmap.width())
        {
            this->resize(wid + 2, pixmap.height() + h + 2);
        }
        else
        {
            this->resize(pixmap.width() + 2,  pixmap.height() + h + 2);
        }
    }
    else
    {
        this->resize(pixmap.width() + 2,  pixmap.height() + 2);
    }

    QPainter painter(this);
    painter.setFont(m_font);

    if(m_containPixmap)
    {
        QRect rect((this->width()-pixmap.width())/2, 1, pixmap.width(), pixmap.height());
        painter.drawPixmap(rect, pixmap);
    }

    painter.setPen(m_penColor);
    if(m_containText)
    {
       painter.drawText(QRect((this->width() -wid) /2, pixmap.height() + 2, wid, h), m_text);
    }

    if(m_isPress)
    {
        painter.fillRect(this->rect(), QColor(100, 100, 100, 60));
    }
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
