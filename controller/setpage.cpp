#include "setpage.h"
#include "mainwindow.h"
#include "setdevicepage.h"
#include "setpoint.h"
#include "systestpage.h"
#include "systemcfgpage.h"
#include "parametercalibration.h"
#include "maintenancecycle.h"
#include "networkpage.h"

#include "alarmsetpage.h"
#include "LoginDlg.h"
#include "ExtraDisplay.h"
#include "ex_superpowerpage.h"
#include "ex_usercfgpage.h"
#include "ex_userinfo.h"
#include "ex_factorytestpage.h"
#include "dloginwarningdialog.h"
#include "dpushbutton.h"

#define BTNS_PER_ROW (4)

#define BTNS_H_MARGE (15)

#define BTNS_V_MARGE (10)

static QString SubPageName[SET_BTN_NUMBER] = 
{
    "Maintain Period",
    "Alarm Set",
    "System Test",
    "Param Calibrate",
    "Set Point",
    "System Config",
    "Network Set",
    "device set",
    "Super Power",
    "Factory"
};

static ButtonInfo Btninfos[SET_BTN_NUMBER] =
{
    {QString("Consumables Life"), QString(":/ButtonIcon/lifetime.png")},
    {QString("Alarm Config."), QString(":/ButtonIcon/alarmconfig.png")},
    {QString("Function Test"), QString(":/ButtonIcon/functiontest.png")},
    {QString("Calibration"), QString(":/ButtonIcon/calibration.png")},
    {QString("Alarm Set Point"), QString(":/ButtonIcon/alarmsetpoint.png")},
    {QString("System Config."), QString(":/ButtonIcon/systemconfig.png")},
    {QString("Connectivity"), QString(":/ButtonIcon/connectivity.png")},
    {QString("Connecting Device"), QString(":/ButtonIcon/connectingdevice.png")},
    {QString("Initialize"), QString(":/ButtonIcon/factoryreset.png")},
    {QString("Super Power"), QString(":/ButtonIcon/super.png")},
    {QString("Factory"), QString(":/ButtonIcon/factory.png")}
};

SetPage::SetPage(QObject *parent,CBaseWidget *widget,MainWindow *wndMain) : CSubPage(parent,widget,wndMain) //CPage(parent,widget,wndMain)
{
    initUi();

    creatTitle();

    buildTranslation();

    createSubPage();
}

void SetPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void SetPage::createSubPage()
{
    int index;
    CBaseWidget *tmpWidget;
    
    for (index = 0; index < SET_BTN_NUMBER; index++)
    {
        m_pSubPages[index] = NULL;
        tmpWidget = NULL;

        switch(index)
        {
        case SET_BTN_MAINTAIN_PERIOD:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new MaintenanceCyclePage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_ALARM_SETTING:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new AlarmSetPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_TEST:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SysTestPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_PERIPHERAL_DEVICE_MANAGER:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SetDevicePage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_PARAM_CONFIG:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SetPoint(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_DEVICE_CONFIG:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SystemCfgPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_NETWORK:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new NetworkPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_PARAMETER_CALIBRATE:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new ParameterCalibrationPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SYSTEM_SUPER:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_SuperPowerPage(this, tmpWidget, m_wndMain);
            break;
        case SET_BTN_SYSTEM_FACTORYTEST:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_FactoryTestPage(this, tmpWidget, m_wndMain);
            break;
        }
    }
}

void SetPage::checkLoginInfo()
{
}

void SetPage::buildTitles()
{
    QStringList stringList;
    
    stringList << tr("Service");

    setTitles(stringList);

}

void SetPage::buildTranslation()
{ 
    m_pBtns[SET_BTN_MAINTAIN_PERIOD]->setText(tr("Consumables Life"));
    m_pBtns[SET_BTN_ALARM_SETTING]->setText(tr("Alarm Config."));
    m_pBtns[SET_BTN_SYSTEM_TEST]->setText(tr("Function Test"));
    m_pBtns[SET_BTN_SYSTEM_PARAMETER_CALIBRATE]->setText(tr("Calibration"));
    m_pBtns[SET_BTN_SYSTEM_PARAM_CONFIG]->setText(tr("Alarm Set Point"));
    m_pBtns[SET_BTN_SYSTEM_DEVICE_CONFIG]->setText(tr("System Config."));
    m_pBtns[SET_BTN_SYSTEM_NETWORK]->setText(tr("Connectivity"));

    m_pBtns[SET_BTN_PERIPHERAL_DEVICE_MANAGER]->setText(tr("Connecting Device"));
    m_pBtns[SET_BTN_INITIALIZE]->setText(tr("Initialize"));

    m_pBtns[SET_BTN_SYSTEM_SUPER]->setText(tr("Super Power"));
    m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->setText(tr("Factory"));

    for(int i = 0; i < SET_BTN_NUMBER; ++i)
    {
        QFont *font = m_wndMain->getFont(GLOBAL_FONT_14);
        m_pBtns[i]->setFont(*font);
    }

}

void SetPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void SetPage::initUi()
{
    int row,col,rows,index;

    int xPos,yPos,yStartPos;

    int itemWidth;

    int x;
    
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);

    rows = (SET_BTN_NUMBER + BTNS_PER_ROW - 1) / BTNS_PER_ROW;

    itemWidth = (width() - BTNS_H_MARGE * 2) / BTNS_PER_ROW;

    yStartPos = height()/2 - 60;


    for( index = 0 ; index < SET_BTN_NUMBER ; index++)
    {
        row = index / BTNS_PER_ROW;
        col = index % BTNS_PER_ROW;

        m_pBtns[index] = new DPushButton(Btninfos[index].m_fileName, Btninfos[index].m_text, m_widget, index);

        xPos = BTNS_H_MARGE + col*itemWidth + col*BTNS_H_MARGE + 70;

        yPos = yStartPos + (row - (rows/2))*120 - BTNS_V_MARGE/2;

        m_pBtns[index]->setMoveRule(DPushButton::CenterMove);
        m_pBtns[index]->cmove(xPos, yPos);
        connect(m_pBtns[index], SIGNAL(clicked()), this, SLOT(on_btn_clicked()));
    }
    /*
    if(MACHINE_PURIST == gGlobalParam.iMachineType)
    {
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->enable(false);
    }
    */
    x = 400 - PAGEID_MARGIN/2 - PAGEID_MARGIN - gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width()*2;

    m_pBtns[SET_BTN_SYSTEM_SUPER]->hide();
    m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->hide();

}

