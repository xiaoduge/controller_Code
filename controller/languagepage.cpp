#include "languagepage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "cbitmapbutton.h"

#include "ctrlapplication.h"

#include "notify.h"


#include <QRect>

QString Country[LanguageNum] =
{
    QString(":/pic/English.png"),
    QString(":/pic/China.png"),
    QString(":/pic/Spanish.png"),
    QString(":/pic/France.png"),
    QString(":/pic/Germany.png"),
    QString(":/pic/Italian.png"),

};

QString CountryName[LanguageNum] =
{
    QString("English"),
    QString("Chinese"),
    QString("Spanish"),
    QString("French"),
    QString("German"),
    QString("Italian"),

};

QRect backrect[LanguageNum] =
{
    QRect(72 , 163 , 202 , 125),
    QRect(299 , 163 , 202 , 125),
    QRect(526 , 163 , 202 , 125),
    QRect(72 , 320 , 202 , 125),
    QRect(299 , 320 , 202 , 125),
    QRect(526 , 320 , 202 , 125),
};

LanguagePage::LanguagePage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{

    m_iLanguage = gGlobalParam.MiscParam.iLan;

    creatTitle();

    initUi();

    buildTranslation();

}

void LanguagePage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void LanguagePage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Language");

    setTitles(stringList);

}

void LanguagePage::buildTranslation()
{
    int iLoop;

    CountryName[0] = tr("English");
    CountryName[1] = tr("Chinese");
    CountryName[2] = tr("Spanish");
    CountryName[3] = tr("French");
    CountryName[4] = tr("German");
    CountryName[5] = tr("Italian");

    for(iLoop = 0 ; iLoop < LanguageNum ; iLoop++)
    {
        lbStr[iLoop]->setText(CountryName[iLoop]);
    } 
}

void LanguagePage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void LanguagePage::setBackColor()
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

void LanguagePage::initUi()
{
    setBackColor();

    QPixmap *LanguagePix = new QPixmap(":/pic/Language.png");

    for(int iLoop = 0 ; iLoop < LanguageNum ; iLoop++)
    {
        QPixmap tmpPix;
        tmpPix.load(Country[iLoop]);

        btnLanguage[iLoop] = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_SINGLE,iLoop);

        btnLanguage[iLoop]->setButtonPicture(LanguagePix);

        btnLanguage[iLoop]->setGeometry(backrect[iLoop]);

        btnLanguage[iLoop]->setStyleSheet("background-color:transparent");

        connect(btnLanguage[iLoop], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

        lbLanguage[iLoop] = new QLabel(btnLanguage[iLoop]);

        lbLanguage[iLoop]->setPixmap(tmpPix);
        lbLanguage[iLoop]->setGeometry(QRect(76,28,50,32));

        lbStr[iLoop] = new QLabel(btnLanguage[iLoop]);
        lbStr[iLoop]->setText(CountryName[iLoop]);
        lbStr[iLoop]->setGeometry(QRect(50,70,100,30));
        lbStr[iLoop]->setAlignment(Qt::AlignCenter);
        lbStr[iLoop]->setStyleSheet(" font-size:18pt;color:#3f4a56;font-family:Arial;");
    }
}

void LanguagePage::on_btn_clicked(int index)
{
   m_iLanguage = index;
   
   if (m_iLanguage != gGlobalParam.MiscParam.iLan)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.iLan = m_iLanguage;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       gApp->installTranslators(m_iLanguage);
       
       m_wndMain->switchLanguage();
       
       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_LANGUAGE);

       show(false);
       m_parent->show(true);
    }
}

void LanguagePage::leaveSubPage()
{
    if (m_iLanguage != gGlobalParam.MiscParam.iLan)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;  
   
       MiscParam.iLan = m_iLanguage;

       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       gApp->installTranslators(m_iLanguage);
       
       m_wndMain->switchLanguage();
    }
    
    CSubPage::leaveSubPage();
}


