#include "dhintdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QMutexLocker>
#include <QDebug>

DHintDialog *DHintDialog::instance = NULL;

QMutex hintDialogmutex;

DHintDialog::DHintDialog(QWidget *parent) :
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

DHintDialog::~DHintDialog()
{
    qDebug() << tr("~DHintDialog()");
}

void DHintDialog::setInfo(QString strText)
{
    m_pLbInfo->setText(strText);
}


void DHintDialog::getInstance(QString strText)
{
    if (NULL == instance)
    {
        QMutexLocker locker(&hintDialogmutex);
        if(NULL == instance)
        {
            DHintDialog *pDlg = new DHintDialog();
            pDlg->setInfo(strText);
            pDlg->start();
            instance = pDlg;
        }
    }
}

void DHintDialog::start()
{
    m_timer->start(1000);
    show();
}

void DHintDialog::on_timer_event()
{
    if(instance != NULL)
    {
        m_timer->stop();
        delete this;
        instance = NULL;
    }
}
