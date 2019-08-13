#include <QMouseEvent>

#include "servicepage.h"
#include "mainwindow.h"
#include "titlebar.h"
#include "SterilizePage.h"
#include "ConsumableinsPage.h"
#include "allocationsetpage.h"
#include "ex_usercfgpage.h"
#include "ex_historypage.h"
#include "LoginDlg.h"
#include "ex_userinfo.h"
#include "ExtraDisplay.h"
#include "ex_rfidcfgpage.h"
#include "ex_managersetpage.h"
#include "ex_permissionsetpage.h"

#include "ex_languagepage.h"
#include "dloginwarningdialog.h"
#include "ex_hintdialog.h"
#include "ex_languagepage.h"
#include "unitpage.h"
#include "dpushbutton.h"

#define BTNS_PER_ROW (4)

#define BTNS_H_MARGE (15)

#define BTNS_V_MARGE (10)

static QString SubPageName[SERVICE_BTN_NUMBER] = 
{    
    "Language",
    "Units",
//    "Material Install",
    "ManagerConfig",
    "User Config",
    "Permission",
    "History",
    "Sterilize",
    "Allocation Set",
    "Service"
};

static ButtonInfo Btninfos[SERVICE_BTN_NUMBER] =
{
    {QString("Select Language"), QString(":/ButtonIcon/language.png")},
    {QString("Units"), QString(":/ButtonIcon/units.png")},
    {QString("Setting"), QString(":/ButtonIcon/setting.png")},
    {QString("User Config"), QString(":/ButtonIcon/userconfig.png")},
    {QString("Permission"), QString(":/ButtonIcon/permission.png")},
    {QString("History"), QString(":/ButtonIcon/history.png")},
    {QString("Cleaning & Sanitization"), QString(":/ButtonIcon/cleaning.png")},
    {QString("Dist. Control"), QString(":/ButtonIcon/distcontrol.png")},
    {QString("Service"), QString(":/ButtonIcon/service.png")},
};

ServicePage::ServicePage(QObject *parent,CBaseWidget *widget,MainWindow *wndMain) : CPage(parent,widget,wndMain)
{
    initUi();

    creatTitle();

    buildTranslation();

    Create_subPage();
}

void ServicePage::creatTitle()
{
    CPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void ServicePage::Create_subPage()
{
    int index;
    
    CBaseWidget *tmpWidget;
    
    for (index = 0; index < SERVICE_BTN_NUMBER; index++)
    {
        m_pSubPages[index] = NULL;
        tmpWidget = NULL;

        switch(index)
        {
        case SERVICE_BTN_STERILIZE:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[SERVICE_BTN_STERILIZE] = new SterilizePage(this , tmpWidget , m_wndMain);
            break;
        case SERVICE_BTN_MANAGERCONFIG:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[SERVICE_BTN_MANAGERCONFIG] = new Ex_ManagerSetPage(this , tmpWidget , m_wndMain);
            break;
#if 0
        case SERVICE_BTN_INSTALL:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[SERVICE_BTN_INSTALL] = new ConsumableInsPage(this , tmpWidget , m_wndMain);
            break;
#endif
        case SET_BTN_SYSTEM_ALLOCATION:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new AllocationSetPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_USER_CFG:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_UserCfgPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_HISTORY_RECORD:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_HistoryPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_PERMISSION:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_PermissionSetPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_LANGUAGE:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new Ex_Languagepage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_UNITS:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new UnitPage(this , tmpWidget , m_wndMain);
            break;
        case SET_BTN_SERVICE:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[index] = new SetPage(this , tmpWidget , m_wndMain);
            break;
        }

    }

}

void ServicePage::update()
{
    if(user_LoginState.loginState())
    {
        m_pLogoutBtn->show();
    }
    else
    {
        m_pLogoutBtn->hide();
    }
}

void ServicePage::buildTitles()
{
    QStringList stringList;
    
    stringList << tr("Service");

    setTitles(stringList);

}

void ServicePage::buildTranslation()
{
    m_pBtns[SERVICE_BTN_STERILIZE]->setText(tr("Cleaning & Sanitization"));
    m_pBtns[SERVICE_BTN_MANAGERCONFIG]->setText(tr("Setting"));
    m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->setText(tr("Dist. Control"));
    m_pBtns[SET_BTN_USER_CFG]->setText(tr("User Config"));
    m_pBtns[SET_BTN_HISTORY_RECORD]->setText(tr("History"));
    m_pBtns[SET_BTN_PERMISSION]->setText(tr("Permission"));
    m_pBtns[SET_BTN_SERVICE]->setText(tr("Service"));
    m_pBtns[SET_BTN_LANGUAGE]->setText(tr("Select Language"));
    m_pBtns[SET_BTN_UNITS]->setText(tr("Units"));

    for(int i = 0; i < SERVICE_BTN_NUMBER; ++i)
    {
        QFont *font = m_wndMain->getFont(GLOBAL_FONT_14);
        m_pBtns[i]->setTextFont(*font);
    }
}

void ServicePage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}


