#include "CfgDialog.h"
#include <QInputDialog>
#include <QComboBox>
#include <QRect>
#include <QPushButton>
#include<QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QTextCodec>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include "mainwindow.h"

#include "keyboard.h"
#include "Display.h"
#include "MyParams.h"
#include "notify.h"

static QString strTmName[] = {"RoWashT1","RoWashT2","RoWashT3",
                              "PhWashT1","PhWashT2","PhWashT3","PhWashT4","PhWashT5",    
                              "InitRunT1",
                              "NormRunT1","NormRunT2","NormRunT3","NormRunT4","RoWashT5",
                              "N3Period", "N3Duration",
                              "TOCT1","TOCT2","RoWashT3",
                              "IdleCirPeriod", "InterCirDuration",
};

static QString strSmName[] = {"HaveB1","HaveB2","HaveB3",
                              "HaveN1","HaveN2","HaveN3",
                              "HaveT1",
                              "HaveI",
                              "HaveTOC",
                              "HaveP_PACK",
                              "HaveAT_PACK",
                              "HaveU_PACK",
                              "HaveH_PACK",
};


static QString strAlarmName[] = {"WaterIP","ROResi ","ROProd ","EdiProd","UPProd ",
                                 "B Over","B leak",
                                 "N1     ","N2     ","N3     ",
                                 "EDI   ",
                                 "GPUMP1", "GPUMP2",
                                 "RPUMP1V", "RPUMP2V","RPUMP1I",    "RPUMP2I",
};

static QString strFmName[] = {"FM1","FM2","FM3",
                              "FM4",
};

static QString strPmName[] = {"PM1","PM2","PM3",
};

static QString strWeekdayName[] = {
    "SUN","MON","TUE","WED","THU","FRI","SAT"
};

static QString strCalName[] = {
    "IN COND", 
    "IN TEMP",
    "RO COND", 
    "RO TEMP",
    "EDI COND",
    "EDI TEMP",
    "UP COND",
    "UP TEMP",
    "TOC COND",
    "TOC TEMP",
    "FM1",
    "FM2",
    "FM3",
    "FM4",
    "PM1",
    "PM2",
    "PM3",
    
};



/*QLineEdit LEdit[] = {
{N3Duration}
};*/


int giMachineIdx;
int giType;

