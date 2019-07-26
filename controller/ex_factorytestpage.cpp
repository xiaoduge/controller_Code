#include "ex_factorytestpage.h"
#include "titlebar.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include "Ex_Display_c.h"
#include "ex_hintdialog.h"
#include <QPainter>
#include <QMessageBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QDir>
#include <QTextBrowser>
#include <QProcess>
#include "dlineedit.h"


Ex_FactoryTestPage::Ex_FactoryTestPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain)
                                   : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
    isFlow = false;
    isPressure = false;
}

void Ex_FactoryTestPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_FactoryTestPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Factory Test");
    setTitles(stringList);
}

void Ex_FactoryTestPage::setBackColor()
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

void Ex_FactoryTestPage::buildTranslation()
{
    int i;
    m_tabWidget->setTabText(0, tr("RFID"));
    m_tabWidget->setTabText(1, tr("Flow & Pressure"));
    m_tabWidget->setTabText(2, tr("Wifi Test"));
    m_tabWidget->setTabText(3, tr("Maintenance"));

    m_pConfigLabel[CONFIG_CAT]->setText(tr("Cat No.:"));
    m_pConfigLabel[CONFIG_LOT]->setText(tr("Lot No.:"));
    m_pConfigLabel[CONFIG_INSTALLDATE]->setText(tr("Installation Date:"));
    m_pConfigLabel[CONFIG_VOLUMEOFUSE]->setText(tr("Volume of Use:"));
    m_pIndexLabel->setText(tr("Address:"));

    m_pWriteBtn->setText(tr("Write"));
    m_pReadBtn->setText(tr("Read"));
    m_pClearBtn->setText(tr("Clear"));

    m_pAreaLab[FTESTPAGE_FLOW]->setText(tr("Flow Test"));
    m_pAreaLab[FTESTPAGE_FLOW]->setAlignment(Qt::AlignCenter);
    m_pFlowDisplayLab[S1_DISPLAY]->setText(tr("S1"));
    m_pFlowDisplayLab[S2_DISPLAY]->setText(tr("S2"));
    m_pFlowDisplayLab[S3_DISPLAY]->setText(tr("S3"));
    m_pFlowDisplayLab[S4_DISPLAY]->setText(tr("S4"));
    for(i = 0; i < FLOW_DISPLAY_NUM; i++)
    {
        m_pFlowDisplayLab[i]->setAlignment(Qt::AlignCenter);
    }
    m_pBtn[FTESTPAGE_FLOW]->setText(tr("Start"));


    m_pAreaLab[FTESTPAGE_ILOOP]->setText(tr("Pressure Test"));
    m_pAreaLab[FTESTPAGE_ILOOP]->setAlignment(Qt::AlignCenter);
    m_pPreDisplayLab[P1_DISPLAY]->setText(tr("Work Pres."));
    m_pPreDisplayLab[P2_DISPLAY]->setText(tr("Pure Tank Level"));
    m_pPreDisplayLab[P3_DISPLAY]->setText(tr("Source Tank Level"));
    for(i = 0; i < PRESSURE_DISPLAY_NUM; i++)
    {
        m_pPreDisplayLab[i]->setAlignment(Qt::AlignCenter);
    }

    m_pBtn[FTESTPAGE_ILOOP]->setText(tr("Start"));

    m_pClearWifiMsgBtn->setText(tr("Clear"));

    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));

}

