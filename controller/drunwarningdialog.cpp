#include "drunwarningdialog.h"
#include "ex_dialogtitle.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

DRunWarningDialog::DRunWarningDialog(const QString& text, QWidget *parent) : QDialog(parent), m_text(text)
{
    initUI();
}

void DRunWarningDialog::setButtonText(int num, const QString &text)
{
    switch(num)
    {
    case 0:
        m_pConfirmBtn->setText(text);
        break;
    case 1:
        m_pQuitBtn->setText(text);
        break;
    default:
        break;
    }
}

void DRunWarningDialog::initTitleBar()
{
    this->setFixedSize(360, 222);
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_titleBar = new Ex_DialogTitle(this);
    m_titleBar->move(0, 0);
    m_titleBar->setTitleContent(tr("Warning"), Qt::white, 16);
}

void DRunWarningDialog::initUI()
{
    initTitleBar();

    m_pLabel = new QLabel(m_text, this);
    m_pLabel->setWordWrap(true);
    m_pLabel->setAlignment(Qt::AlignCenter);
    QPalette palette = m_pLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pLabel->setPalette(palette);

    m_pConfirmBtn = new QPushButton(tr("Confirm"), this);
    m_pQuitBtn = new QPushButton(tr("Quit"), this);

    m_pLabel->setGeometry(10, 40, 340, 110);

    m_pConfirmBtn->setGeometry(60, 160, 100, 30);
    m_pQuitBtn->setGeometry(200, 160, 100, 30);

    QString qss = QString("QDialog{\
                          background-color:rgb(23, 78, 158);\
                         }\
                         QPushButton\
                         {\
                            color:white;\
                            background-color:rgb(14 , 150 , 254);\
                            border-radius:5px;\
                         }\
                         QPushButton:hover \
                         {\
                            color:white;\
                            background-color:rgb(44 , 137 , 255);\
                         }\
                         QPushButtonn:pressed\
                         {\
                            color:white;\
                            background-color:rgb(14 , 135 , 228);\
                            padding-left:3px;\
                            padding-top:3px;\
                         }");

    this->setStyleSheet(qss);

    connect(m_pConfirmBtn, SIGNAL(clicked()), this, SLOT(on_confirmBtn_clicked()));
    connect(m_pQuitBtn, SIGNAL(clicked()), this, SLOT(on_quitBtn_clicked()));
}

void DRunWarningDialog::on_confirmBtn_clicked()
{
    accept();
}

void DRunWarningDialog::on_quitBtn_clicked()
{
    reject();
}