CfgDlg::CfgDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("CFG"));
    int iLoop = 0 ,LENum = 0;
    QString strTemp;

    m_nWidgets = 0;

    Qt::WindowFlags flags=Qt::Dialog;
    //flags |=Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    mFontTime.setPointSize(24);

    m_pTblWidget = new QTabWidget();  

    m_pWidgets[m_nWidgets] = new QWidget();  

    /* for page 1*/
    m_pMainLayout   = new QGridLayout(this);
    
    for(iLoop = 0;iLoop < MACHINE_PARAM_SP_NUM; iLoop++)
    {
        strTemp ="sp";
        strTemp +=QString::number(iLoop+1);
        strTemp +=":";
        m_pLabelsp[iLoop] = new QLabel;
        m_pLabelsp[iLoop]->setText(strTemp);

        m_pMainLayout->addWidget(m_pLabelsp[iLoop],iLoop,1);

        m_pEditSP[iLoop] = new QLineEdit;
        
        m_pEditSP[iLoop]->setFixedSize(48,18);
        
        m_pMainLayout->addWidget(m_pEditSP[iLoop],iLoop,2);

        m_pEditSP[iLoop]->installEventFilter(this);
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditSP[iLoop];
        
    }

    for(iLoop = 0;iLoop < MACHINE_PARAM_SUBMODULE_NUM;iLoop++)
    {
        strTemp = strSmName[iLoop];
        m_pQCheckBoxSm[iLoop] = new QCheckBox;
        m_pQCheckBoxSm[iLoop]->setText(strTemp);
        m_pMainLayout->addWidget(m_pQCheckBoxSm[iLoop],iLoop + 2,7);
    }

    //setLayout(m_pMainLayout);

    m_pWidgets[m_nWidgets]->setLayout(m_pMainLayout);  
    QIcon icon1(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon1, "CFG1");      
    m_nWidgets++;

    QHBoxLayout *layout = new QHBoxLayout();  

    /* CFG 2*/
    m_pWidgets[m_nWidgets] = new QWidget();
    QGridLayout *pcfg2Grid = new QGridLayout;
    for(iLoop = 0;iLoop < MACHINE_PARAM_TM_NUM;iLoop++)
    {
        strTemp  = strTmName[iLoop];
        strTemp +=":";
        m_pLabelTm[iLoop] = new QLabel;
        m_pLabelTm[iLoop]->setText(strTemp);

        m_pEditTm[iLoop]  = new QLineEdit;
        
        m_pEditTm[iLoop]->setFixedSize(48,18);
        m_pEditTm[iLoop]->installEventFilter(this);

        pcfg2Grid->addWidget(m_pLabelTm[iLoop],iLoop/2,(iLoop % 2)*2+ 1);
        pcfg2Grid->addWidget(m_pEditTm[iLoop] ,iLoop/2,(iLoop % 2)*2+ 2);
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditTm[iLoop];
    }

    m_pWidgets[m_nWidgets]->setLayout(pcfg2Grid);  

    QIcon icon2(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon2, "TIMERS");      
    m_nWidgets++;

    /* cfg 3 */
    m_pWidgets[m_nWidgets] = new QWidget();
    QGridLayout *pcfg3Grid = new QGridLayout;
    for(iLoop = 0;iLoop < APP_FM_FLOW_METER_NUM;iLoop++)
    {
        strTemp  = strFmName[iLoop];
        strTemp +=":";
        m_pLabelFm[iLoop] = new QLabel;
        m_pLabelFm[iLoop]->setText(strTemp);

        m_pEditFm[iLoop]  = new QLineEdit;
        m_pEditFm[iLoop]->setFixedSize(48,18);
        m_pEditFm[iLoop]->installEventFilter(this);

        pcfg3Grid->addWidget(m_pLabelFm[iLoop],iLoop, 1);
        pcfg3Grid->addWidget(m_pEditFm[iLoop] ,iLoop, 2);
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditFm[iLoop];
    }

    for(iLoop = 0;iLoop < APP_EXE_PRESSURE_METER;iLoop++)
    {
        strTemp  = strPmName[iLoop];
        strTemp +=":";
        m_pLabelPm[iLoop] = new QLabel;
        m_pLabelPm[iLoop]->setText(strTemp);

        m_pEditPm[iLoop]  = new QLineEdit;
        m_pEditPm[iLoop]->setFixedSize(48,18);
        m_pEditPm[iLoop]->installEventFilter(this);

        m_pCombBox[iLoop] = new QComboBox();
        m_pCombBox[iLoop]->setFixedSize(48,18);

        switch(iLoop)
        {
        case 0:
            m_pCombBox[iLoop]->addItem("1.0MPa");
            m_pCombBox[iLoop]->addItem(tr("User Defined"));
            connect(m_pCombBox[iLoop], SIGNAL(currentIndexChanged(int)),
             this, SLOT(PM_PM1_CmbIndexChange(int)));
            break;
        case 1:
            m_pCombBox[iLoop]->addItem("30");
            m_pCombBox[iLoop]->addItem("60");
            m_pCombBox[iLoop]->addItem("100");
            m_pCombBox[iLoop]->addItem("200");
            m_pCombBox[iLoop]->addItem("350");
            m_pCombBox[iLoop]->addItem(tr("User Defined"));
            connect(m_pCombBox[iLoop], SIGNAL(currentIndexChanged(int)),
             this, SLOT(PM_PM2_CmbIndexChange(int)));
            break;
        case 2:
            m_pCombBox[iLoop]->addItem("30");
            m_pCombBox[iLoop]->addItem("60");
            m_pCombBox[iLoop]->addItem("100");
            m_pCombBox[iLoop]->addItem("200");
            m_pCombBox[iLoop]->addItem("350");
            m_pCombBox[iLoop]->addItem(tr("User Defined"));
            connect(m_pCombBox[iLoop], SIGNAL(currentIndexChanged(int)),
             this, SLOT(PM_PM3_CmbIndexChange(int)));
            break;
        }

        pcfg3Grid->addWidget(m_pLabelPm[iLoop],iLoop, 3);
        pcfg3Grid->addWidget(m_pEditPm[iLoop] ,iLoop, 4);
        pcfg3Grid->addWidget(m_pCombBox[iLoop],iLoop, 5);
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditPm[iLoop];
    }

    m_pWidgets[m_nWidgets]->setLayout(pcfg3Grid);  

    QIcon icon3(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon3, "CFG3");   
    m_nWidgets++;


    /* cfg 4 */
    m_pWidgets[m_nWidgets] = new QWidget();

    QVBoxLayout *pcfg4VBox = new QVBoxLayout();  
    QHBoxLayout *pcfg4HBox = new QHBoxLayout();  
    QGridLayout *pcfg4Grid = new QGridLayout;

    m_pEditTC[0]  = new QLineEdit;
    m_pEditTC[0]->installEventFilter(this);
    m_pEditTC[0]->setFont(mFontTime);
    
    m_pStartTime[0] = new QDateTimeEdit;
    m_pStartTime[0]->setDisplayFormat("HH");  
    m_pStartTime[0]->setFont(mFontTime);
    m_pEndTime[0] = new QDateTimeEdit;
    m_pEndTime[0]->setDisplayFormat("HH");      
    m_pEndTime[0]->setFont(mFontTime);

    pcfg4Grid->addWidget(m_pEditTC[0]    ,0, 1);
    pcfg4Grid->addWidget(m_pStartTime[0] ,0, 2);
    pcfg4Grid->addWidget(m_pEndTime[0]   ,0, 3);
    
    for(iLoop = 0;iLoop < 7;iLoop++)
    {
        strTemp  = strWeekdayName[iLoop];
        strTemp +=":";
        m_pQCheckBoxWeekday[iLoop] = new QCheckBox;
        m_pQCheckBoxWeekday[iLoop]->setText(strTemp);
        m_pQCheckBoxWeekday[iLoop]->setFont(mFontTime);

        pcfg4Grid->addWidget(m_pQCheckBoxWeekday[iLoop],1, iLoop);
    }

    pcfg4VBox->addLayout(pcfg4Grid);

    /* RPump */
    {
        QFile qss(":/app/musicslider.qss");
        qss.open(QFile::ReadOnly);
        QString qsss = QLatin1String (qss.readAll());
        qss.close();    
        m_pRPumpSlider = new QSlider(Qt::Horizontal);
        m_pRPumpSlider->setFixedSize(120,30);
        m_pRPumpSlider->setMinimum(0);
        m_pRPumpSlider->setMaximum(24);
        m_pRPumpSlider->setValue(12);
        m_pRPumpSlider->setStyleSheet(qsss);  
        pcfg4HBox->addWidget(m_pRPumpSlider);

        //qDebug() << qsss;
    }

    pcfg4VBox->addLayout(pcfg4HBox);

    m_pWidgets[m_nWidgets]->setLayout(pcfg4VBox);  

    QIcon icon4(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon4, "Misc");     
    m_nWidgets++;


    /* cfg 5 calibrate */
    m_pWidgets[m_nWidgets] = new QWidget();
    QGridLayout *pcfg5Grid = new QGridLayout;
    for(iLoop = 0;iLoop < DISP_CAL_I_NUM;iLoop++)
    {
        /* for coefficiences */
        strTemp  = strCalName[2*iLoop + 0];
        strTemp +=":";
        m_pLabelCal4Eco[2*iLoop + 0] = new QLabel;
        m_pLabelCal4Eco[2*iLoop + 0]->setText(strTemp);

        m_pEditCal[3*iLoop + 0]  = new QLineEdit;
        m_pEditCal[3*iLoop + 0]->setFixedSize(48,18);
        m_pEditCal[3*iLoop + 0]->installEventFilter(this);

        m_pEditCal[3*iLoop + 1]  = new QLineEdit;
        m_pEditCal[3*iLoop + 1]->setFixedSize(48,18);
        m_pEditCal[3*iLoop + 1]->installEventFilter(this);

        m_pEditCalState[2*iLoop + 0] = new QLineEdit;
        m_pEditCalState[2*iLoop + 0]->setFixedSize(48,18);
        m_pEditCalState[2*iLoop + 0]->setReadOnly(true); /* read only */

        pcfg5Grid->addWidget(m_pLabelCal4Eco[2*iLoop + 0],2*iLoop + 0, 1);
        pcfg5Grid->addWidget(m_pEditCal[3*iLoop + 0]     ,2*iLoop + 0, 2);
        pcfg5Grid->addWidget(m_pEditCal[3*iLoop + 1]     ,2*iLoop + 0, 3);
        pcfg5Grid->addWidget(m_pEditCalState[2*iLoop + 0],2*iLoop + 0, 4);


        /* for temp */
        strTemp  = strCalName[2*iLoop + 1];
        strTemp +=":";
        m_pLabelCal4Eco[2*iLoop + 1] = new QLabel;
        m_pLabelCal4Eco[2*iLoop + 1]->setText(strTemp);

        m_pEditCal[3*iLoop + 2]  = new QLineEdit;
        m_pEditCal[3*iLoop + 2]->setFixedSize(48,18);
        m_pEditCal[3*iLoop + 2]->installEventFilter(this);


        m_pEditCalState[2*iLoop + 1] = new QLineEdit;
        m_pEditCalState[2*iLoop + 1]->setFixedSize(48,18);
        m_pEditCalState[2*iLoop + 1]->setReadOnly(true); /* read only */

        pcfg5Grid->addWidget(m_pLabelCal4Eco[2*iLoop + 1],2*iLoop + 1, 1);
        pcfg5Grid->addWidget(m_pEditCal[3*iLoop + 2]     ,2*iLoop + 1, 2);
        pcfg5Grid->addWidget(m_pEditCalState[2*iLoop + 1],2*iLoop + 1, 4);
        
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCal[3*iLoop + 0];
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCal[3*iLoop + 1];
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCal[3*iLoop + 2];

    }

    for(iLoop = 0;iLoop < DISP_CAL_F_NUM ;iLoop++)
    {
        /* for coefficiences */
        strTemp  = strCalName[DISP_CAL_I_NUM*2 + iLoop];
        strTemp +=":";
        m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + iLoop] = new QLabel;
        m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + iLoop]->setText(strTemp);


        m_pEditCal[DISP_CAL_I_NUM*3 + iLoop]  = new QLineEdit;
        m_pEditCal[DISP_CAL_I_NUM*3 + iLoop]->setFixedSize(48,18);
        m_pEditCal[DISP_CAL_I_NUM*3 + iLoop]->installEventFilter(this);


        m_pEditCalState[2*DISP_CAL_I_NUM + iLoop] = new QLineEdit;
        m_pEditCalState[2*DISP_CAL_I_NUM + iLoop]->setFixedSize(48,18);
        m_pEditCalState[2*DISP_CAL_I_NUM + iLoop]->setReadOnly(true); /* read only */

        pcfg5Grid->addWidget(m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + iLoop],2*DISP_CAL_I_NUM + iLoop, 1);
        pcfg5Grid->addWidget(m_pEditCal[DISP_CAL_I_NUM*3 + iLoop]     ,2*DISP_CAL_I_NUM + iLoop, 2);
        pcfg5Grid->addWidget(m_pEditCalState[2*DISP_CAL_I_NUM + iLoop],2*DISP_CAL_I_NUM + iLoop, 4);
        
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCal[DISP_CAL_I_NUM*3 + iLoop];

    }

    for(iLoop = 0;iLoop < DISP_CAL_P_NUM ;iLoop++)
    {
        /* for coefficiences */
        strTemp  = strCalName[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + iLoop];
        strTemp +=":";
        m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + iLoop] = new QLabel;
        m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + iLoop]->setText(strTemp);


        m_pEditCal[DISP_CAL_I_NUM*3 + DISP_CAL_F_NUM + iLoop]  = new QLineEdit;
        m_pEditCal[DISP_CAL_I_NUM*3 + DISP_CAL_F_NUM + iLoop]->setFixedSize(48,18);
        m_pEditCal[DISP_CAL_I_NUM*3 + DISP_CAL_F_NUM + iLoop]->installEventFilter(this);


        m_pEditCalState[2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop] = new QLineEdit;
        m_pEditCalState[2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop]->setFixedSize(48,18);
        m_pEditCalState[2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop]->setReadOnly(true); /* read only */

        pcfg5Grid->addWidget(m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + iLoop],2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop, 1);
        pcfg5Grid->addWidget(m_pEditCal[DISP_CAL_I_NUM*3 + DISP_CAL_F_NUM + iLoop]     ,2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop, 2);
        pcfg5Grid->addWidget(m_pEditCalState[2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop],2*DISP_CAL_I_NUM + DISP_CAL_F_NUM + iLoop, 4);
        
        
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCal[DISP_CAL_I_NUM*3 + DISP_CAL_F_NUM + iLoop];

    }

    m_pWidgets[m_nWidgets]->setLayout(pcfg5Grid);  
    QIcon icon5(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon5, "Calibrate");   
    m_nWidgets++;


    /* CFG 6 */
    m_pWidgets[m_nWidgets] = new QWidget();
    QGridLayout *pcfg6Grid = new QGridLayout;

    for(iLoop = 0;iLoop < DISP_ALARM_NUM;iLoop++)
    {
        strTemp = strAlarmName[iLoop];
        m_pQCheckBoxAlarm[iLoop] = new QCheckBox;
        m_pQCheckBoxAlarm[iLoop]->setText(strTemp);

        pcfg6Grid->addWidget(m_pQCheckBoxAlarm[iLoop],iLoop,1);

        if (iLoop >= DISP_ALARM_N1
            && iLoop <= DISP_ALARM_RPI2)
        {
            int iEditIdx = iLoop-DISP_ALARM_N1;
            m_pEditAlarm[iEditIdx] = new QLineEdit;
            m_pEditAlarm[iEditIdx]->setFixedSize(48,18);
    
            pcfg6Grid->addWidget(m_pEditAlarm[iEditIdx] ,iLoop,2);
            m_pEditAlarm[iEditIdx]->installEventFilter(this);
            LineEdit_TYPE[LENum++].m_pEditName = m_pEditAlarm[iEditIdx];        
        }
    }

    m_pWidgets[m_nWidgets]->setLayout(pcfg6Grid);  

    QIcon icon6(":/pics/image/battery0.png");  
    m_pTblWidget->addTab(m_pWidgets[m_nWidgets], icon6, "ALARM");      
    m_nWidgets++;   

    /* end for cfgs */

    layout->addWidget(m_pTblWidget);  

    /* common */
    QGridLayout *tmpGrid = new QGridLayout;
    m_pLabelCFGTYPE      = new QLabel;
    m_pLabelCFGTYPE->setText("CFGTYPE");
    
    m_pComboBoxCFG  = new QComboBox;
    
    for(iLoop = 0;iLoop < MACHINE_NUM; iLoop++)
    {
        m_pComboBoxCFG->addItem(gaMachineType[iLoop].strName);
    }

    m_pComboBoxCFG->setCurrentIndex(gGlobalParam.iMachineType);

    m_pBtnSave = new QPushButton();
    m_pBtnSave->setObjectName(QString::fromUtf8("Save"));
    m_pBtnSave->setText("Save");

    m_pBtnExit = new QPushButton();
    m_pBtnExit->setObjectName(QString::fromUtf8("Exit"));
    m_pBtnExit->setText("Exit");

    tmpGrid->addWidget(m_pLabelCFGTYPE,1,7);
    tmpGrid->addWidget(m_pComboBoxCFG,1,8);

    tmpGrid->addWidget(m_pBtnSave,8,8);
    tmpGrid->addWidget(m_pBtnExit,9,8);

    layout->addLayout(tmpGrid);  

    this->setLayout(layout);  

    CFGCom();
    
    connect(m_pComboBoxCFG,SIGNAL(currentIndexChanged(int)),this,SLOT(CFGCom()));
    connect(m_pBtnSave,SIGNAL(clicked()),this,SLOT(SaveCfg()));
    connect(m_pBtnExit,SIGNAL(clicked()),this,SLOT(Exit()));

    for(iLoop = 0;iLoop < MACHINE_PARAM_SUBMODULE_NUM;iLoop++)
    {
        connect(m_pQCheckBoxSm[iLoop], SIGNAL(stateChanged(int)), this, SLOT(Have_Checked(int)));
    }
    
    for(iLoop = 0;iLoop < DISP_ALARM_NUM;iLoop++)
    {
        connect(m_pQCheckBoxAlarm[iLoop], SIGNAL(stateChanged(int)), this, SLOT(Alarm_Checked(int)));
    }
    
}

