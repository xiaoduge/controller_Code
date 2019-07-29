#include "ex_usermanualpage.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QLabel>


Ex_UserManualPage::Ex_UserManualPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_curPage = 0;
    initPixNameList();

    creatTitle();
    initUi();
    buildTranslation();
}

void Ex_UserManualPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_UserManualPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("User Manual");

    setTitles(stringList);

}

void Ex_UserManualPage::buildTranslation()
{

}

void Ex_UserManualPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void Ex_UserManualPage::setBackColor()
{
    QSize size(width(),height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(228, 231, 240));
    QPalette pal(m_widget->palette());
    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));
    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void Ex_UserManualPage::createControl()
{
    QWidget *tmpWidget = NULL;

    tmpWidget = new QWidget(m_widget);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(0, 55, 800 ,545)); //QRect(5 , 70 , 773 ,500)

    m_pContentLb = new QLabel(tmpWidget);
    m_pContentLb->setGeometry(QRect(0, 0, 800, 545));

    //
    m_pixmap = QPixmap(m_strList.at(m_curPage));
    m_pContentLb->setPixmap(m_pixmap);
    m_pContentLb->setScaledContents(true);

}

void Ex_UserManualPage::loadNewContent(bool next)
{
    if (next)
    {
        if(m_curPage > 0)
        {
            m_curPage--;
            m_pixmap = QPixmap(m_strList.at(m_curPage));
            m_pContentLb->setPixmap(m_pixmap);
            m_pContentLb->setScaledContents(true);

        }
    }

    else
    {
        if(m_curPage < (m_maxPageNum[gGlobalParam.iMachineType] - 1))
        {
            m_curPage++;
            m_pixmap = QPixmap(m_strList.at(m_curPage));
            m_pContentLb->setPixmap(m_pixmap);
            m_pContentLb->setScaledContents(true);

        }

    }

}


void Ex_UserManualPage::initUi()
{
    setBackColor();
    createControl();
}

void Ex_UserManualPage::update()
{
//    loadNewContent();
    m_pixmap = QPixmap(m_strList.at(m_curPage));
    m_pContentLb->setPixmap(m_pixmap);
    m_pContentLb->setScaledContents(true);
}

void Ex_UserManualPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }
    m_curX = e->x();
    m_curY = e->y();

}

void Ex_UserManualPage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_lstFlag = 1;
        m_curX = e->x();
        m_curY = e->y();
    }
}

void Ex_UserManualPage::mouseReleaseEvent(QMouseEvent *e)
{

    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        loadNewContent(m_curX - m_lstX > 0 ? true : false);
    }

    m_lstFlag = 0;

}

void Ex_UserManualPage::initMaxPageNum()
{
    m_maxPageNum[MACHINE_L_Genie] = 0;
    m_maxPageNum[MACHINE_L_UP] = 0;
    m_maxPageNum[MACHINE_L_EDI_LOOP] = 0;
    m_maxPageNum[MACHINE_L_RO_LOOP] = 0;

    m_maxPageNum[MACHINE_Genie] = 8;
    m_maxPageNum[MACHINE_UP] = 0;
    m_maxPageNum[MACHINE_EDI] = 0;
    m_maxPageNum[MACHINE_RO] = 0;

    m_maxPageNum[MACHINE_PURIST] = 0;
    m_maxPageNum[MACHINE_ADAPT] = 0;


}

void Ex_UserManualPage::initPixNameList()
{
    int iLoop;
    QString tmp;

    initMaxPageNum();

    if(0 == m_maxPageNum[gGlobalParam.iMachineType])
    {
        tmp = QString("://userManual/Genie_0.png");
        m_strList << tmp;
    }
    else
    {
        for(iLoop = 0; iLoop < m_maxPageNum[gGlobalParam.iMachineType]; iLoop++)
        {
            tmp = QString("://userManual/G_E_%1.png").arg(iLoop+1);
            m_strList << tmp;
        }
    }

}














