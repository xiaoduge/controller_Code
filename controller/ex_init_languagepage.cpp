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
#include <QComboBox>



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
    m_pCbLan->setItemText(0, tr("English"));
    m_pCbLan->setItemText(1, tr("Chinese"));
    m_pCbLan->setItemText(2, tr("Spanish"));
    m_pCbLan->setItemText(3, tr("French"));
    m_pCbLan->setItemText(4, tr("German"));
    m_pCbLan->setItemText(5, tr("Italian"));
    m_pCbLan->setItemText(6, tr("Korean"));
    m_pCbLan->setItemText(7, tr("Russian"));

//    m_pBtnSave->setText(tr("Save"));
    m_nextBtn->setText(tr("Next"));
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

    QPixmap worldPix = QPixmap(":/pic/blueWorld.png");

    m_pLbWorld = new QLabel(m_widget);
    m_pLbWorld->setPixmap(worldPix);
    m_pLbWorld->setGeometry(20, 90, worldPix.width(), worldPix.height());

    m_pCbLan = new QComboBox(m_widget);
    QStringList strList;
    strList << tr("English")
            << tr("Chinese")
            << tr("Spanish")
            << tr("French")
            << tr("German")
            << tr("Italian")
            << tr("Korean")
            << tr("Russian");
    m_pCbLan->addItems(strList);
    m_pCbLan->setCurrentIndex(m_iLanguage);
    m_pCbLan->setGeometry(570, 105, 160, 40);
    connect(m_pCbLan, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbLan_currentIndexChanged(int)));

//    m_pBtnSave = new QPushButton( m_widget);
//    m_pBtnSave->setGeometry(560, 180, 160, 40);
//    connect(m_pBtnSave, SIGNAL(clicked()), this, SLOT(on_saveBtn_clicked()));
//    m_pBtnSave->hide();

    m_nextBtn = new QPushButton(m_widget);
    m_nextBtn->setGeometry(570, 180, 160, 40);
    connect(m_nextBtn, SIGNAL(clicked()), this, SLOT(on_nextBtn_clicked()));

}

void Ex_Init_Languagepage::on_nextBtn_clicked()
{
    on_saveBtn_clicked(); //next clicked save
    emit languageSwitchBtnClicked(1);

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

void Ex_Init_Languagepage::on_cbLan_currentIndexChanged(int index)
{
    m_iLanguage = index;

    gApp->installTranslators(m_iLanguage);
    m_wndMain->switchLanguage();
}


