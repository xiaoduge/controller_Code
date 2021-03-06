#include "dlanguagepage.h"
#include "mainwindow.h"

#include "cbitmapbutton.h"
#include "ctrlapplication.h"

#include <QRect>

//QString Country[LanguageNum] =
//{
//    QString(":/pic/English.png"),
//    QString(":/pic/China.png"),
//    QString(":/pic/Spanish.png"),
//    QString(":/pic/France.png"),
//    QString(":/pic/Germany.png"),
//    QString(":/pic/Italian.png"),
//    QString(":/pic/SouthKorea.png")
//};


DLanguagepage::DLanguagepage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iLanguage = gGlobalParam.MiscParam.iLan;

    creatTitle();
    initUi();
    buildTranslation();
}

void DLanguagepage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void DLanguagepage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Language");

    setTitles(stringList);

}

void DLanguagepage::buildTranslation()
{
    m_pCbLan->setItemText(0, tr("English"));
    m_pCbLan->setItemText(1, tr("Chinese"));
    m_pCbLan->setItemText(2, tr("Spanish"));
    m_pCbLan->setItemText(3, tr("French"));
    m_pCbLan->setItemText(4, tr("German"));
    m_pCbLan->setItemText(5, tr("Italian"));
    m_pCbLan->setItemText(6, tr("Korean"));
    m_pCbLan->setItemText(7, tr("Russian"));
    m_pCbLan->setItemText(8, tr("Portugese"));

    m_pBtnSave->setText(tr("Save"));
}

void DLanguagepage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void DLanguagepage::setBackColor()
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

void DLanguagepage::on_saveBtn_clicked()
{
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
    m_wndMain->prepareKeyStroke();
}

void DLanguagepage::on_cbLan_currentIndexChanged(int index)
{
    m_iLanguage = index;
}

void DLanguagepage::initUi()
{
    setBackColor();

    QPixmap worldPix = QPixmap(":/pic/blueWorld.png");

    m_pLbWorld = new QLabel(m_widget);
    m_pLbWorld->setPixmap(worldPix);
    m_pLbWorld->setGeometry(20, 100, worldPix.width(), worldPix.height());

    m_pCbLan = new QComboBox(m_widget);
    QStringList strList;
    strList << tr("English")
            << tr("Chinese")
            << tr("Spanish")
            << tr("French")
            << tr("German")
            << tr("Italian")
            << tr("Korean")
            << tr("Russian")
            << tr("Portugese");

    m_pCbLan->addItems(strList);
    m_pCbLan->setCurrentIndex(m_iLanguage);
    m_pCbLan->setGeometry(570, 105, 160, 40);
    connect(m_pCbLan, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbLan_currentIndexChanged(int)));

    m_pBtnSave = new QPushButton( m_widget);
    m_pBtnSave->setGeometry(570, 180, 160, 40);
    connect(m_pBtnSave, SIGNAL(clicked()), this, SLOT(on_saveBtn_clicked()));
}


void DLanguagepage::leaveSubPage()
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



