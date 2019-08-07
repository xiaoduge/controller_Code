#include "mainwindow.h"
#include "subpage.h"
#include "titlebar.h"

CSubPage::CSubPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CPage(parent,widget,wndMain)
{

}

void CSubPage::creatTitle()
{
    CPage::creatTitle();

    m_pTitleBar = new titleBar(m_widget,1);
    
    m_pTitleBar->setGeometry(0,0,800,55);
    
    connect(m_pTitleBar, SIGNAL(clicked(int)), this, SLOT(on_navi_clicked(int)));
}

void CSubPage::leaveSubPage()
{
    show(false);
    m_parent->show(true);
}

void CSubPage::hideTitleBar()
{
    m_pTitleBar->hide();
}

void CSubPage::on_navi_clicked(int index)
{
    switch(index)
    {
    case TITLE_BAR_ID_POWER_OFF:
        break;
    case TITLE_BAR_ID_BACK:
        leaveSubPage();
        break;
    case TITLE_BAR_ID_HOME:
        m_wndMain->home();
        break;
    }
}

