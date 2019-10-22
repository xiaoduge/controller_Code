#include "ex_permissionsetpage.h"
#include "mainwindow.h"
#include "Display.h"
#include "ex_hintdialog.h"

#include <QFile>
#include <QRadioButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QHeaderView>
#include "dlineedit.h"

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
    m_tabWidget->setTabText(1, tr("Verification"));

#ifdef SUB_ACCOUNT
    m_tabWidget->setTabText(2, tr("Sub-Account"));
    m_chSubAccount->setText(tr("Sub-Account"));
    m_pNameLB->setText(tr("Name"));
    m_pQueryBtn->setText(tr("Query"));
    m_pDisplayAllBtn->setText(tr("Display All"));
    m_pDeleteOneBtn->setText(tr("Reset"));
    m_pDeleteAllBtn->setText(tr("Reset All"));
#endif

    m_pRfidTitle->setText(tr("RFID Config"));
    m_chRfid->setText(tr("Disable RFID"));
    m_saveRfidBtn->setText(tr("Save"));

    m_pPermissionTitle->setText(tr("Verification"));
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

#ifdef SUB_ACCOUNT
    initSubAccountPage();
#endif
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
    if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
    {
        m_chPermission->setChecked(true);
    }
    else
    {
        m_chPermission->setChecked(false);
    }
#ifdef SUB_ACCOUNT
    //2019.10.15 add, for sub-account
    if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_SUB_ACCOUNT))
    {
        m_chSubAccount->setChecked(true);
    }
    else
    {
        m_chSubAccount->setChecked(false);
    }
#endif
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
        miscParam.ulMisFlags |= 1 << DISP_SM_User_Authorization;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_User_Authorization);
    }

    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_INSTALL_PERMISSION);
    m_wndMain->prepareKeyStroke();
    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_PermissionSetPage::on_chSubAccount_stateChanged(int state)
{
    DISP_MISC_SETTING_STRU  miscParam = gGlobalParam.MiscParam;
    if ((Qt::Checked == state))
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_SUB_ACCOUNT;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_SUB_ACCOUNT);
    }

    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_INSTALL_PERMISSION);
    m_wndMain->prepareKeyStroke();
//    Ex_HintDialog::getInstance(tr("Successfully saved"));
}

void Ex_PermissionSetPage::on_QueryBtn_clicked()
{
    QString strName = m_pNameLineEdit->text();
    m_pTableModel->setFilter(QString("name='%1'").arg(strName));
    m_pTableModel->select();
    initTabelHeaderData();// ex
    m_pTableView->hideColumn(0);
    m_wndMain->prepareKeyStroke();
}

void Ex_PermissionSetPage::on_DisplayAllBtn_clicked()
{
    m_pTableModel->setTable("SubAccount");
    m_pTableModel->select();
    initTabelHeaderData();// ex
    m_pTableView->hideColumn(0);
    m_wndMain->prepareKeyStroke();
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
    m_tabWidget->addTab(m_pageWidget[PAGE_PERMISSION], icon1, tr("Verification"));
}

void Ex_PermissionSetPage::initSubAccountPage()
{
    m_pageWidget[PAGE_SUBACCOUNT] = new QWidget;

    QString strQss4Chk = m_wndMain->getQss4Chk();

    m_chSubAccount = new QCheckBox;
    m_chSubAccount->setStyleSheet(strQss4Chk);

    m_pTableView = new QTableView;
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pNameLB = new QLabel;
    m_pNameLineEdit = new DLineEdit;
    m_pQueryBtn = new QPushButton;
    m_pDisplayAllBtn = new QPushButton;
    m_pDeleteOneBtn = new QPushButton;
    m_pDeleteAllBtn = new QPushButton;

    QHBoxLayout* h0Layout = new QHBoxLayout;
    QHBoxLayout* h1Layout = new QHBoxLayout;
    QHBoxLayout* h2Layout = new QHBoxLayout;
    QHBoxLayout* h3Layout = new QHBoxLayout;

    h0Layout->setAlignment(Qt::AlignLeft);
    h0Layout->addWidget(m_chSubAccount);

    h1Layout->addWidget(m_pTableView);

    m_pNameLineEdit->setMaximumWidth(250);
    m_pNameLineEdit->setMinimumWidth(200);
    m_pQueryBtn->setMinimumWidth(150);
    m_pDisplayAllBtn->setMinimumWidth(150);
    h2Layout->addWidget(m_pNameLB);
    h2Layout->addWidget(m_pNameLineEdit);
    h2Layout->addStretch(1);
    h2Layout->addWidget(m_pQueryBtn);
    h2Layout->addWidget(m_pDisplayAllBtn);

    h3Layout->setAlignment(Qt::AlignRight);
    m_pDeleteOneBtn->setMinimumWidth(150);
    m_pDeleteAllBtn->setMinimumWidth(150);
    h3Layout->addWidget(m_pDeleteOneBtn);
    h3Layout->addWidget(m_pDeleteAllBtn);

    QVBoxLayout* vLayout = new QVBoxLayout;

    vLayout->addLayout(h0Layout);
    vLayout->addLayout(h1Layout);
    vLayout->addLayout(h2Layout);
    vLayout->addLayout(h3Layout);

    m_pageWidget[PAGE_SUBACCOUNT]->setLayout(vLayout);

    connect(m_chSubAccount, SIGNAL(stateChanged(int)),
            this, SLOT(on_chSubAccount_stateChanged(int)));
    connect(m_pQueryBtn, SIGNAL(clicked()), this, SLOT(on_QueryBtn_clicked()));
    connect(m_pDisplayAllBtn, SIGNAL(clicked()), this, SLOT(on_DisplayAllBtn_clicked()));

    connect(m_pDeleteOneBtn, SIGNAL(clicked()), this, SLOT(on_DeleteOneBtn_clicked()));
    connect(m_pDeleteAllBtn, SIGNAL(clicked()), this, SLOT(on_DeleteAllBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[PAGE_SUBACCOUNT], icon1, tr("Verification"));

    initDBTabelModel();
}

void Ex_PermissionSetPage::initDBTabelModel()
{
    m_pTableModel = new QSqlTableModel(m_widget);
    m_pTableModel->setTable("SubAccount");
    m_pTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pTableModel->select();
    initTabelHeaderData();// ex
    m_pTableView->setModel(m_pTableModel);

    m_pTableView->verticalHeader()->setDefaultSectionSize(50);
    m_pTableView->verticalHeader()->setMinimumSectionSize(50);
    m_pTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    m_pTableView->hideColumn(0);
}

void Ex_PermissionSetPage::initTabelHeaderData()
{
    m_pTableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pTableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_pTableModel->setHeaderData(2, Qt::Horizontal, tr("UP Quantity"));
    m_pTableModel->setHeaderData(3, Qt::Horizontal, tr("HP Quantity"));
}

void Ex_PermissionSetPage::on_DeleteOneBtn_clicked()
{
    QString strName = m_pNameLineEdit->text();
    QString strSql = QString("delete from SubAccount where name='%1'").arg(strName);
    QSqlQuery query;
    query.exec(strSql);

    m_wndMain->prepareKeyStroke();
}

void Ex_PermissionSetPage::on_DeleteAllBtn_clicked()
{
    QString strSql = QString("delete from SubAccount");
    QSqlQuery query;
    query.exec(strSql);

    m_wndMain->prepareKeyStroke();
}





