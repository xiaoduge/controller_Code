#include "ex_init_languagepage.h"

#include "titlebar.h"
#include "mainwindow.h"
#include <QPainter>
#include <QScrollBar>
#include <QListWidgetItem>
#include "cbitmapbutton.h"
#include "ctrlapplication.h"
#include "notify.h"
#include <QRect>
#include <QPushButton>



Ex_Init_Languagepage::Ex_Init_Languagepage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{

    m_iLanguage = gGlobalParam.MiscParam.iLan;

    creatTitle();

    initUi();

    buildTranslation();

    this->hideTitleBar();

}

void Ex_Init_Languagepage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_Init_Languagepage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Language");

    setTitles(stringList);

}

void Ex_Init_Languagepage::buildTranslation()
{
    int iLoop;

    CountryName[0] = tr("English");
    CountryName[1] = tr("Chinese");
    CountryName[2] = tr("Spanish");
    CountryName[3] = tr("French");
    CountryName[4] = tr("German");
    CountryName[5] = tr("Italian");

    for(iLoop = 0 ; iLoop < Ex_LanguageNum ; iLoop++)
    {
        lbStr[iLoop]->setText(CountryName[iLoop]);
    }
    m_nextBtn->setText(tr("Next"));
    m_saveBtn->setText(tr("Save"));
}

void Ex_Init_Languagepage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void Ex_Init_Languagepage::setBackColor()
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

void Ex_Init_Languagepage::initUi()
{
    setBackColor();

    QPixmap *LanguagePix = new QPixmap(":/pic/Language.png");

    for(int iLoop = 0 ; iLoop < Ex_LanguageNum ; iLoop++)
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
    m_nextBtn = new QPushButton(m_widget);
    m_saveBtn = new QPushButton(m_widget);
    connect(m_nextBtn, SIGNAL(clicked()), this, SLOT(on_nextBtn_clicked()));
    connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(on_saveBtn_clicked()));

    m_saveBtn->hide(); //hide
    m_saveBtn->move(200, 477);
    m_nextBtn->move(500, 477);

}

void Ex_Init_Languagepage::on_btn_clicked(int index)
{
   m_iLanguage = index;

   //ex
   gApp->installTranslators(m_iLanguage);
   m_wndMain->switchLanguage();
   //end
}

void Ex_Init_Languagepage::on_nextBtn_clicked()
{
    on_saveBtn_clicked(); //next clicked save
    emit languageSwitchBtnClicked(1);

}

void Ex_Init_Languagepage::on_saveBtn_clicked()
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
}

void Ex_Init_Languagepage::leaveSubPage()
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