void Ex_FactoryTestPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_FactoryTestPage::initFlowTestPage()
{
    int i;
    m_pageWidget[FACTORY_PAGE_FLOW] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_FLOW]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_FLOW]->setStyleSheet(qss);

    for(i = 0; i < FTESTPAGE_NUM; i++)
    {
        m_pFrame[i] = new QFrame(m_pageWidget[FACTORY_PAGE_FLOW]);
        m_pBtn[i] = new QPushButton;
        m_pBtn[i]->setFixedWidth(80);
        m_pAreaLab[i] = new QLabel;
    }

    for(i = 0; i < FLOW_DISPLAY_NUM; i++)
    {
        m_pFlowDisplayLE[i] = new DLineEdit;
        m_pFlowDisplayLE[i]->setReadOnly(true);
        m_pFlowDisplayLab[i] = new QLabel;
    }

    for(i = 0; i <  PRESSURE_DISPLAY_NUM; i++)
    {
        m_pPreDisplayLE[i] = new DLineEdit;
        m_pPreDisplayLE[i]->setReadOnly(true);
        m_pPreDisplayLab[i] = new QLabel;
    }
    m_pFrame[FTESTPAGE_FLOW]->setGeometry(10, 60, 770, 120);
    m_pFrame[FTESTPAGE_ILOOP]->setGeometry(10, 220, 770, 120);
   //    m_pFrame[FTESTPAGE_KEY]->setGeometry(0, 340, 800, 120);

    qss = ".QFrame{ background-color: white;\
                     border: 2px solid; \
                     border-color: rgb(135,206,250);\
                     border-radius: 4px; \
                     padding: 2px;\
                   }";

    m_pFrame[FTESTPAGE_FLOW]->setStyleSheet(qss);
    m_pFrame[FTESTPAGE_ILOOP]->setStyleSheet(qss);

    QGridLayout* gLayout_Flow = new QGridLayout;
    gLayout_Flow->addWidget(m_pAreaLab[FTESTPAGE_FLOW], 0, 0, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S1_DISPLAY], 0, 1, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S2_DISPLAY], 0, 2, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S3_DISPLAY], 0, 3, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S4_DISPLAY], 0, 4, 1, 1);

    gLayout_Flow->addWidget(m_pBtn[FTESTPAGE_FLOW], 1, 0, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S1_DISPLAY], 1, 1, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S2_DISPLAY], 1, 2, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S3_DISPLAY], 1, 3, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S4_DISPLAY], 1, 4, 1, 1);

    gLayout_Flow->setHorizontalSpacing(50);
    gLayout_Flow->setVerticalSpacing(30);
    m_pFrame[FTESTPAGE_FLOW]->setLayout(gLayout_Flow);

    QGridLayout* gLayout_ILoop = new QGridLayout;
    gLayout_ILoop->addWidget(m_pAreaLab[FTESTPAGE_ILOOP], 0, 0, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P1_DISPLAY], 0, 1, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P2_DISPLAY], 0, 2, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P3_DISPLAY], 0, 3, 1, 1);


    gLayout_ILoop->addWidget(m_pBtn[FTESTPAGE_ILOOP], 1, 0, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P1_DISPLAY], 1, 1, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P2_DISPLAY], 1, 2, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P3_DISPLAY], 1, 3, 1, 1);

    gLayout_ILoop->setHorizontalSpacing(60);
    gLayout_ILoop->setVerticalSpacing(30);
    m_pFrame[FTESTPAGE_ILOOP]->setLayout(gLayout_ILoop);

    connect(m_pBtn[FTESTPAGE_FLOW], SIGNAL(clicked()), this, SLOT(on_flowBtn_clicked()));
    connect(m_pBtn[FTESTPAGE_ILOOP], SIGNAL(clicked()), this, SLOT(on_iLoopBtn_clicked()));
    //    connect(m_pBtn[FTESTPAGE_KEY], SIGNAL(clicked()), this, SLOT(on_keyBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_FLOW], icon1, tr("Flow & Pressure"));

}

void Ex_FactoryTestPage::initRFIDTestPage()
{
    m_pageWidget[FACTORY_PAGE_RFID] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_RFID]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_RFID]->setStyleSheet(qss);

    QFormLayout* fLayout = new QFormLayout;
    fLayout->setAlignment(Qt::AlignCenter);
    fLayout->setVerticalSpacing(8);

    m_pIndexLabel = new QLabel;
    m_pIndexCombo = new QComboBox;
    m_pIndexCombo->addItem(tr("20"));
    m_pIndexCombo->addItem(tr("21"));
    m_pIndexCombo->addItem(tr("22"));
    m_pIndexCombo->addItem(tr("23"));
    m_pIndexCombo->addItem(tr("24"));
    m_pIndexCombo->setMinimumWidth(100);
    fLayout->addRow(m_pIndexLabel, m_pIndexCombo);

    for(int i = 0; i < CONFIG_NUM; i++)
    {
        m_pConfigLabel[i] = new QLabel;
        m_pConfigLineEdit[i] = new DLineEdit;
//        m_pConfigLineEdit[i]->setMaximumWidth(180);
        fLayout->addRow(m_pConfigLabel[i], m_pConfigLineEdit[i]);
    }

//    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setReadOnly(true);
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setInputMask("0000-00-00");

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setAlignment(Qt::AlignCenter);
    m_pWriteBtn = new QPushButton;
    m_pReadBtn = new QPushButton;
    m_pClearBtn = new QPushButton;
    m_pWriteBtn->setMaximumWidth(100);
    m_pReadBtn->setMaximumWidth(100);
    m_pClearBtn->setMaximumWidth(100);
    hLayout->addWidget(m_pWriteBtn);
    hLayout->addSpacing(50);
    hLayout->addWidget(m_pReadBtn);
    hLayout->addSpacing(50);
    hLayout->addWidget(m_pClearBtn);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(fLayout);
    vLayout->addLayout(hLayout);

    m_pageWidget[FACTORY_PAGE_RFID]->setLayout(vLayout);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_RFID], icon1, tr("RFID TEST"));

    connect(m_pWriteBtn, SIGNAL(clicked()), this, SLOT(on_writeBtn_clicked()));
    connect(m_pReadBtn, SIGNAL(clicked()), this, SLOT(on_readBtn_clicked()));
    connect(m_pClearBtn, SIGNAL(clicked()), this, SLOT(on_clearBtn_clicked()));
}

