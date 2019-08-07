#include "dconsumableinstallwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QFile>

DConsumableInstallWidget::DConsumableInstallWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    initUI();
}

void DConsumableInstallWidget::setTag(const QString &tags)
{
    m_pTagLabel->setText(tags);
}

void DConsumableInstallWidget::setInstallStatus(bool status)
{
    m_pInsCheckBox->setChecked(status);
}

void DConsumableInstallWidget::setID(int id)
{
    m_id = id;
}

int DConsumableInstallWidget::id()
{
    return m_id;
}

void DConsumableInstallWidget::initUI()
{
    m_pTagLabel = new QLabel;
    m_pTagLabel->setFixedWidth(150);
    m_pTagLabel->setStyleSheet("font-size:20pt;color:#000000;font-family:Arial;QFont::Bold");
    m_pInsCheckBox = new QCheckBox;
    m_pInsCheckBox->setEnabled(false);

    QFile qss(":/app/checkbox.qss");
    qss.open(QFile::ReadOnly);
    QString strQss = QLatin1String (qss.readAll());
    qss.close();
    m_pInsCheckBox->setStyleSheet(strQss);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addSpacing(20);
    hLayout->addWidget(m_pTagLabel);
    hLayout->addSpacing(180);
    hLayout->addWidget(m_pInsCheckBox);
    hLayout->addStretch();

    this->setLayout(hLayout);

}
