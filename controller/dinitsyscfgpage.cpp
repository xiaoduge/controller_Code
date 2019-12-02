#include "dinitsyscfgpage.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>


DInitSyscfgpage::DInitSyscfgpage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
    this->hideTitleBar();
}

void DInitSyscfgpage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void DInitSyscfgpage::buildTitles()
{
    QStringList stringList;

    stringList << tr("System Config");

    setTitles(stringList);

}

void DInitSyscfgpage::buildTranslation()
{
    m_pExLbTitle->setText(tr("Configuration"));

    m_chCfg[DISP_CFG_ElecLeakProtector]->setText(tr("Leak Protector"));
    m_chCfg[DISP_CFG_TankUV]->setText(tr("Tank UV"));


    m_pExNextBtn->setText(tr("Next"));
    m_pExBackBtn->setText(tr("Back"));
}

void DInitSyscfgpage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void DInitSyscfgpage::setBackColor()
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

void DInitSyscfgpage::createControl()
{
    QString strQss4Chk = m_wndMain->getQss4Chk();

    QWidget *tmpWidget = NULL;

    tmpWidget = new QWidget(m_widget);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(134 , 190 , 530 ,60));

    m_chCfg[DISP_CFG_ElecLeakProtector] = new QCheckBox(tmpWidget);
    m_chCfg[DISP_CFG_ElecLeakProtector]->setStyleSheet(strQss4Chk);

    m_chCfg[DISP_CFG_TankUV] = new QCheckBox(tmpWidget);
    m_chCfg[DISP_CFG_TankUV]->setStyleSheet(strQss4Chk);


    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->addWidget(m_chCfg[DISP_CFG_ElecLeakProtector]);
    line1Layout->addWidget(m_chCfg[DISP_CFG_TankUV]);
    tmpWidget->setLayout(line1Layout);

    m_pExNextBtn = new QPushButton(m_widget);
    m_pExBackBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_ExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_ExBackBtn_clicked()));

    m_pExBackBtn->move(200, 450);
    m_pExNextBtn->move(500, 450);
}


void DInitSyscfgpage::initUi()
{
    setBackColor();
    createHeader();
    createControl();
}

void DInitSyscfgpage::update()
{
    connectData();
}

void DInitSyscfgpage::createHeader()
{
    m_pExLbTitle = new QLabel(m_widget);
    m_pExLbTitle->setGeometry(QRect(134, 135 , 220 , 28));
    m_pExLbTitle->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");

}

void DInitSyscfgpage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_curX = e->x();
        m_curY = e->y();
        m_lstFlag = 1;
    }
}

void DInitSyscfgpage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void DInitSyscfgpage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        save();
        m_wndMain->naviInitPage(Ex_Init_Syscfg, m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}


void DInitSyscfgpage::connectData()
{
    for(int i = 0; i < DISP_CFG_NUMBER; i++)
    {
        switch(i)
        {
        case 0:
        {
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_ElecLeakProtector))
            {
                m_chCfg[0]->setChecked(true);
            }
            else
            {
                m_chCfg[0]->setChecked(false);
            }
            break;
        }
        case 1:
        {
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TankUV))
            {
                m_chCfg[1]->setChecked(true);
            }
            else
            {
                m_chCfg[1]->setChecked(false);
            }
            break;
        }
        default:
            break;
        }
    }
}

void DInitSyscfgpage::save()
{
    DISP_SUB_MODULE_SETTING_STRU  smParam = gGlobalParam.SubModSetting;

    if ((Qt::Checked == m_chCfg[DISP_CFG_ElecLeakProtector]->checkState()))
    {
        smParam.ulFlags |= 1 << DISP_SM_ElecLeakProtector;
    }
    else
    {
        smParam.ulFlags &= ~(1 << DISP_SM_ElecLeakProtector);
    }

    if ((Qt::Checked == m_chCfg[DISP_CFG_TankUV]->checkState()))
    {
        smParam.ulFlags |= 1 << DISP_SM_TankUV;
    }
    else
    {
        smParam.ulFlags &= ~(1 << DISP_SM_TankUV);
    }

    MainSaveSubModuleSetting(gGlobalParam.iMachineType,smParam);

    MainUpdateGlobalParam();

}

void DInitSyscfgpage::on_ExNextBtn_clicked()
{
    save();
    m_wndMain->naviInitPage(Ex_Init_Syscfg,  0);
    m_wndMain->prepareKeyStroke();
}

void DInitSyscfgpage::on_ExBackBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_Syscfg, 1);
    m_wndMain->prepareKeyStroke();
}






