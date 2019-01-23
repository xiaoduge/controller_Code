#include "ex_basewindow.h"
#include "ex_dialogtitle.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>

Ex_BaseWindow::Ex_BaseWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    initTitleBar();
}

Ex_BaseWindow::~Ex_BaseWindow()
{

}

void Ex_BaseWindow::setTitleText(const QString &titleContent, const QColor &textColor, int titleFontSize)
{
    m_titleBar->setTitleContent(titleContent, textColor, titleFontSize);
}

void Ex_BaseWindow::showEvent(QShowEvent *event)
{
    updatePage();
    QWidget::showEvent(event);
}

void Ex_BaseWindow::initTitleBar()
{
    m_titleBar = new Ex_DialogTitle(this);
    m_titleBar->move(0, 0);
}

void Ex_BaseWindow::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 10, 10);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(23, 78, 158)));  //238, 223, 204

    return QWidget::paintEvent(event);
}