void Ex_FactoryTestPage::initUpdateWifiPage()
{
    m_pageWidget[FACTORY_PAGE_UPDWIFI] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_UPDWIFI]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_UPDWIFI]->setStyleSheet(qss);

    m_pClearWifiMsgBtn = new QPushButton(m_pageWidget[FACTORY_PAGE_UPDWIFI]);
    m_pClearWifiMsgBtn->setGeometry(550, 50, 80, 30);
    connect(m_pClearWifiMsgBtn, SIGNAL(clicked()), this, SLOT(on_clearWifiMsgBtn_clicked()));

    m_pWifiMsgTBrowser = new QTextBrowser(m_pageWidget[FACTORY_PAGE_UPDWIFI]);
    m_pWifiMsgTBrowser->setGeometry(50, 85, 700, 400);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_UPDWIFI], icon1, tr("Update Wifi"));
}

void Ex_FactoryTestPage::initzigbeePage()
{
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setStyleSheet(qss);

    QHBoxLayout  *hLayout =  new QHBoxLayout;

    m_plbZigbeeUpd = new QLabel;
    m_plbZigbeeUpd->setAlignment(Qt::AlignCenter);
    m_plbZigbeeUpd->setText("00");
    hLayout->addWidget(m_plbZigbeeUpd);

    m_pBtnZigbeeUpd  = new QPushButton;
    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));
    hLayout->addWidget(m_pBtnZigbeeUpd);

    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setLayout(hLayout);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_UPDZIGBEE], icon1, tr("Maintenance"));

    connect(m_pBtnZigbeeUpd, SIGNAL(clicked()), this, SLOT(on_updZigbeeBtn_clicked()));
}

void Ex_FactoryTestPage::on_flowBtn_clicked()
{
    if(isFlow)
    {
        return;
    }
    isFlow = true;

    int iRet;
    iRet = Ex_FactoryTest(0);
    if(iRet)
    {
        QMessageBox::warning(m_widget, tr("Fail: Flow"), tr("Can not enter test Mode"), QMessageBox::Ok);
    }
}

void Ex_FactoryTestPage::on_iLoopBtn_clicked()
{
    if(isPressure)
    {
        return;
    }
    isPressure = true;
    int iRet;
    iRet = Ex_FactoryTest(1);
    if(iRet)
    {
        if(iRet)
        {
            QMessageBox::warning(m_widget, tr("Fail: Pressure"), tr("Can not enter test Mode"), QMessageBox::Ok);
        }
    }
}

void Ex_FactoryTestPage::on_keyBtn_clicked()
{
}

void Ex_FactoryTestPage::on_writeBtn_clicked()
{
#ifdef RFIDTEST
    m_wndMain->prepareKeyStroke();
    int index = m_pIndexCombo->currentIndex();
    QString catData = m_pConfigLineEdit[CONFIG_CAT]->text();
    QString lotData = m_pConfigLineEdit[CONFIG_LOT]->text();
    QString installDate = m_pConfigLineEdit[CONFIG_INSTALLDATE]->text();
    QString volData = m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->text();
    int iRet = 0;

    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_CATALOGUE_NUM, catData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write cat error"), QMessageBox::Ok);
    }
    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_LOT_NUMBER, lotData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write lot error"), QMessageBox::Ok);
    }

    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_INSTALL_DATE, installDate);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write install date error"), QMessageBox::Ok);
    }
    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_UNKNOW_DATA, volData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write vol data error"), QMessageBox::Ok);
    }

    Ex_HintDialog::getInstance(tr("Write finished"));
    m_wndMain->updateCMInfoWithRFID(0);
#endif
}

void Ex_FactoryTestPage::on_readBtn_clicked()
{
#ifdef RFIDTEST
    m_wndMain->prepareKeyStroke();
    updateRFIDInfo(m_pIndexCombo->currentIndex());
#endif
}

void Ex_FactoryTestPage::on_clearBtn_clicked()
{
    m_pConfigLineEdit[CONFIG_CAT]->clear();
    m_pConfigLineEdit[CONFIG_LOT]->clear();
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->clear();
    m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->clear();
}