CfgDlg::~CfgDlg()
{

    delete m_pBtnSave;
    delete m_pBtnExit;

    delete m_pLabelCFGTYPE;
    delete m_pComboBoxCFG;
    delete m_pMainLayout;
    
}

void CfgDlg::CFGCom()
{
    int iLoop;

    for(iLoop = 0; iLoop < MACHINE_NUM ; iLoop++)
    {
        if(m_pComboBoxCFG->currentText() == gaMachineType[iLoop].strName)
        {
            giMachineIdx = iLoop;
            giType = gaMachineType[iLoop].iType;
            break;
        }
    }


    MainRetriveMachineParam(giMachineIdx,MMParam);
    MainRetriveTMParam(giMachineIdx,TMParam);
    MainRetriveAlarmSetting(giMachineIdx,AlarmSettting);
    MainRetriveSubModuleSetting(giMachineIdx,SubModSetting);
    MainRetriveCMParam(giMachineIdx,CMParam);
    MainRetriveFmParam(giMachineIdx,FmParam);
    MainRetrivePmParam(giMachineIdx,PmParam);
    MainRetriveCalParam(giMachineIdx,CalParam);
    MainRetriveMiscParam(giMachineIdx,MiscParam);
    MainRetriveCleanParam(giMachineIdx,CleanParam);

    for(iLoop = 0; iLoop < LESUM ; iLoop++)
    {
        if (gaMachineType[giMachineIdx].aParamId[iLoop])
        {
            LineEdit_TYPE[iLoop].m_pEditName->setEnabled(true);
        }
        else
        {
            LineEdit_TYPE[iLoop].m_pEditName->setEnabled(false);
        }
    }

    /* Machine paramter */
    for(iLoop = 0; iLoop < MACHINE_PARAM_SP_NUM ; iLoop++)
    {
        m_pEditSP[iLoop]->setText(QString::number(MMParam.SP[iLoop]));
    }
    
    for(iLoop = 0; iLoop < TIME_PARAM_NUM ; iLoop++)
    {
        m_pEditTm[iLoop]->setText(QString::number(TMParam.aulTime[iLoop]));
    }


    for(iLoop = 0; iLoop < DISP_SM_NUM ; iLoop++)
    {
        if ((SubModSetting.ulFlags & (1 << iLoop)) != 0)
        {
            m_pQCheckBoxSm[iLoop]->setChecked(true);
        }
        else
        {
            m_pQCheckBoxSm[iLoop]->setChecked(false);
        }
    }
    
    for(iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM ; iLoop++)
    {
        m_pEditFm[iLoop]->setText(QString::number(FmParam.aulCfg[iLoop]));
    }

    for(iLoop = 0; iLoop < APP_EXE_PRESSURE_METER ; iLoop++)
    {
        m_pEditPm[iLoop]->setText(QString::number(PmParam.afDepth[iLoop],'f',2));
    }

    for(iLoop = 0; iLoop < DISP_CAL_NUM ; iLoop++)
    {
        m_pEditCal[iLoop]->setText(QString::number(CalParam.afCfg[iLoop],'f',3));
    }


    for(iLoop = 0; iLoop < DISP_ALARM_NUM ; iLoop++)
    {
        if ((AlarmSettting.ulFlags & (1 << iLoop)) != 0)
        {
            m_pQCheckBoxAlarm[iLoop]->setChecked(true);
        }
        else
        {
            m_pQCheckBoxAlarm[iLoop]->setChecked(false);
        }

        if (iLoop >= DISP_ALARM_N1 && iLoop <= DISP_ALARM_RPI2)
        {
            int iEditIdx = iLoop - DISP_ALARM_N1;
            m_pEditAlarm[iEditIdx]->setText(QString::number(AlarmSettting.fAlarms[iEditIdx],'f',1));
        
        }
    }

    /* for tube cir setting */
    for (iLoop = 0; iLoop < 7 ; iLoop++)
    {
        if (MiscParam.ulAllocMask & (1 << iLoop))
        {
            m_pQCheckBoxWeekday[iLoop]->setChecked(true);
        }
        else
        {
            m_pQCheckBoxWeekday[iLoop]->setChecked(false);
        }
    }

    m_pEditTC[0]->setText(QString::number(MiscParam.iTubeCirDuration));

    QTime time;
    time.setHMS(MiscParam.aiTubeCirTimeInfo[0],0,0);
    m_pStartTime[0]->setTime(time);
    time.setHMS(MiscParam.aiTubeCirTimeInfo[1],0,0);
    m_pEndTime[0]->setTime(time);

}

