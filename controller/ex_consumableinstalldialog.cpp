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
#include <QDebug>

Ex_ConsumableInstallDialog::Ex_ConsumableInstallDialog(int id, QWidget * parent) :
    Ex_BaseWindow(parent),
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
    QPalette palette = m_pCatLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pCatLabel->setPalette(palette);

    m_pLineEditCat = new QLineEdit(this);
    m_pLineEditCat->setAlignment(Qt::AlignCenter);
    m_pLineEditCat->setReadOnly(true);

    m_pLotLabel = new QLabel(this);
    palette = m_pLotLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pLotLabel->setPalette(palette);

    m_pLineEditLot = new QLineEdit(this);
    m_pLineEditLot->setAlignment(Qt::AlignCenter);
    m_pLineEditLot->setReadOnly(true);

    m_pUserLabel = new QLabel(this);
    palette = m_pUserLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pUserLabel->setPalette(palette);

    m_pLineEditUser = new QLineEdit(this);
    m_pLineEditUser->setAlignment(Qt::AlignCenter);

    m_pInstallBtn = new QPushButton(tr("Install"), this);

    m_pcomboBox->setGeometry(100, 50, 180, 30);
    m_pLineEditCat->setGeometry(100, 80, 180, 30);
    m_pLineEditLot->setGeometry(100, 110, 180, 30);
    m_pLineEditUser->setGeometry(100, 140, 180, 30);

    m_pCatLabel->setGeometry(45, 80, 50, 30);
    m_pLotLabel->setGeometry(45, 110, 50, 30);
    m_pUserLabel->setGeometry(45, 140, 50, 30);

    m_pInstallBtn->setGeometry(100, 180, 180, 30); //135, 180, 100, 30
    m_pInstallBtn->setObjectName("loginButton");
    QString qss = QString("QPushButton#loginButton\
                         {\
                            color:white;\
                            background-color:rgb(14 , 150 , 254);\
                            border-radius:5px;\
                         }\
                         QPushButton#loginButton:hover \
                         {\
                            color:white;\
                            background-color:rgb(44 , 137 , 255);\
                         }\
                         QPushButton#loginButton:pressed\
                         {\
                            color:white;\
                            background-color:rgb(14 , 135 , 228);\
                            padding-left:3px;\
                            padding-top:3px;\
                         }");
    m_pInstallBtn->setStyleSheet(qss);


    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
    {
        m_pUserLabel->hide();
        m_pLineEditUser->hide();
    }

    connect(m_pInstallBtn, SIGNAL(clicked()), this, SLOT(on_installBtn_clicked()));
    connect(m_pcomboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboBox_currentIndexChanged(int)));
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
    this->setTitleText(tr("Components Installation"), Qt::white, 16);

    m_pCatLabel->setText(tr("CAT"));
    m_pLotLabel->setText(tr("LOT"));
    m_pUserLabel->setText(tr("User"));

    m_pInstallBtn->setText(tr("Install"));
}

void Ex_ConsumableInstallDialog::updatePage()
{
    buildTranslation();
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
    {
        if(m_pUserLabel->isVisible())
        {
            m_pUserLabel->hide();
            m_pLineEditUser->hide();
        }
    }
    else
    {
        if(!m_pUserLabel->isVisible())
        {
            m_pUserLabel->show();
            m_pLineEditUser->show();
        }
    }
}

void Ex_ConsumableInstallDialog::on_closeBtn_clicked()
{
    this->close();
    qDebug() << QString("on_closeBtn_clicked() instanceID: %1").arg(m_instanceID);
}

void Ex_ConsumableInstallDialog::on_installBtn_clicked()
{
    if (!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization)))
    {
        QString userName = m_pLineEditUser->text();
        if(userName.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter a valid username"), QMessageBox::Ok);
            return;
        }

        bool ok = this->checkUserInfo(userName);
        if(!ok)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Invalid username"), QMessageBox::Ok);
            return;
        }
        gpMainWnd->saveLoginfo(userName);
    }
    else
    {
        gpMainWnd->saveLoginfo("unknow");
    }

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
    QWidget::closeEvent(e);
}
