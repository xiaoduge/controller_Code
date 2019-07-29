#include "dwificonfigdialog.h"
#include "mainwindow.h"
#include "dlineedit.h"
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include "ex_hintdialog.h"
#include <QDebug>
#include<unistd.h>

DWifiConfigDialog::DWifiConfigDialog(QWidget *parent) : DWifiConfigWidget(parent)
{
    initUI();
}

void DWifiConfigDialog::updatePage()
{
    m_pPSKLabel->setText(tr("PassWord"));
    m_pConfigBtn->setText(tr("Config"));
    m_pCancelBtn->setText(tr("Cancel"));
}

void DWifiConfigDialog::setSSIDName(const QString &name)
{
    m_pSSIDLabel->setText(name);
}

void DWifiConfigDialog::on_configBtn_clicked()
{
    QString strFileName(WIFICONFIG_FILE);
    if(strFileName.isEmpty())
    {
        return;
    }

    QFile sourceFile(strFileName);

    if(!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug() << "Open wifi config file failed: 1";
        return;
    }

    QString strAll = QString("# WPA-PSK/TKIP\n\nctrl_interface=/var/run/wpa_supplicant\n\nnetwork={\n\tssid=\"%1\"\n\tscan_ssid=1\n\tkey_mgmt=WPA-PSK WPA-EAP\n\tproto=RSN\n\tpairwise=TKIP CCMP\n\tgroup=TKIP CCMP\n\tpsk=\"%2\"\n        auth_alg=OPEN\n\tpriority=1\n}\n\n")
            .arg(m_pSSIDLabel->text())
            .arg(m_pPSKLineEdit->text());

    QByteArray array = strAll.toLatin1();
    char *data = array.data();

    sourceFile.write(data);

    if(sourceFile.isOpen())
    {
        sourceFile.close();
    }
    m_pPSKLineEdit->clear();
    close();
    sync();
    Ex_HintDialog::getInstance(tr("Successfully"));
}

void DWifiConfigDialog::on_cancelBtn_clicked()
{
    m_pPSKLineEdit->clear();
    this->close();
}

void DWifiConfigDialog::initUI()
{
    m_pSSIDLabel = new QLabel(this);
    m_pSSIDLabel->setGeometry(0, 10, 300, 40);
    m_pSSIDLabel->setAlignment(Qt::AlignCenter);
    m_pSSIDLabel->setText(tr("WIFI SSID"));

    m_pPSKLabel = new QLabel(this);
    m_pPSKLabel->setGeometry(40, 55, 160, 25);

    m_pPSKLineEdit = new DLineEdit(this);
    m_pPSKLineEdit->setGeometry(40, 82, 220, 25);

    m_pConfigBtn = new QPushButton(this);
    connect(m_pConfigBtn, SIGNAL(clicked()), this, SLOT(on_configBtn_clicked()));


    m_pConfigBtn->setGeometry(0, 140, 150, 40);

    m_pCancelBtn = new QPushButton(this);
    connect(m_pCancelBtn, SIGNAL(clicked()), this, SLOT(on_cancelBtn_clicked()));
    m_pCancelBtn->setGeometry(150, 140, 150, 40);

    this->setStyleSheet("QPushButton{background: transparent;color: white;border-width: 2px;border-color:white}\
                         QLabel{color:white}");

}
