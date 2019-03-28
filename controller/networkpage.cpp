#include "networkpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "cbitmapbutton.h"

#include <QRect>



NetworkPage::NetworkPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;

    creatTitle();

    initUi();

    buildTranslation();

}

void NetworkPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void NetworkPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Connectivity");

    setTitles(stringList);

}

void NetworkPage::buildTranslation()
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
}

void NetworkPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void NetworkPage::setBackColor()
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

void NetworkPage::initUi()
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

//        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 160 + 70 * iLoop , 530 ,60));
        m_pBackWidget[iLoop]->setGeometry(QRect(124 , 160 + 80 * (iLoop - 1) , 530 ,60));

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

        //2019.3.14 add
        if(DISPLAY_NETWORK_CAN == iLoop)
        {
            m_pBackWidget[iLoop]->hide();
        }
    }
    
    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,NETWORKPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}

void NetworkPage::save()
{
    
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.iNetworkMask = m_iNetworkMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
       
    }
    
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_NETWORK);
    
    show(false);
    m_parent->show(true);

}

void NetworkPage::on_btn_clicked(int index)
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

void NetworkPage::on_checkBox_changeState(int state)
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


void NetworkPage::leaveSubPage()
{    
    CSubPage::leaveSubPage();
}


