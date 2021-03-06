#include "dsysmsgpage.h"
#include "exconfig.h"
#include "mainwindow.h"
#include <QListWidgetItem>


DSysMsgPage::DSysMsgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();

    buildTranslation();

}

void DSysMsgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void DSysMsgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Information");

    setTitles(stringList);

}

void DSysMsgPage::buildTranslation()
{
    switch(gAdditionalCfgParam.productInfo.iCompany)
    {
    case 0:
        buildTranslationTitleRephile();
        break;
    case 1:
        buildTranslationTitleVWR();
        break;
    default:
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

void DSysMsgPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void DSysMsgPage::setBackColor()
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

void DSysMsgPage::setTitleMsg()
{
    m_pExmsgLabel[SYSMS_PAGE_LB_TITLE]->setText(m_ExMsgName[SYSMS_PAGE_LB_TITLE]);
}

void DSysMsgPage::setNameMsg(int iIdx,QString strInfo)
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

void DSysMsgPage::updateNameMsg()
{
    setNameMsg(SYSMS_PAGE_LB_1, gAdditionalCfgParam.productInfo.strCatalogNo);
    setNameMsg(SYSMS_PAGE_LB_2, gAdditionalCfgParam.productInfo.strSerialNo);
    setNameMsg(SYSMS_PAGE_LB_3, gAdditionalCfgParam.productInfo.strProductDate);
    setNameMsg(SYSMS_PAGE_LB_4, gAdditionalCfgParam.productInfo.strInstallDate);
    setNameMsg(SYSMS_PAGE_LB_5, gAdditionalCfgParam.productInfo.strVersion);
}

void DSysMsgPage::buildTranslationTitleRephile()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_RO:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie PURIST");
        break;
    case MACHINE_ADAPT:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Genie A ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    default:
        break;
    }
}

void DSysMsgPage::buildTranslationTitleVWR()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("Super-Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_UP:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_RO:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR P");
        break;
    case MACHINE_ADAPT:
        m_ExMsgName[SYSMS_PAGE_LB_TITLE] = QString("VWR A ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    default:
        break;
    }
}

void DSysMsgPage::initUi()
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

void DSysMsgPage::update()
{
//    updateNameMsg();
    buildTranslation();
}