void CfgDlg::SaveCfg()
{
    int iLoop ;

    for(iLoop = 0; iLoop < LESUM ; iLoop++)
    {
        if (iLoop < MACHINE_PARAM_SP_NUM)
        {
            float fValue = LineEdit_TYPE[iLoop].m_pEditName->text().toFloat();
            MMParam.SP[iLoop] = fValue;
        }
        else if (iLoop < MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM)
        {
            int  iValue = LineEdit_TYPE[iLoop].m_pEditName->text().toInt();
            TMParam.aulTime[iLoop - MACHINE_PARAM_SP_NUM] = iValue;
        }
        else if (iLoop < MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM)
        {
            int iValue = LineEdit_TYPE[iLoop].m_pEditName->text().toInt();
        
            FmParam.aulCfg[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM)] = iValue;
        }
        else if (iLoop < MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM)
        {
            float fValue = LineEdit_TYPE[iLoop].m_pEditName->text().toFloat();
        
            PmParam.afCap[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM)] = m_fPmCap[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM)];
            PmParam.afDepth[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM)] = fValue;
        }
        else if (iLoop < MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM + DISP_CAL_NUM)
        {
            int fValue = LineEdit_TYPE[iLoop].m_pEditName->text().toFloat();
        
            CalParam.afCfg[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM)] = fValue;
        }
        else if (iLoop < MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM + DISP_CAL_NUM + DISP_ALARM_PARAMS)
        {
            int fValue = LineEdit_TYPE[iLoop].m_pEditName->text().toFloat();
        
            AlarmSettting.fAlarms[iLoop - (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM + DISP_CAL_NUM)] = fValue;
        }
        
    }

    /* for checkbox */
    SubModSetting.ulFlags = 0;
    for (iLoop = 0; iLoop < MACHINE_PARAM_SUBMODULE_NUM; iLoop++)
    {
        int tmp;
        
        tmp = Qt::Checked == m_pQCheckBoxSm[iLoop]->checkState() ? 1 : 0;

        SubModSetting.ulFlags |= (tmp << iLoop);
    
    }

    /* for checkbox */
    AlarmSettting.ulFlags = 0;
    for (iLoop = 0; iLoop < DISP_ALARM_NUM; iLoop++)
    {
        int tmp;
        
        tmp = Qt::Checked == m_pQCheckBoxAlarm[iLoop]->checkState() ? 1 : 0;

        AlarmSettting.ulFlags |= tmp << iLoop;


    }   
    
    /* for tube circulation setting */
    {
        QTime time;
        int hs,he;

        time = m_pStartTime[0]->time();
        hs = time.hour();

        time = m_pEndTime[0]->time();
        he = time.hour();

        if (he >= hs)
        {
            for (iLoop = 0; iLoop < 7; iLoop++)
            {
               if ( Qt::Checked == m_pQCheckBoxWeekday[iLoop]->checkState())
               {
                   MiscParam.ulAllocMask |= 1 << iLoop;
               }
               else
               {
                   MiscParam.ulAllocMask &= ~(1 << iLoop);
               }
            }
            MiscParam.aiTubeCirTimeInfo[0] = hs;
            MiscParam.aiTubeCirTimeInfo[1] = he;
            MiscParam.iTubeCirDuration = m_pEditTC[0]->text().toInt();
        }

        
        MiscParam.RPumpSetting[0] = m_pRPumpSlider->value();
    }

    MainSaveMachineParam(giMachineIdx,MMParam);
    MainSaveTMParam(giMachineIdx,TMParam);
    MainSaveAlarmSetting(giMachineIdx,AlarmSettting);
    MainSaveSubModuleSetting(giMachineIdx,SubModSetting);
    MainSaveFMParam(giMachineIdx,FmParam);
    MainSavePMParam(giMachineIdx,PmParam);
    MainSaveCalParam(giMachineIdx,CalParam);
    MainSaveMiscParam(giMachineIdx,MiscParam);
    MainSaveMachineType(giMachineIdx);
    
    MainUpdateGlobalParam();


}

