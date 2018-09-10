#include "ex_init_networkpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "cbitmapbutton.h"

#include <QRect>



Ex_Init_Networkpage::Ex_Init_Networkpage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;
    creatTitle();
    initUi();
    buildTranslation();
    this->hideTitleBar();

}

void Ex_Init_Networkpage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_Init_Networkpage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Connectivity");
    setTitles(stringList);

}

void Ex_Init_Networkpage::buildTranslation()
{
    int iLoop;
    m_astrNetName[0] = tr("CAN");
    m_astrNetName[1] = tr("Zigbee");
    m_astrNetName[2] = tr("WIFI");

    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
        m_laName[iLoop]->setText(m_astrNetName[iLoop]);
    }
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
    m_pExBackBtn->setText(tr("Back"));
    m_pExNextBtn->setText(tr("Next"));
}

void Ex_Init_Networkpage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_Init_Networkpage::setBackColor()
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

void Ex_Init_Networkpage::initUi()
{
    int iLoop;

    m_strQss4Chk = m_wndMain->getQss4Chk();

    setBackColor();

    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
        m_pBackWidget[iLoop] = new QWidget(m_widget);

        QPalette pal(m_pBackWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::gray);

        m_pBackWidget[iLoop]->setAutoFillBackground(true);
        m_pBackWidget[iLoop]->setPalette(pal);

        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 160 + 70 * iLoop , 530 ,60));

        m_laName[iLoop]      = new QLabel(m_pBackWidget[iLoop]);
        m_laName[iLoop]->setPixmap(NULL);
        m_laName[iLoop]->setGeometry(QRect(25, 30 , 100 , 20));
        m_laName[iLoop]->show();

        m_chkSwitchs[iLoop] = new QCheckBox(m_pBackWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(480 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(m_strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        if (m_iNetworkMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));

    }

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,NETWORKPAGE_BTN_SAVE);

    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);

    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    //700,560,
    m_pBtnSave->setGeometry(350,400,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());

    m_pBtnSave->setStyleSheet("background-color:transparent");

    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

//    m_pBtnSave->show();
    m_pBtnSave->hide();

    m_pExBackBtn = new QPushButton(m_widget);
    m_pExNextBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_m_pExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_m_pExBackBtn_clicked()));

    m_pExBackBtn->move(200, 450);
    m_pExNextBtn->move(500, 450);

}

void Ex_Init_Networkpage::save()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    }

}

void Ex_Init_Networkpage::on_btn_clicked(int index)
{

   switch(index)
   {
   case NETWORKPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }

   m_wndMain->prepareKeyStroke();
}

void Ex_Init_Networkpage::on_checkBox_changeState(int state)
{
    (void)state;
    int iLoop;

    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }

    //int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;


    for(iLoop = 0 ; iLoop < DISPLAY_NETWORK_NUM ; iLoop++)
    {
       if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
       {
           m_iNetworkMask |= 1 << iLoop;
       }
       else
       {
           m_iNetworkMask &= ~(1 << iLoop);
       }
    }

}

void Ex_Init_Networkpage::on_m_pExNextBtn_clicked()
{
    save(); //next clicked save

    emit networkSwitchBtnClicked(1);

}

void Ex_Init_Networkpage::on_m_pExBackBtn_clicked()
{
    emit networkSwitchBtnClicked(0);
}

void Ex_Init_Networkpage::leaveSubPage()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    }
    CSubPage::leaveSubPage();
}



