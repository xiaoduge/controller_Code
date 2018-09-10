#include <QTextCodec>
#include <QMessageBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>

#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "ToastDlg.h"
#include "mainwindow.h"

ToastDlg * ToastDlg::instance = NULL;

ToastDlg::ToastDlg(QWidget *parent) :
    QDialog(parent,Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{

    setWindowFlags( Qt::CustomizeWindowHint 
                  | Qt::WindowMinimizeButtonHint 
                  | Qt::WindowMaximizeButtonHint);
    
    setFixedSize(200, 60);

    m_pLbInfo    = new QLabel;
    m_pLbInfo->setAlignment(Qt::AlignCenter);

    m_pBottomLayout =  new QVBoxLayout;

    m_pBottomLayout->addWidget(m_pLbInfo);

    setLayout(m_pBottomLayout);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timer_event()));

}

ToastDlg::~ToastDlg()
{
    if (m_pLbInfo)       delete m_pLbInfo;
    if (m_pBottomLayout) delete m_pBottomLayout;
}

void ToastDlg::setInfo(QString strText)
{
    m_pLbInfo->setText(strText);
}


void ToastDlg::makeToast(QString strText)
{
    if (!instance)
    {
       ToastDlg *pDlg = new ToastDlg();
       pDlg->setInfo(strText);
       pDlg->start();
       instance = pDlg;
    }
}

void ToastDlg::start()
{
    m_timer->start(1000); 
    show();
}

void ToastDlg::on_timer_event()
{
    m_timer->stop();
    
    delete this;

    instance = NULL;
}


