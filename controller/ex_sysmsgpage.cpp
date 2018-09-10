#include "ex_sysmsgpage.h"
#include "ExtraDisplay.h"
#include "titlebar.h"
#include "mainwindow.h"
#include <QPainter>
#include <QScrollBar>
#include <QListWidgetItem>


Ex_SysMsgPage::Ex_SysMsgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

}

void Ex_SysMsgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_SysMsgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Information");

    setTitles(stringList);

}

void Ex_SysMsgPage::buildTranslation()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Super-Genie G");
        break;
    case MACHINE_L_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Super-Genie U");
        break;
    case MACHINE_L_EDI_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = ("Super-Genie E");
        break;
    case MACHINE_L_RO_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Super-Genie R");
        break;
    case MACHINE_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Genie G");
        break;
    case MACHINE_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Genie U");
        break;
    case MACHINE_EDI:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Genie E");
        break;
    case MACHINE_RO:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Genie R");
        break;
    case MACHINE_PURIST:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("PURIST U");
        break;
    case MACHINE_ADAPT:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = tr("Genie A");
        break;
    }

    m_ExMsgName[SYSMS_PAGE_LB_1] = tr("Catalog No.")       + " :  %1";
    m_ExMsgName[SYSMS_PAGE_LB_2] = tr("Serial No.")        + " :  %1";
    m_ExMsgName[SYSMS_PAGE_LB_3] = tr("Production Date")   + " :  %1";
    m_ExMsgName[SYSMS_PAGE_LB_4] = tr("Installation Date") + " :  %1";
    m_ExMsgName[SYSMS_PAGE_LB_5] = tr("Software Version")  + " :  %1";

    setTitleMsg();
    updateNameMsg();
}

void Ex_SysMsgPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void Ex_SysMsgPage::setBackColor()
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

void Ex_SysMsgPage::setTitleMsg()
{
    m_pExmsgLabel[SYSMS_PAGE_LB_TITLE]->setText(m_ExMsgName[SYSMS_PAGE_LB_TITLE]);
}

void Ex_SysMsgPage::setNameMsg(int iIdx,QString strInfo)
{
    QString tmpMsg;

    switch(iIdx)
    {
    case SYSMS_PAGE_LB_1:
        tmpMsg = m_ExMsgName[SYSMS_PAGE_LB_1].arg(strInfo);
        m_pExmsgLabel[SYSMS_PAGE_LB_1]->setText(tmpMsg);
        break;
    case SYSMS_PAGE_LB_2:
        tmpMsg = m_ExMsgName[SYSMS_PAGE_LB_2].arg(strInfo);
        m_pExmsgLabel[SYSMS_PAGE_LB_2]->setText(tmpMsg);
        break;
    case SYSMS_PAGE_LB_3:
        tmpMsg = m_ExMsgName[SYSMS_PAGE_LB_3].arg(strInfo);
        m_pExmsgLabel[SYSMS_PAGE_LB_3]->setText(tmpMsg);
        break;
    case SYSMS_PAGE_LB_4:
        tmpMsg = m_ExMsgName[SYSMS_PAGE_LB_4].arg(strInfo);
        m_pExmsgLabel[SYSMS_PAGE_LB_4]->setText(tmpMsg);
        break;
    case SYSMS_PAGE_LB_5:
        tmpMsg = m_ExMsgName[SYSMS_PAGE_LB_5].arg(strInfo);
        m_pExmsgLabel[SYSMS_PAGE_LB_5]->setText(tmpMsg);
        break;
    default:
        break;
    }

}

void Ex_SysMsgPage::updateNameMsg()
{
    setNameMsg(SYSMS_PAGE_LB_1, ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo);
    setNameMsg(SYSMS_PAGE_LB_2, ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);
    setNameMsg(SYSMS_PAGE_LB_3, ex_gGlobalParam.Ex_System_Msg.Ex_ProDate);
    setNameMsg(SYSMS_PAGE_LB_4, ex_gGlobalParam.Ex_System_Msg.Ex_InsDate);
    setNameMsg(SYSMS_PAGE_LB_5, ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer);
}

void Ex_SysMsgPage::initUi()
{
    QFont fontTitle("Arial",24,QFont::Bold);

    QFont fontMsg("Arial",24,0);

    setBackColor();

    m_pWidget = new QWidget(m_widget);

    QPalette pal(m_pWidget->palette());

    pal.setColor(QPalette::Background, QColor(255, 255, 255));
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal);
    m_pWidget->setGeometry(QRect(124,124,550,350));

    QHBoxLayout* hLayout = new QHBoxLayout;
    QVBoxLayout* vLayout = new QVBoxLayout;
    QVBoxLayout* msgLayout = new QVBoxLayout;
    hLayout->setAlignment(Qt::AlignHCenter);
    vLayout->setAlignment(Qt::AlignCenter);
    msgLayout->setAlignment(Qt::AlignCenter);

    m_pExmsgLabel[SYSMS_PAGE_LB_TITLE] = new QLabel;
    m_pExmsgLabel[SYSMS_PAGE_LB_TITLE]->setFont(fontTitle);

    hLayout->addWidget(m_pExmsgLabel[SYSMS_PAGE_LB_TITLE]); //title;

    for(int iLoop = 1 ; iLoop < SYSMS_PAGE_LB_NUM; iLoop++)
    {
        m_pExmsgLabel[iLoop] = new QLabel;
        m_pExmsgLabel[iLoop]->setFont(fontMsg);
        vLayout->addWidget(m_pExmsgLabel[iLoop]);
    }
    msgLayout->addLayout(hLayout);
    msgLayout->addLayout(vLayout);

    m_pWidget->setLayout(msgLayout);   

}

void Ex_SysMsgPage::update()
{
    updateNameMsg();
}