void ServicePage::initUi()
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

    rows = (SERVICE_BTN_NUMBER + BTNS_PER_ROW - 1) / BTNS_PER_ROW;

    itemWidth = (width() - BTNS_H_MARGE * 2) / BTNS_PER_ROW;

    yStartPos = height()/2  - 60;

    for( index = 0 ; index < SERVICE_BTN_NUMBER ; index++)
    {
        row = index / BTNS_PER_ROW;
        col = index % BTNS_PER_ROW;

        m_pBtns[index] = new DPushButton(Btninfos[index].m_fileName, Btninfos[index].m_text, m_widget, index);

        xPos = BTNS_H_MARGE + col*itemWidth + col*BTNS_H_MARGE + 60;

        yPos = yStartPos + (row - (rows/2))*120 - BTNS_V_MARGE/2;

        m_pBtns[index]->setMoveRule(DPushButton::CenterMove);
        m_pBtns[index]->cmove(xPos, yPos);

        connect(m_pBtns[index], SIGNAL(clicked()), this, SLOT(on_btn_clicked()));
    }

    if(MACHINE_PURIST == gGlobalParam.iMachineType)
    {
        m_pBtns[SERVICE_BTN_STERILIZE]->hide();
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->hide();

        QPoint point = m_pBtns[SERVICE_BTN_STERILIZE]->originalPos();
        m_pBtns[SET_BTN_SERVICE]->cmove(point);
    }

    if(MACHINE_ADAPT == gGlobalParam.iMachineType)
    {
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->hide();

        QPoint point = m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->originalPos();
        m_pBtns[SET_BTN_SERVICE]->cmove(point);
    }

    x = 400 - PAGEID_MARGIN/2 - PAGEID_MARGIN - gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width()*2;
    for ( index = 0 ; index < 3 ; index++)
    {
        m_pLbPageId[index] = new QLabel(m_widget);
        m_pLbPageId[index]->setGeometry(x ,560,gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width(),gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->height());
        x += (PAGEID_MARGIN + gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]->width());

        if (index == 2)
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_SELECT]);
        }
        else
        {
            m_pLbPageId[index]->setPixmap(*gpGlobalPixmaps[GLOBAL_BMP_PAGE_UNSELECT]);
        }
    }
    
    m_pTitleBar = new titleBar(m_widget);

    m_pTitleBar->setGeometry(0,0,800,55);
    
    connect(m_pTitleBar, SIGNAL(clicked(int)), this, SLOT(on_navi_clicked(int)));

    QPixmap pixmap(":/pic/logout.png");
    m_pLogoutBtn = new DPushButton(pixmap, m_widget);
    m_pLogoutBtn->cmove(740, 545);
    connect(m_pLogoutBtn, SIGNAL(clicked()), this, SLOT(on_logoutBtn_clicked()));
}

void ServicePage::on_btn_clicked(/* int index */)
{
    QObject* obj = sender();
    DPushButton* button = qobject_cast<DPushButton*>(obj);
    int index = button->id();
    switch(index)
    {
    case SERVICE_BTN_STERILIZE:
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
        
            return;
        }
        break;
#if 0
    case SERVICE_BTN_INSTALL:
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
        
            return;
        }
        break;
#endif
    }

    if (m_pSubPages[index])
    {
        show(false);
        switch(index)
        {
        case SET_BTN_LANGUAGE:
        case SET_BTN_UNITS:
            notVerify(index);
            break;
//        case SERVICE_BTN_INSTALL:
        case SET_BTN_HISTORY_RECORD:
        case SET_BTN_SYSTEM_ALLOCATION:
        case SERVICE_BTN_STERILIZE:
        case SERVICE_BTN_MANAGERCONFIG:
            userVerify(index);
            break;
        case SET_BTN_PERMISSION:
        case SET_BTN_USER_CFG:
            managerVerify(index);
            break;
        case SET_BTN_SERVICE:
            serviceVerify(index);
            break;
        default:
            break;

        }
    }
    m_wndMain->prepareKeyStroke();
}

