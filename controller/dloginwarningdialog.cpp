#include "dloginwarningdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMutexLocker>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>

DLoginWarningDialog *DLoginWarningDialog::instance = NULL;

QMutex g_loginWaringMutex;

DLoginWarningDialog::DLoginWarningDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QColor(112,128,144));
    this->setPalette(palette);

    palette = this->palette();
    palette.setBrush(QPalette::WindowText, Qt::white);
    this->setPalette(palette);//

    setFixedSize(280, 120); //200 60

    m_pLbInfo    = new QLabel;
    m_pLbInfo->setAlignment(Qt::AlignCenter);
    m_pLbInfo->setWordWrap(true);

    m_pColoseBtn = new QPushButton(tr("close"));
    connect(m_pColoseBtn, SIGNAL(clicked()), this, SLOT(on_closeBtn_clicked()));

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addStretch();
    hlayout->addWidget(m_pColoseBtn);
    hlayout->addStretch();
    m_pBottomLayout =  new QVBoxLayout;

    m_pBottomLayout->addWidget(m_pLbInfo);
    m_pBottomLayout->addLayout(hlayout);

    setLayout(m_pBottomLayout);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timer_event()));
}

DLoginWarningDialog::~DLoginWarningDialog()
{
    qDebug() << tr("~DLoginWarningDialog()");
}

void DLoginWarningDialog::setInfo(QString strText)
{
    m_pLbInfo->setText(strText);
}

void DLoginWarningDialog::getInstance(QString strText)
{
    if (NULL == instance)
    {
        QMutexLocker locker(&g_loginWaringMutex);
        if(NULL == instance)
        {
            instance  = new DLoginWarningDialog();
            instance->setInfo(strText);
            instance->start();
        }
    }
}

void DLoginWarningDialog::on_closeBtn_clicked()
{
    deleteInstance();
}

void DLoginWarningDialog::start()
{
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_timer->start(1000*8);
    show();
}

void DLoginWarningDialog::deleteInstance()
{
    QMutexLocker locker(&g_loginWaringMutex);
    if(instance != NULL)
    {
        m_timer->stop();
        delete this;
        instance = NULL;
    }
}

void DLoginWarningDialog::on_timer_event()
{
    deleteInstance();
}

void DLoginWarningDialog::mousePressEvent(QMouseEvent *event)
{
    m_offsetPoint = event->pos();
}

void DLoginWarningDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->globalPos() - m_offsetPoint;
    this->move(point);
}

