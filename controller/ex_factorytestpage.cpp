#include "ex_factorytestpage.h"
#include "titlebar.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include "Ex_Display_c.h"
#include <QMessageBox>
#include <QGridLayout>


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

void Ex_FactoryTestPage::buildTranslation()
{
    int i;
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

    m_pBtn[FTESTPAGE_ILOOP]->setText(tr("Stop"));

}

void Ex_FactoryTestPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}


void Ex_FactoryTestPage::createControl()
{
    int i;
    QWidget* tmpWidget = new QWidget(m_widget);
    tmpWidget->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    tmpWidget->setStyleSheet(qss);

    for(i = 0; i < FTESTPAGE_NUM; i++)
    {
        m_pFrame[i] = new QFrame(tmpWidget);
        m_pBtn[i] = new QPushButton;
        m_pBtn[i]->setFixedWidth(80);
        m_pAreaLab[i] = new QLabel;
    }

    for(i = 0; i < FLOW_DISPLAY_NUM; i++)
    {
        m_pFlowDisplayLE[i] = new QLineEdit;
        m_pFlowDisplayLE[i]->setReadOnly(true);
        m_pFlowDisplayLab[i] = new QLabel;
    }

    for(i = 0; i <  PRESSURE_DISPLAY_NUM; i++)
    {
        m_pPreDisplayLE[i] = new QLineEdit;
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


void Ex_FactoryTestPage::initUi()
{
    createControl();
}

void Ex_FactoryTestPage::show(bool bShow)
{
    CSubPage::show(bShow);
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

void Ex_FactoryTestPage::updSoureTank(int iLevel, float fVolume)
{
    Q_UNUSED(fVolume);
    m_pPreDisplayLE[P3_DISPLAY]->setText(QString("%1").arg(iLevel));
}