void ServicePage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();

        m_curX = e->x();
        m_curY = e->y();
        
        m_lstFlag = 1;
    }
}

void ServicePage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}


void ServicePage::mouseReleaseEvent(QMouseEvent *e)
{

    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
#ifdef FLOWCHART
        m_wndMain->naviPage(3,m_curX - m_lstX > 0 ? 1 : 0);
#else
        m_wndMain->naviPage(2,m_curX - m_lstX > 0 ? 1 : 0);
#endif
    }
    m_lstFlag = 0;
}


void ServicePage::on_navi_clicked(int index)
{
    switch(index)
    {
    case TITLE_BAR_ID_POWER_OFF:
        break;
    case TITLE_BAR_ID_BACK:
        break;
    case TITLE_BAR_ID_HOME:
        show(false);
        m_wndMain->home();
        break;
    }
    printf("tmp = %d\r\n" , index);
}

void ServicePage::on_logoutBtn_clicked()
{
    if(user_LoginState.loginState())
    {
        user_LoginState.setLoginState(false);
        m_pLogoutBtn->hide();
    }
    Ex_HintDialog::getInstance(tr("Signed out"));
}

void ServicePage::notVerify(int index)
{
    m_pSubPages[index]->show(true);
}

void ServicePage::userVerify(int index)
{
    if((gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
     &&(index != SERVICE_BTN_MANAGERCONFIG))
    {
        m_pSubPages[index]->show(true);
        return;
    }

    if(user_LoginState.loginState())
    {
        m_pSubPages[index]->show(true);
    }

    else
    {
        LoginDlg dlg;
        dlg.exec();
        if(0 == dlg.m_iLogInResult)
        {
            Ex_UserInfo userInfo;
            int ret = userInfo.checkUserInfo(dlg.m_strUserName, dlg.m_strPassword);
            switch(ret)
            {
            case 4:
            case 3:
            case 2:
            case 1:
            {
                m_wndMain->saveLoginfo(dlg.m_strUserName, dlg.m_strPassword);
                m_pSubPages[index]->show(true);
                user_LoginState.setLoginState(true);
                break;
            }
            case 0:
            {
                show(true);
                DLoginWarningDialog::getInstance(tr("Login failed!"));
                break;
            }
            default:
                break;
            }
        }
        else
        {
            show(true);
        }

    }
}

void ServicePage::managerVerify(int index)
{
    Ex_UserInfo userInfo;
    DUserInfo userlog = m_wndMain->getLoginfo();
    bool isManager = userInfo.checkManagerInfo(userlog.m_strUserName);

    if(user_LoginState.loginState() && isManager)
    {
        m_pSubPages[index]->show(true);
    }
    else
    {
        LoginDlg dlg;
        dlg.exec() ;
        if(0 == dlg.m_iLogInResult)
        {

            int ret = userInfo.checkUserInfo(dlg.m_strUserName, dlg.m_strPassword);
            switch(ret)
            {
            case 4:
            case 3:
            case 2:
            {
                m_wndMain->saveLoginfo(dlg.m_strUserName, dlg.m_strPassword);
                m_pSubPages[index]->show(true);
                user_LoginState.setLoginState(true);
                break;
            }
            case 1:
                show(true);
                DLoginWarningDialog::getInstance(tr("User's privilege is low, please use the manager account or service account to log in!"));
                break;
            case 0:
                show(true);
                DLoginWarningDialog::getInstance(tr("Login failed!"));
                break;
            default:
                break;
            }
        }
        else
        {
            show(true);
        }

    }
}

void ServicePage::serviceVerify(int index)
{
    Ex_UserInfo userInfo;
    DUserInfo userlog = m_wndMain->getLoginfo();
    bool isEngineer = userInfo.checkEngineerInfo(userlog.m_strUserName);

    if(user_LoginState.loginState() && isEngineer )
    {
        m_pSubPages[index]->show(true);
    }
    else
    {
        LoginDlg dlg;
        dlg.exec() ;
        if(0 == dlg.m_iLogInResult)
        {
            Ex_UserInfo userInfo;
            int ret = userInfo.checkUserInfo(dlg.m_strUserName, dlg.m_strPassword);
            SetPage* setpage = qobject_cast<SetPage*>(m_pSubPages[index]);
            setpage->setSuperPage(false);
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
                    SetPage* setpage = qobject_cast<SetPage*>(m_pSubPages[index]);
                    setpage->setSuperPage(true);
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

}




