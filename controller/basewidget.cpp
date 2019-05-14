#include "basewidget.h"
#include "page.h"
#include <QDebug>


CBaseWidget::CBaseWidget(QWidget *parent) : QWidget(parent)
{
    this->installEventFilter(this);
    this->m_pPage = NULL;
}

bool CBaseWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this )
    {
        //if(event->type()==QEvent::ShowToParent)
        //if (QEvent::WindowActivate == event->type())
        if (QEvent::Show == event->type())
        {
            qDebug()<< this << "show";
            if(this->m_pPage)
            {
                this->m_pPage->update();
            }
        }
        else if (event->type()== QEvent::Hide)
        {
            qDebug()<< this << "hide";
            if(this->m_pPage)
            {
                this->m_pPage->fade();
            }

        }
    }
    return QWidget::eventFilter(watched,event); ;
}

void CBaseWidget::setPage(CPage *page)
{
    m_pPage = page;
}

void CBaseWidget::mousePressEvent(QMouseEvent *e)
{
    m_pPage->mousePressEvent(e);
}

void CBaseWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_pPage->mouseReleaseEvent(e);
}

void CBaseWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_pPage->mouseMoveEvent(e);
}

