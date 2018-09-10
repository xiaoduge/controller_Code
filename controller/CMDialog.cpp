#include "CMDialog.h"
#include <QInputDialog>
#include <QComboBox>
#include <QRect>
#include <QPushButton>
#include <QFileDialog>
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

#include "keyboard.h"
#include "Display.h"
#include "MyParams.h"

#include "PermissionDlg.h"


extern QString gastrCMCfgName[] ;

extern QString INSERT_sql_Log ;

extern QString INSERT_sql_Water ;

extern QString gastrCMActionName[];

CMDlg::CMDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("CM MGR"));
    int iLoop = 0 ,LENum = 0;
    QString strTemp;

    Qt::WindowFlags flags=Qt::Dialog;
    //flags |=Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    installEventFilter(this);

    m_pMainLayout   = new QGridLayout(this);

    m_pBtnGroup = new QButtonGroup;


    for(iLoop = 0;iLoop < DISP_CM_NUM; iLoop++)
    {
        strTemp  = gastrCMCfgName[iLoop];
        strTemp += ":";
        
        m_pLabelCM[iLoop] = new QLabel;
        m_pLabelCM[iLoop]->setText(strTemp);

        m_pMainLayout->addWidget(m_pLabelCM[iLoop],iLoop,1);

        m_pEditCMCfg[iLoop] = new QLineEdit;
        m_pEditCMCfg[iLoop]->setFixedSize(48,18);
        m_pMainLayout->addWidget(m_pEditCMCfg[iLoop],iLoop,2);
        m_pEditCMCfg[iLoop]->installEventFilter(this);
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCMCfg[iLoop];
        
        m_pEditCMCur[iLoop] = new QLineEdit;
        m_pEditCMCur[iLoop]->setFixedSize(48,18);
        m_pMainLayout->addWidget(m_pEditCMCur[iLoop],iLoop,3);
        m_pEditCMCur[iLoop]->installEventFilter(this);
        LineEdit_TYPE[LENum++].m_pEditName = m_pEditCMCur[iLoop];
        m_pEditCMCur[iLoop]->setReadOnly(true); /* read only */

        m_pBtnCM[iLoop] = new QPushButton;
        m_pMainLayout->addWidget(m_pBtnCM[iLoop],iLoop,4);
        m_pBtnCM[iLoop]->setObjectName(QString::fromUtf8("Reset"));
        m_pBtnCM[iLoop]->setText("Reset");

        m_pBtnGroup->addButton(m_pBtnCM[iLoop],iLoop);

    }


    m_pBtnSave = new QPushButton();
    m_pBtnSave->setObjectName(QString::fromUtf8("Save"));
    m_pBtnSave->setText("Save");

    m_pBtnExit = new QPushButton();
    m_pBtnExit->setObjectName(QString::fromUtf8("Exit"));
    m_pBtnExit->setText("Exit");


    m_pMainLayout->addWidget(m_pBtnSave,DISP_CM_NUM + 1,5);
    m_pMainLayout->addWidget(m_pBtnExit,DISP_CM_NUM + 2,5);

    setLayout(m_pMainLayout);

    CFGCom();
    
    connect(m_pBtnSave,SIGNAL(clicked()),this,SLOT(SaveCfg()));
    connect(m_pBtnExit,SIGNAL(clicked()),this,SLOT(Exit()));

    connect( m_pBtnGroup, SIGNAL(buttonClicked (int)), this, SLOT(resetClick(int)) );

}

CMDlg::~CMDlg()
{

    delete m_pBtnSave;
    delete m_pBtnExit;
    delete m_pMainLayout;
    
}