void Ex_FactoryTestPage::on_clearWifiMsgBtn_clicked()
{
    m_pWifiMsgTBrowser->clear();
}

void Ex_FactoryTestPage::on_updZigbeeBtn_clicked()
{
    int iLength;

    QString BIN_FILE =  ":/other/shznApp.bin";


    QFileInfo info(BIN_FILE);
    if (info.exists())
    {
        iLength = info.size();
    }

    if (iLength <= MAX_FILE_SIZE)
    {
        QFile tmpfile(BIN_FILE);
        tmpfile.open(QIODevice::ReadOnly);
        tmpfile.read((char*)gFileMem, iLength);
        tmpfile.close();

        DispStartZigbeeUpd(iLength,NULL);
    }
    m_wndMain->prepareKeyStroke();
}


void Ex_FactoryTestPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;
    //add page
    initRFIDTestPage();
    initFlowTestPage();
    initUpdateWifiPage();
    initzigbeePage();

    mainLayout->addWidget(m_tabWidget, 0, 0);
    m_mainWidget->setLayout(mainLayout);

    QFile qss(":/app/tabWidget.qss");
    qss.open(QFile::ReadOnly);
    QString tabWidgetqss = QLatin1String (qss.readAll());
    qss.close();

    m_tabWidget->setStyleSheet(tabWidgetqss);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);

}

void Ex_FactoryTestPage::show(bool bShow)
{
    CSubPage::show(bShow);
}

void Ex_FactoryTestPage::update()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_INSTALL);
    CSubPage::update();
}

void Ex_FactoryTestPage::fade()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_NORMAL);
    CSubPage::fade();
}


void Ex_FactoryTestPage::updateFlow(int iIndex, int value)
{
    double temp = (value*1.0)/1000;
    switch(iIndex)
    {
    case APP_FM_FM1_NO:
        m_pFlowDisplayLE[S1_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM2_NO:
        m_pFlowDisplayLE[S2_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM3_NO:
        m_pFlowDisplayLE[S3_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM4_NO:
        m_pFlowDisplayLE[S4_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    }
}

void Ex_FactoryTestPage::updatePressure(int iIndex, float fvalue)
{
    if(APP_EXE_PM1_NO == iIndex)
    {
        m_pPreDisplayLE[P1_DISPLAY]->setText(QString("%1").arg(fvalue));
    }
}

void Ex_FactoryTestPage::updTank(int iLevel, float fVolume)
{
    Q_UNUSED(fVolume);
    m_pPreDisplayLE[P2_DISPLAY]->setText(QString("%1").arg(iLevel));
}

void Ex_FactoryTestPage::updSourceTank(int iLevel, float fVolume)
{
    Q_UNUSED(fVolume);
    m_pPreDisplayLE[P3_DISPLAY]->setText(QString("%1").arg(iLevel));
}

void Ex_FactoryTestPage::updateRFIDInfo(int iRfId)
{
    int iRet;

    CATNO cn;
    LOTNO ln;
    QDate installDate;
    int volUsed;

    memset(cn,0,sizeof(CATNO));
    memset(ln,0,sizeof(LOTNO));

//    if (m_wndMain->getRfidState(iRfId))
//    {
//        m_wndMain->getRfidCatNo(iRfId,cn);
//        m_wndMain->getRfidLotNo(iRfId,ln);
//        m_wndMain->getRfidInstallDate(iRfId, &installDate);
//        m_wndMain->getRfidVolofUse(iRfId, volUsed);
//    }

//    else
    {
        iRet = m_wndMain->readRfid(iRfId);
        if (iRet)
        {
            return;
        }

        m_wndMain->getRfidCatNo(iRfId,cn);
        m_wndMain->getRfidLotNo(iRfId,ln);
        m_wndMain->getRfidInstallDate(iRfId, &installDate);
        m_wndMain->getRfidVolofUse(iRfId, volUsed);
    }

    m_pConfigLineEdit[CONFIG_CAT]->setText(cn);
    m_pConfigLineEdit[CONFIG_LOT]->setText(ln);
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setText(installDate.toString("yyyy-MM-dd"));
    m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->setText(QString("%1").arg(volUsed));
}

void Ex_FactoryTestPage::updateWifiTestMsg(const QString &msg)
{
    QString strContent = m_pWifiMsgTBrowser->toPlainText();
    m_pWifiMsgTBrowser->setText(strContent + "\n" + msg);
}

void Ex_FactoryTestPage::zigbeeUpdResult(int iResult, int iPercent)
{
    if (iResult)
    {
        m_plbZigbeeUpd->setText(tr("FAIL"));
    }
    else
    {
        m_plbZigbeeUpd->setText(QString::number(iPercent));
    }
}












