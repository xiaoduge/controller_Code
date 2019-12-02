#include "dinitlanguagepage.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "ctrlapplication.h"
#include <QRect>
#include <QPushButton>
#include <QComboBox>

DInitLanguagepage::DInitLanguagepage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_iLanguage = gGlobalParam.MiscParam.iLan;

    creatTitle();

    initUi();

    buildTranslation();

    this->hideTitleBar();

}

void DInitLanguagepage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void DInitLanguagepage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Language");

    setTitles(stringList);

}

void DInitLanguagepage::buildTranslation()
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

//    m_pBtnSave->setText(tr("Save"));
    m_nextBtn->setText(tr("Next"));
}

void DInitLanguagepage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void DInitLanguagepage::setBackColor()
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

void DInitLanguagepage::initUi()
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
            << tr("Russian")
            << tr("Portugese");
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

void DInitLanguagepage::on_nextBtn_clicked()
{
    on_saveBtn_clicked(); //next clicked save
    m_wndMain->naviInitPage(Ex_Init_Lan, 0);
    m_wndMain->prepareKeyStroke();
}

void DInitLanguagepage::leaveSubPage()
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

void DInitLanguagepage::mousePressEvent(QMouseEvent *e)
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

void DInitLanguagepage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void DInitLanguagepage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        on_saveBtn_clicked();
        m_wndMain->naviInitPage(Ex_Init_Lan, m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}

void DInitLanguagepage::on_saveBtn_clicked()
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

void DInitLanguagepage::on_cbLan_currentIndexChanged(int index)
{
    m_iLanguage = index;

    gApp->installTranslators(m_iLanguage);
    m_wndMain->switchLanguage();
}


