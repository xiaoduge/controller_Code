#include "drfidcfgpage.h"
#include "mainwindow.h"
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>


DRFIDCfgPage::DRFIDCfgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void DRFIDCfgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void DRFIDCfgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("RFID Config");

    setTitles(stringList);

}

void DRFIDCfgPage::buildTranslation()
{
    m_pExLbTitle->setText(tr("RFID Config"));
    m_chRfid->setText(tr("Disable RFID"));
    m_saveBtn->setText(tr("Save"));

}

void DRFIDCfgPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void DRFIDCfgPage::setBackColor()
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

void DRFIDCfgPage::createControl()
{
    QString strQss4Chk = m_wndMain->getQss4Chk();

    QWidget *tmpWidget = NULL;

    tmpWidget = new QWidget(m_widget);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(134 , 190 , 530 ,60));

    m_chRfid = new QCheckBox(tmpWidget);
    m_chRfid->setStyleSheet(strQss4Chk);


    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->addWidget(m_chRfid);
    tmpWidget->setLayout(line1Layout);

    m_saveBtn = new QPushButton(m_widget);
    connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(on_saveBtn_clicked()));
    m_saveBtn->move(550, 450);
}


void DRFIDCfgPage::initUi()
{
    setBackColor();
    createHeader();
    createControl();
}

void DRFIDCfgPage::update()
{
    connectData();
}

void DRFIDCfgPage::createHeader()
{
    m_pExLbTitle = new QLabel(m_widget);
    m_pExLbTitle->setGeometry(QRect(134, 135 , 220 , 28));
    m_pExLbTitle->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");
    m_pExLbTitle->hide();
}


void DRFIDCfgPage::connectData()
{
    if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
    {
        m_chRfid->setChecked(true);
    }
    else
    {
        m_chRfid->setChecked(false);
    }
}

void DRFIDCfgPage::on_saveBtn_clicked()
{
    save();
}

void DRFIDCfgPage::save()
{
   DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;
   if ((Qt::Checked == m_chRfid->checkState()))
   {
       miscParam.ulMisFlags |= 1 << DISP_SM_RFID_Authorization;
   }
   else
   {
       miscParam.ulMisFlags &= ~(1 << DISP_SM_RFID_Authorization);
   }

   MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
   MainUpdateGlobalParam();

   m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_RFID_CONFIG);

   show(false);
   m_parent->show(true);

}