bool CfgDlg::eventFilter(QObject *watched, QEvent *event)
{

    return QDialog::eventFilter(watched,event);
}

void CfgDlg::closeEvent(QCloseEvent *event)
{
    (void)event;
}

void CfgDlg::Exit()
{
    close();
}

void CfgDlg::Have_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        qDebug()<<"HaveB2_Checked";
    }
    else
    {
        qDebug()<<"HaveB2_Release";
    }
}

void CfgDlg::Alarm_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        qDebug()<<"HaveB2_Checked";
    }
    else
    {
        qDebug()<<"HaveB2_Release";
    }
}

void CfgDlg::PM_PM1_CmbIndexChange(int index)
{
    (void)index;
    int iCurIdx = m_pCombBox[0]->currentIndex();

    switch(iCurIdx)
    {
    case 0:
        m_pEditPm[0]->setText(QString::number(1.0,'f',2));
        break;
    default:
        break;
    }
}
void CfgDlg::PM_PM2_CmbIndexChange(int index)
{
    (void)index;
    int iCurIdx = m_pCombBox[1]->currentIndex();

    switch(iCurIdx)
    {
    case 0:
        m_pEditPm[1]->setText(QString::number(0.3,'f',2));
        m_fPmCap[1] = 30;
        break;
    case 1:
        m_pEditPm[1]->setText(QString::number(0.6,'f',2));
        m_fPmCap[1] = 60;
        break;
    case 2:
        m_pEditPm[1]->setText(QString::number(1.0,'f',2));
        m_fPmCap[1] = 100;
        break;
    case 3:
        m_pEditPm[1]->setText(QString::number(1.0,'f',2));
        m_fPmCap[1] = 200;
        break;
    case 4:
        m_pEditPm[1]->setText(QString::number(1.0,'f',2));
        m_fPmCap[1] = 350;
        break;
    case 5:
        m_pEditPm[1]->setText(QString::number(1.0,'f',2));
        m_fPmCap[1] = 350;
        break;
    default:
        break;
    }
}
void CfgDlg::PM_PM3_CmbIndexChange(int index)
{
    (void)index;
    int iCurIdx = m_pCombBox[2]->currentIndex();

    switch(iCurIdx)
    {
    case 0:
        m_pEditPm[2]->setText(QString::number(0.3,'f',2));
        m_fPmCap[2] = 30;
        break;
    case 1:
        m_pEditPm[2]->setText(QString::number(0.6,'f',2));
        m_fPmCap[2] = 60;
        break;
    case 2:
        m_pEditPm[2]->setText(QString::number(1.0,'f',2));
        m_fPmCap[2] = 100;
        break;
    case 3:
        m_pEditPm[2]->setText(QString::number(1.0,'f',2));
        m_fPmCap[2] = 200;
        break;
    case 4:
        m_pEditPm[2]->setText(QString::number(1.0,'f',2));
        m_fPmCap[2] = 350;
        break;
    case 5:
        m_pEditPm[2]->setText(QString::number(1.0,'f',2));
        m_fPmCap[2] = 350;
        break;
    default:
        break;
    }

}

