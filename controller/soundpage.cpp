#include "soundpage.h"


#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include <QRect>

SoundPage::SoundPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iSoundMask = gGlobalParam.MiscParam.iSoundMask;

    creatTitle();
    
    initUi();
    
    buildTranslation();

}

void SoundPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SoundPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Audio");

    setTitles(stringList);

}

void SoundPage::buildTranslation()
{
    int iLoop ;

    m_strSounds[0] = tr("Touch-tone");
    m_strSounds[1] = tr("Audio Alerts");
    m_strSounds[2] = tr("Audio Alarms");

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_lblNames[iLoop]->setText(m_strSounds[iLoop]);
    }
    
    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
}

void SoundPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}



void SoundPage::setBackColor()
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

void SoundPage::initUi()
{
    int iLoop ;

    setBackColor();
    
    m_strQss4Chk = m_wndMain->getQss4Chk();

    QPixmap back(":/pic/SubPageBack.png");

    QSize size(back.width(), back.height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_pBackWidget[iLoop] = new QWidget(m_widget);

        QPalette pal(m_pBackWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::gray);

        m_pBackWidget[iLoop]->setAutoFillBackground(true);
        m_pBackWidget[iLoop]->setPalette(pal);

        m_pBackWidget[iLoop]->setGeometry(QRect(134 , 190 + 70 * iLoop , 530 ,60));

        m_lblNames[iLoop] = new QLabel(m_pBackWidget[iLoop]);
        m_lblNames[iLoop]->setPixmap(NULL);
        m_lblNames[iLoop]->setGeometry(QRect(25, 30 , 100 , 20));
        m_lblNames[iLoop]->setText("Sound");
        m_lblNames[iLoop]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_lblNames[iLoop]->show();
        m_lblNames[iLoop]->setAlignment(Qt::AlignLeft);

        m_chkSwitchs[iLoop] = new QCheckBox(m_pBackWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(480 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(m_strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        if (m_iSoundMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));

    }
    
    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SOUNDPAGE_BTN_SAVE);
    
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    
    m_pBtnSave->setStyleSheet("background-color:transparent");
    
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    
    m_pBtnSave->show();

}

void SoundPage::save()
{
    
    if (m_iSoundMask != gGlobalParam.MiscParam.iSoundMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.iSoundMask = m_iSoundMask;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
       
       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_SOUND);
       
    }
    
    show(false);
    m_parent->show(true);

}

void SoundPage::on_btn_clicked(int index)
{
   qDebug()<<index;
   
   switch(index)
   {
   case SOUNDPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }
   
   m_wndMain->prepareKeyStroke();
}



void SoundPage::on_checkBox_changeState(int state)
{
    int iLoop;
    
    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }
    
    //int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;


    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
       if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
       {
           m_iSoundMask |= 1 << iLoop;
       }
       else
       {
           m_iSoundMask &= ~(1 << iLoop);
       }
    }

}

void SoundPage::leaveSubPage()
{    
    CSubPage::leaveSubPage();
}

