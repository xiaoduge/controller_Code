#include "page.h"
#include "basewidget.h"
#include "mainwindow.h"

CPage::CPage(QObject *parent,CBaseWidget *widget,MainWindow  *wndMain) : QObject(parent),m_widget(widget),m_wndMain(wndMain)
{
    m_parent = (CPage *)parent;
    m_curTitle = 0;
    m_pLabelTitle = NULL;

    m_lstX = 0;
    m_lstY = 0;
    m_lstFlag = 0;    
    
    m_widget->setPage(this);

    m_wndMain->addPage(this);

    show(false);

}

void CPage::creatTitle()
{
//#define TITLE_WIDTH (200)
//    m_pLabelTitle= new QLabel(m_widget);
//    m_pLabelTitle->setGeometry(QRect(300 -  TITLE_WIDTH / 2, 26 ,TITLE_WIDTH ,24));
//    m_pLabelTitle->setFont(QFont("" , 24 ,  QFont::Bold));
//   m_pLabelTitle->setAlignment(Qt::AlignCenter);

}

void CPage::show(bool bShow) 
{
    if (bShow) 
    {
        m_widget->show() ; 

        if (m_wndMain ) m_wndMain->setCurrentPage(this);
    }
    else 
    {
        m_widget->hide();
    }
}

void CPage::setWidgetVisible(bool isVisible)
{
    m_widget->setVisible(isVisible);
}