void CfgDlg::SM_btnGroupClicked(int index)
{
    int tmp = Qt::Checked == m_pQCheckBoxSm[index]->checkState() ? 1 : 0;

    if (tmp)
    {
        qDebug()<<"SM_Checked" << index;
    }
    else
    {
        qDebug()<<"SM_unhecked" << index;
    }
    

}

void CfgDlg::InfoUpdate(int iType,void *pData)
{

    if (!isVisible())
    {
        return ;
    }
    
    if (m_pWidgets[4] == m_pTblWidget->currentWidget()) 
    {
        switch(iType)
        {
        case DISP_NOT_ECO:
            {
                NOT_ECO_ITEM_STRU *pItem = (NOT_ECO_ITEM_STRU *)pData;  

                if (pItem->ucId < DISP_CAL_I_NUM)
                {
                    m_pEditCalState[pItem->ucId*2 + 0]->setText(QString::number(pItem->fValue,'f',3));
                    m_pEditCalState[pItem->ucId*2 + 1]->setText(QString::number(pItem->usValue));
                }
                else
                {
                    qDebug()<<"iType : " << iType << "ID : " << pItem->ucId;
                }
            }
            break;
        case DISP_NOT_FM:
            {
                NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pData;  

                if (pItem->ucId < DISP_CAL_F_NUM)
                {
                    m_pEditCalState[DISP_CAL_I_NUM*2 +  pItem->ucId]->setText(QString::number(pItem->ulValue));
                }
                else
                {
                    qDebug()<<"iType : " << iType << "ID : " << pItem->ucId;
                }
            }
            break;
        case DISP_NOT_PM:
            {
                NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pData;  
                
                if (pItem->ucId < DISP_CAL_P_NUM)
                {
                    m_pEditCalState[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + pItem->ucId]->setText(QString::number(pItem->ulValue));
                }
                else
                {
                    qDebug()<<"iType : " << iType << "ID : " << pItem->ucId;
                }
            }
            break;
        case DISP_NOT_GPUMP:
            {
            }
            break;
        case DISP_NOT_RPUMP:
            {
            }
            break;
        case DISP_NOT_EDI:
            {
            }
            break;
        case DISP_NOT_RECT:
            {
            }
            break;
        default:
            break;
        }
    }
}


