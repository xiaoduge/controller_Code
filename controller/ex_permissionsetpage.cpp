#include "ex_permissionsetpage.h"
#include "mainwindow.h"
#include "Display.h"
#include "ex_hintdialog.h"

#include <QDebug>
#include <QPainter>
#include <QRadioButton>


Ex_PermissionSetPage::Ex_PermissionSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void Ex_PermissionSetPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_PermissionSetPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Permission");
    setTitles(stringList);

}

void Ex_PermissionSetPage::buildTranslation()
{
    m_tabWidget->setTabText(0, tr("RFID Config"));
    m_tabWidget->setTabText(1, tr("Permission"));

    m_pRfidTitle->setText(tr("RFID Config"));
    m_chRfid->setText(tr("Disable RFID"));
    m_saveRfidBtn->setText(tr("Save"));

    m_pPermissionTitle->setText(tr("Permission"));
    m_chPermission->setText(tr("Users is free of verification."));
    m_savePermissionBtn->setText(tr("Save"));
}

void Ex_PermissionSetPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_PermissionSetPage::setBackColor()
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


void Ex_PermissionSetPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;

    //add page
    initRFIDConfigPage();
    initInstallPermissionPage();
    //end

    mainLayout->addWidget(m_tabWidget, 0, 0);
    m_mainWidget->setLayout(mainLayout);

    QFile qss(":/app/tabWidget.qss");
    qss.open(QFile::ReadOnly);
    QString tabWidgetqss = QLatin1String (qss.readAll());
    qss.close();

    m_tabWidget->setStyleSheet(tabWidgetqss);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);

}

void Ex_PermissionSetPage::update()
{
    //RFID
    if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
    {
        m_chRfid->setChecked(true);
    }
    else
    {
        m_chRfid->setChecked(false);
    }
    //Consumable install Permission
    if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_ConsumableInstall_Authorization))
    {
        m_chPermission->setChecked(true);
    }
    else
    {
        m_chPermission->setChecked(false);
    }
}

void Ex_PermissionSetPage::on_RfidsaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU  miscParam = gGlobalParam.MiscParam;
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
    m_wndMain->prepareKeyStroke();

    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_PermissionSetPage::on_PermissionsaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU  miscParam = gGlobalParam.MiscParam;
    if ((Qt::Checked == m_chPermission->checkState()))
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_ConsumableInstall_Authorization;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_ConsumableInstall_Authorization);
    }

    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_INSTALL_PERMISSION);
    m_wndMain->prepareKeyStroke();
    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_PermissionSetPage::initRFIDConfigPage()
{
    m_pageWidget[PAGE_RFID] = new QWidget;

    //create header
    m_pRfidTitle = new QLabel(m_pageWidget[PAGE_RFID]);
    m_pRfidTitle->setGeometry(QRect(134, 135 , 220 , 28));
    m_pRfidTitle->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");
    m_pRfidTitle->hide();

    QString strQss4Chk = m_wndMain->getQss4Chk();

    QWidget *tmpWidget = new QWidget(m_pageWidget[PAGE_RFID]);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(134 , 120 , 530 ,60));

    m_chRfid = new QCheckBox(tmpWidget);
    m_chRfid->setStyleSheet(strQss4Chk);

    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->addWidget(m_chRfid);
    tmpWidget->setLayout(line1Layout);

    m_saveRfidBtn = new QPushButton(m_pageWidget[PAGE_RFID]);
    connect(m_saveRfidBtn, SIGNAL(clicked()), this, SLOT(on_RfidsaveBtn_clicked()));
    m_saveRfidBtn->move(550, 350);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[PAGE_RFID], icon1, tr("RFID Config"));
}

void Ex_PermissionSetPage::initInstallPermissionPage()
{
    m_pageWidget[PAGE_PERMISSION] = new QWidget;

    //create header
    m_pPermissionTitle = new QLabel(m_pageWidget[PAGE_PERMISSION]);
    m_pPermissionTitle->setGeometry(QRect(134, 135 , 220 , 28));
    m_pPermissionTitle->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");
    m_pPermissionTitle->hide();

    QString strQss4Chk = m_wndMain->getQss4Chk();

    QWidget *tmpWidget = new QWidget(m_pageWidget[PAGE_PERMISSION]);
    QPalette pal(tmpWidget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));

    tmpWidget->setAutoFillBackground(true);
    tmpWidget->setPalette(pal);
    tmpWidget->setGeometry(QRect(134 , 120 , 530 ,60));

    m_chPermission = new QCheckBox(tmpWidget);
    m_chPermission->setStyleSheet(strQss4Chk);

    QHBoxLayout* line1Layout = new QHBoxLayout;
    line1Layout->addWidget(m_chPermission);
    tmpWidget->setLayout(line1Layout);

    m_savePermissionBtn = new QPushButton(m_pageWidget[PAGE_PERMISSION]);
    connect(m_savePermissionBtn, SIGNAL(clicked()), this, SLOT(on_PermissionsaveBtn_clicked()));
    m_savePermissionBtn->move(550, 350);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[PAGE_PERMISSION], icon1, tr("Permission"));
}





