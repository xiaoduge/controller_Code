#include "ex_hintdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QMutexLocker>
#include <QDebug>

Ex_HintDialog *Ex_HintDialog::instance = NULL;

QMutex hintDialogmutex;

Ex_HintDialog::Ex_HintDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QColor(112,128,144));
    this->setPalette(palette);

    palette = this->palette();
    palette.setBrush(QPalette::WindowText, Qt::white);
    this->setPalette(palette);//

    setFixedSize(250, 70); //200 60

    m_pLbInfo    = new QLabel;
    m_pLbInfo->setAlignment(Qt::AlignCenter);

    m_pBottomLayout =  new QVBoxLayout;

    m_pBottomLayout->addWidget(m_pLbInfo);

    setLayout(m_pBottomLayout);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timer_event()));
}

Ex_HintDialog::~Ex_HintDialog()
{
    qDebug() << tr("~Ex_HintDialog()");
}

void Ex_HintDialog::setInfo(QString strText)
{
    m_pLbInfo->setText(strText);
}


void Ex_HintDialog::getInstance(QString strText)
{
    if (NULL == instance)
    {
        QMutexLocker locker(&hintDialogmutex);
        Ex_HintDialog *pDlg = new Ex_HintDialog();
        pDlg->setInfo(strText);
        pDlg->start();
        instance = pDlg;
    }
}

void Ex_HintDialog::start()
{
    m_timer->start(1000);
    show();
}

void Ex_HintDialog::on_timer_event()
{
    m_timer->stop();
    delete this;
    instance = NULL;
}
