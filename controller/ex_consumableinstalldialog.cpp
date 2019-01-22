#include "ex_consumableinstalldialog.h"
#include "Display.h"
#include "mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

Ex_ConsumableInstallDialog::Ex_ConsumableInstallDialog(int id, QWidget * parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    m_instanceID(id)
{
    initUI();
    buildTranslation();
    this->setWindowTitle(tr("Components Installation"));
}


void Ex_ConsumableInstallDialog::switchLanguage()
{
    buildTranslation();
}

void Ex_ConsumableInstallDialog::initUI()
{
    m_pcomboBox = new QComboBox(this);

    m_pCatLabel = new QLabel(this);
    m_pLineEditCat = new QLineEdit(this);
    m_pLineEditCat->setAlignment(Qt::AlignCenter);
    m_pLineEditCat->setReadOnly(true);

    m_pLotLabel = new QLabel(this);
    m_pLineEditLot = new QLineEdit(this);
    m_pLineEditLot->setAlignment(Qt::AlignCenter);
    m_pLineEditLot->setReadOnly(true);

    m_pUserLabel = new QLabel(this);
    m_pLineEditUser = new QLineEdit(this);
    m_pLineEditUser->setAlignment(Qt::AlignCenter);

    m_pInstallBtn = new QPushButton(tr("Install"), this);
    m_pCloseBtn = new QPushButton(tr("close"), this);
    m_pCloseBtn->hide();

    m_pcomboBox->setGeometry(60, 20, 180, 30);

    m_pCatLabel->setGeometry(20, 70, 60, 30);
    m_pLineEditCat->setGeometry(90, 70, 180, 30);

    m_pLotLabel->setGeometry(20, 120, 60, 30);
    m_pLineEditLot->setGeometry(90, 120, 180, 30);

    m_pUserLabel->setGeometry(20, 170, 60, 30);
    m_pLineEditUser->setGeometry(90, 170, 180, 30);

    m_pInstallBtn->setGeometry(100, 220, 100, 30); //40, 170, 100, 30
//    m_pCloseBtn->setGeometry(160, 220, 100, 30);

    connect(m_pInstallBtn, SIGNAL(clicked()), this, SLOT(on_installBtn_clicked()));
    connect(m_pcomboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboBox_currentIndexChanged(int)));
    //    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(on_closeBtn_clicked()));
}

bool Ex_ConsumableInstallDialog::checkUserInfo(const QString &userName)
{
    if((userName.compare("Super", Qt::CaseInsensitive) == 0)
     ||(userName.compare("Service", Qt::CaseInsensitive) == 0)
     ||(userName.compare("Manager", Qt::CaseInsensitive) == 0))
    {
        return true;
    }

    QString strQuery = QString("select * from User where name = '%1'").arg(userName);
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        qDebug() << "Name: " << name;
        if((userName.compare(name, Qt::CaseInsensitive) == 0))
        {
            return true;
        }
    }

    return false;
}

void Ex_ConsumableInstallDialog::buildTranslation()
{
    this->setWindowTitle(tr("Components Installation"));

    m_pCatLabel->setText(tr("CAT"));
    m_pLotLabel->setText(tr("LOT"));
    m_pUserLabel->setText(tr("User Name:"));

    m_pInstallBtn->setText(tr("Install"));
    m_pCloseBtn->setText(tr("close"));
}

void Ex_ConsumableInstallDialog::on_closeBtn_clicked()
{
    this->close();
    qDebug() << QString("on_closeBtn_clicked() instanceID: %1").arg(m_instanceID);
}

void Ex_ConsumableInstallDialog::on_installBtn_clicked()
{
    QString userName = m_pLineEditUser->text();
    if(userName.isEmpty())
    {
        QMessageBox::warning(this, tr("Waring"), tr("Please enter a valid username"), QMessageBox::Ok);
        return;
    }

    bool ok = this->checkUserInfo(userName);
    if(!ok)
    {
        QMessageBox::warning(this, tr("Waring"), tr("Invalid username"), QMessageBox::Ok);
        return;
    }
    gpMainWnd->saveLoginfo(userName);

    emit installConusumable();
    this->close();
    qDebug() << QString("on_installBtn_clicked() instanceID: %1").arg(m_instanceID);
}

void Ex_ConsumableInstallDialog::on_comboBox_currentIndexChanged(int index)
{
    if((DISP_T_A_FILTER == m_iType) || (DISP_T_B_FILTER == m_iType))
    {
        if(0 == index)
        {
            m_iType = DISP_T_A_FILTER;
            emit consumableTypeChanged(m_iType);
        }
        if(1 == index)
        {
            m_iType = DISP_T_B_FILTER;
            emit consumableTypeChanged(m_iType);
        }

    }
}

void Ex_ConsumableInstallDialog::setConsumableName(int iType, const QString &catNo, const QString &lotNo)
{
    if(this->isVisible())
    {
        return;
    }
    m_pcomboBox->clear();
    m_pLineEditUser->clear();
    m_iType = iType;
    switch(iType)
    {
    case DISP_P_PACK:
        m_pcomboBox->addItem(tr("P Pack"));
        break;
    case DISP_AC_PACK:
        m_pcomboBox->addItem(tr("AC Pack"));
        break;
    case DISP_U_PACK:
        m_pcomboBox->addItem(tr("U Pack"));
        break;
    case DISP_H_PACK:
        m_pcomboBox->addItem(tr("H Pack"));
        break;
    case DISP_PRE_PACK:
        m_pcomboBox->addItem(tr("Prefilter"));
        break;
    case DISP_P_PACK | (1 << 16):
        m_pcomboBox->addItem(tr("Clean Pack"));
        break;
    case DISP_AT_PACK:
        m_pcomboBox->addItem(tr("AT Pack"));
        break;
    case DISP_T_PACK:
        m_pcomboBox->addItem(tr("T Pack"));
        break;
    case DISP_MACHINERY_RO_MEMBRANE:
        m_pcomboBox->addItem(tr("RO Membrane"));
        break;
    case DISP_N2_UV:
        m_pcomboBox->addItem(tr("185 UV Lamp"));
        break;
    case DISP_N1_UV:
        m_pcomboBox->addItem(tr("254 UV Lamp"));
        break;
    case DISP_N3_UV:
        m_pcomboBox->addItem(tr("Tank UV Lamp"));
        break;
    case DISP_MACHINERY_RO_BOOSTER_PUMP:
        m_pcomboBox->addItem(tr("RO Pump"));
        break;
    case DISP_MACHINERY_CIR_PUMP:
        m_pcomboBox->addItem(tr("Recir. Pump"));
        break;
    case DISP_T_A_FILTER:
        m_pcomboBox->addItem(tr("Final Fliter A"));
        m_pcomboBox->addItem(tr("Final Fliter B"));
        break;
    case DISP_MACHINERY_EDI:
        m_pcomboBox->addItem(tr("EDI Module"));
        break;
    case DISP_W_FILTER:
        m_pcomboBox->addItem(tr("Tank Vent Filter"));
        break;
    default:
        break;
    }

    m_pLineEditCat->setText(catNo);
    m_pLineEditLot->setText(lotNo);

    this->show();
}

void Ex_ConsumableInstallDialog::closeEvent(QCloseEvent *e)
{
    emit setCheckStatus(false);
    QDialog::closeEvent(e);
}