void CMDlg::showCmInfo()
{
    int iId;
    for(iId = 0; iId < DISP_CM_NUM ; iId++)
    {
        switch(iId)
        {
        case DISP_P_PACKLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));

                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_P_PACK))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
            }
            break;
        case DISP_P_PACKLIFEL:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));

            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_P_PACK))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            
            break;
        case DISP_U_PACKLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));

                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_U_PACK))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
                
            }
            break;
        case DISP_U_PACKLIFEL:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_U_PACK))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            
            break;
        case DISP_PRE_PACKLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_H_PACK))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
                
            }
            break;
        case DISP_PRE_PACKLIFEL:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_H_PACK))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            
            break;
        case DISP_AT_PACKLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_AT_PACK))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
                
            }
            break;
        case DISP_AT_PACKLIFEL:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_AT_PACK))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            
            break;
        case DISP_N1_UVLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN1))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
                
            }
            break;
        case DISP_N1_UVLIFEHOUR:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN1))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            break;
        case DISP_N2_UVLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN2))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
            }
            break;
        case DISP_N2_UVLIFEHOUR:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN2))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            break;
        case DISP_N3_UVLIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                m_pEditCMCur[iId]->setText(QString::number(tmp));
                if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN3))
                {
                    m_pEditCMCfg[iId]->setEnabled(true);
                    m_pEditCMCur[iId]->setEnabled(true);
                    m_pBtnCM[iId]->setEnabled(true);
                }
                else
                {
                    m_pEditCMCfg[iId]->setEnabled(false);
                    m_pEditCMCur[iId]->setEnabled(false);
                    m_pBtnCM[iId]->setEnabled(false);
                }
            }
            break;
        case DISP_N3_UVLIFEHOUR:
            m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveN3))
            {
                m_pEditCMCfg[iId]->setEnabled(true);
                m_pEditCMCur[iId]->setEnabled(true);
                m_pBtnCM[iId]->setEnabled(true);
            }
            else
            {
                m_pEditCMCfg[iId]->setEnabled(false);
                m_pEditCMCur[iId]->setEnabled(false);
                m_pBtnCM[iId]->setEnabled(false);
            }
            break;
        case DISP_T_B_FILTERLIFE:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
            }
            break;
        case DISP_T_A_FILTERLIFE:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
            }
            break;
        case DISP_W_FILTERLIFE:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
            }
            break;
        case DISP_ROC12LIFEDAY:
            {
                int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
                
                m_pEditCMCur[iId]->setText(QString::number(tmp));
            }
            break;
        default:
            break;
        }    

    }

}

void CMDlg::CFGCom()
{
    int iLoop;

    /* Machine paramter */
    for(iLoop = 0; iLoop < DISP_CM_NUM ; iLoop++)
    {
        m_pEditCMCfg[iLoop]->setText(QString::number(gGlobalParam.CMParam.aulCms[iLoop]));
    }

    showCmInfo();
}

void CMDlg::resetItem(int iId,QString &strName)
{
    MainResetCmInfo(iId);

    switch(iId)
    {
    case DISP_P_PACKLIFEDAY:
    case DISP_U_PACKLIFEDAY:
    case DISP_PRE_PACKLIFEDAY:
    case DISP_N1_UVLIFEDAY:
    case DISP_N2_UVLIFEDAY:
    case DISP_N3_UVLIFEDAY:
    case DISP_ROC12LIFEDAY:
        {
            int tmp = gGlobalParam.CMParam.aulCms[iId] - ((DispGetCurSecond() - gCMUsage.info.aulCms[iId])/DISP_DAYININSECOND);
            
            m_pEditCMCur[iId]->setText(QString::number(tmp));
        }
        break;
    default:
        m_pEditCMCur[iId]->setText(QString::number(gCMUsage.info.aulCms[iId]));
        break;
    }

   {
       /* write to log */
       QSqlQuery query;

       time_t timer = time(NULL);
       struct tm *tblock = localtime(&timer);
       QString strTime   = asctime(tblock);
       
       query.prepare(INSERT_sql_Water);
       query.bindValue(":Name", strName);
       query.bindValue(":action",gastrCMActionName[iId]);
       query.bindValue(":time", strTime);
       query.exec();
       
   }

}

void CMDlg::resetClick(int iId)
{

    // ask for permission
    PermissionDlg dlg;

    dlg.m_eventId = iId;
    dlg.m_pParent = this;

    dlg.exec();
}

void CMDlg::SaveCfg()
{
    int iLoop ;

    for(iLoop = 0; iLoop < DISP_CM_NUM ; iLoop++)
    {
        int  iValue = m_pEditCMCfg[iLoop]->text().toInt();
    
        gGlobalParam.CMParam.aulCms[iLoop] = iValue;

    }

    MainSaveCMParam(gGlobalParam.iMachineType,gGlobalParam.CMParam);

    MainSaveCMInfo(gGlobalParam.iMachineType,gCMUsage.info);

}

bool CMDlg::eventFilter(QObject *watched, QEvent *event)
{
    //int SpnN = 0;

    if (watched == this)
    {
        if(QEvent::WindowActivate == event->type())  
        {  
            showCmInfo();
            
            return true ;  
        }      
    }


    return QDialog::eventFilter(watched,event);
}

void CMDlg::closeEvent(QCloseEvent *event)
{
    (void)event;
}

void CMDlg::Exit()
{
    close();
}



