#include "dbasewindow.h"
#include "ddialogtitle.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>

DBaseWindow::DBaseWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    initTitleBar();
}

DBaseWindow::~DBaseWindow()
{

}

void DBaseWindow::setTitleText(const QString &titleContent, const QColor &textColor, int titleFontSize)
{
    m_titleBar->setTitleContent(titleContent, textColor, titleFontSize);
}

void DBaseWindow::showEvent(QShowEvent *event)
{
    updatePage();
    QWidget::showEvent(event);
}

void DBaseWindow::initTitleBar()
{
    m_titleBar = new DDialogTitle(this);
    m_titleBar->move(0, 0);
}

void DBaseWindow::paintEvent(QPaintEvent* event)
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