void SetPage::on_btn_clicked()
{
    QObject* obj = sender();
    DPushButton* button = qobject_cast<DPushButton*>(obj);
    int index = button->id();

    if(index == SET_BTN_INITIALIZE)
    {
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
            return;
        }

        Ex_UserInfo userInfo;
        DUserInfo userlog = m_wndMain->getLoginfo();
        bool iEngineer = userInfo.checkEngineerInfo(userlog.m_strUserName);
        if(!user_LoginState.loginState() || (!iEngineer))
        {
            m_pBtns[SET_BTN_SYSTEM_SUPER]->hide(); //
            m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->hide();
            LoginDlg dlg;
            dlg.exec() ;
            if(0 == dlg.m_iLogInResult)
            {
                Ex_UserInfo userInfo;
                int ret = userInfo.checkUserInfo(dlg.m_strUserName, dlg.m_strPassword);

                switch(ret)
                {
                case 3:
                case 31:
                case 4:
                {
                    m_wndMain->saveLoginfo(dlg.m_strUserName, dlg.m_strPassword);
                    user_LoginState.setLoginState(true);
                    if(3 != ret)
                    {
                        m_pBtns[SET_BTN_SYSTEM_SUPER]->show(); //
                        m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->show();
                    }
                    break;
                }
                case 1:
                case 2:
                    DLoginWarningDialog::getInstance(tr("User's privilege is low, please use the service account to log in!"));
                    break;
                case 0:
                    DLoginWarningDialog::getInstance(tr("Login failed!"));
                    break;
                default:
                    break;
                }
            }

        }
        else
        {
            toInitializePage();
        }

    }

    if(m_pSubPages[index])
    {
        show(false);
        switch(index)
        {
        case SET_BTN_MAINTAIN_PERIOD:
        case SET_BTN_ALARM_SETTING:
        case SET_BTN_SYSTEM_TEST:
        case SET_BTN_SYSTEM_PARAMETER_CALIBRATE:
        case SET_BTN_SYSTEM_PARAM_CONFIG:
        case SET_BTN_SYSTEM_DEVICE_CONFIG:
        case SET_BTN_SYSTEM_NETWORK:
        case SET_BTN_PERIPHERAL_DEVICE_MANAGER:
        case SET_BTN_SYSTEM_SUPER: //
        case SET_BTN_SYSTEM_FACTORYTEST:
        {
            Ex_UserInfo userInfo;
            DUserInfo userlog = m_wndMain->getLoginfo();
            bool iEngineer = userInfo.checkEngineerInfo(userlog.m_strUserName);
            if(!user_LoginState.loginState() || (!iEngineer))
            {
                m_pBtns[SET_BTN_SYSTEM_SUPER]->hide(); //
                m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->hide();
                LoginDlg dlg;
                dlg.exec() ;
                if(0 == dlg.m_iLogInResult)
                {
                    Ex_UserInfo userInfo;
                    int ret = userInfo.checkUserInfo(dlg.m_strUserName, dlg.m_strPassword);

                    switch(ret)
                    {
                    case 3:
                    case 31:
                    case 4:
                    {
                        m_wndMain->saveLoginfo(dlg.m_strUserName, dlg.m_strPassword);
                        m_pSubPages[index]->show(true);
                        user_LoginState.setLoginState(true);
                        if(3 != ret)
                        {
                            m_pBtns[SET_BTN_SYSTEM_SUPER]->show(); //
                            m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->show();
                        }
                        break;
                    }

                    case 2:
                    case 1:
                        show(true);
                        DLoginWarningDialog::getInstance(tr("User's privilege is low, please use the service account to log in!"));
                        break;
                    case 0:
                    {
                        show(true);
                        DLoginWarningDialog::getInstance(tr("Login failed!"));
                        break;
                    }
                    default:
                        show(true);
                        break;
                    }
                }
                else
                {
                    show(true);
                }

            }
            else
            {
                m_pSubPages[index]->show(true);
            }
            break;
        }
        default:
            break;
        }
    }
    m_wndMain->prepareKeyStroke();
}

void SetPage::toInitializePage()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL,
                                                           tr("NOTIFY"),
                                                           tr("Do you want to restart the system immediately\n to enter the initialization interface?"),
                                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(rb != QMessageBox::Yes)
    {
        return;
    }
    ex_gGlobalParam.Ex_Default = 0;
    MainSaveDefaultState(gGlobalParam.iMachineType);
    m_wndMain->restart();
}

void SetPage::setSuperPage(bool bShow)
{
    if(bShow)
    {
        m_pBtns[SET_BTN_SYSTEM_SUPER]->show();
        m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->show();
    }
    else
    {
        m_pBtns[SET_BTN_SYSTEM_SUPER]->hide();
        m_pBtns[SET_BTN_SYSTEM_FACTORYTEST]->hide();
    }
}


