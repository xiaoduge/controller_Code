#include "ex_dialogtitle.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>

Ex_DialogTitle::Ex_DialogTitle(QWidget *parent)
    : QWidget(parent),
      m_backgroundColor(38, 102, 192), //153, 153, 153
      m_borderWidth(0)
{
    initUI();
}

void Ex_DialogTitle::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
}

void Ex_DialogTitle::setTitleContent(const QString &titleContent, const QColor &textColor, int titleFontSize)
{
    QFont font = m_pTitleLabel->font();
    font.setPointSize(titleFontSize);
    m_pTitleLabel->setFont(font);

    QPalette palette = m_pTitleLabel->palette();
    palette.setColor(QPalette::WindowText, textColor);
    m_pTitleLabel->setPalette(palette);

    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    m_pTitleLabel->setText(titleContent);
}

void Ex_DialogTitle::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

void Ex_DialogTitle::setWindowBorderWidth(int borderWidth)
{
    m_borderWidth = borderWidth;
}

void Ex_DialogTitle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, m_backgroundColor);

    if (this->width() != (this->parentWidget()->width() - m_borderWidth))
    {
        this->setFixedWidth(this->parentWidget()->width() - m_borderWidth);
    }
    QWidget::paintEvent(event);
}

void Ex_DialogTitle::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_startMovePos = event->globalPos();

    return QWidget::mousePressEvent(event);
}

void Ex_DialogTitle::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void Ex_DialogTitle::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void Ex_DialogTitle::initUI()
{
    m_pTitleLabel = new QLabel;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pTitleLabel);
    this->setLayout(hLayout);
}
