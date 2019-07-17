#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPainter>

#include "servicepage.h"
#include "mainwindow.h"
#include "titlebar.h"
#include "SterilizePage.h"
#include "ConsumableinsPage.h"
#include "allocationsetpage.h"
#include "ex_usercfgpage.h"
#include "reportpage.h"
#include "ex_historypage.h"
#include "LoginDlg.h"
#include "ex_userinfo.h"
#include "ExtraDisplay.h"
#include "ex_rfidcfgpage.h"
#include "ex_managersetpage.h"
#include "ex_permissionsetpage.h"

#include "ex_languagepage.h"
#include "dloginwarningdialog.h"
#include "setpage.h"
#include "ex_languagepage.h"
#include "unitpage.h"

#define BTNS_PER_ROW (4)

#define BTNS_H_MARGE (10)

#define BTNS_V_MARGE (10)

static QString SubPageName[SERVICE_BTN_NUMBER] = 
{    
    "Material Install",
    "ManagerConfig",
    "User Config",
    "Permission",
    "History",
    "Language",
    "Units",
    "Sterilize",
    "Allocation Set",
    "Service"
};

static CONFIG_BTN1 sBtns[SERVICE_BTN_NUMBER] = 
{
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
    {-1,-1,&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_ACTIVE],&gpGlobalPixmaps[GLOBAL_BMP_BTN_GENERAL_INACTIVE],BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL ,},
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
        case SERVICE_BTN_INSTALL:
            tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
            tmpWidget->setObjectName(SubPageName[index]);
            tmpWidget->setGeometry(0,0,800,600);
            m_pSubPages[SERVICE_BTN_INSTALL] = new ConsumableInsPage(this , tmpWidget , m_wndMain);
            break;
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
}

void ServicePage::buildTitles()
{
    QStringList stringList;
    
    stringList << tr("Service");

    setTitles(stringList);

}

void ServicePage::buildTranslation()
{
    int index;
    m_pBtns[SERVICE_BTN_STERILIZE]->setTip(tr("Cleaning & Sanitization"));
    m_pBtns[SERVICE_BTN_MANAGERCONFIG]->setTip(tr("Setting"));
    m_pBtns[SERVICE_BTN_INSTALL]->setTip(tr("Components Installation"));
    m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->setTip(tr("Dist. Control"));
    m_pBtns[SET_BTN_USER_CFG]->setTip(tr("User Config"));
    m_pBtns[SET_BTN_HISTORY_RECORD]->setTip(tr("History"));
    m_pBtns[SET_BTN_PERMISSION]->setTip(tr("Permission"));
    m_pBtns[SET_BTN_SERVICE]->setTip(tr("Service"));
    m_pBtns[SET_BTN_LANGUAGE]->setTip(tr("Language"));
    m_pBtns[SET_BTN_UNITS]->setTip(tr("Units"));

    for (index = 0; index < SERVICE_BTN_NUMBER; index++)
    {
        m_pBtns[index]->setFont(m_wndMain->getFont(GLOBAL_FONT_14));
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

    //QPixmap back(":/pic/navigator_bg.png");

    //QSize size(back.width(), back.height());
    
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    //p.drawPixmap(0,0,back);

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);


    rows = (SERVICE_BTN_NUMBER + BTNS_PER_ROW - 1) / BTNS_PER_ROW;

    itemWidth = (width() - BTNS_H_MARGE * 2) / BTNS_PER_ROW;

    yStartPos = height()/2 ;

    for( index = 0 ; index < SERVICE_BTN_NUMBER ; index++)
    {
        row = index / BTNS_PER_ROW;
        col = index % BTNS_PER_ROW;
        
        m_pBtns[index] = new CBitmapButton(m_widget,sBtns[index].enStyle,sBtns[index].enPicStyle,index);

        if (*sBtns[index].pBackBitmap)
        {
            m_pBtns[index]->setButtonPicture(*sBtns[index].pBackBitmap);
        }

        if (*sBtns[index].pFrontBitmap)
        {
            m_pBtns[index]->setPressPicture(*sBtns[index].pFrontBitmap);
        }

        xPos = BTNS_H_MARGE + col*itemWidth + col*BTNS_H_MARGE;
        
        yPos = yStartPos + (row - (rows/2))*105 - BTNS_V_MARGE/2;
        
        m_pBtns[index]->setGeometry(xPos,yPos,m_pBtns[index]->getPicWidth(),m_pBtns[index]->getPicHeight());

        m_pBtns[index]->setStyleSheet("background-color:transparent");
        connect(m_pBtns[index], SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
        m_pBtns[index]->show();
    }

    if(MACHINE_PURIST == gGlobalParam.iMachineType)
    {
        m_pBtns[SERVICE_BTN_STERILIZE]->enable(false);
        m_pBtns[SERVICE_BTN_STERILIZE]->hide();
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->enable(false);
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->hide();

        QPoint point = m_pBtns[SERVICE_BTN_STERILIZE]->pos();
        m_pBtns[SET_BTN_SERVICE]->move(point);

    }

    if(MACHINE_ADAPT == gGlobalParam.iMachineType)
    {
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->enable(false);
        m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->hide();

        QPoint point = m_pBtns[SET_BTN_SYSTEM_ALLOCATION]->pos();
        m_pBtns[SET_BTN_SERVICE]->move(point);
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

}

void ServicePage::on_btn_clicked(int index)
{
    printf("tmp = %d\r\n" , index);

    switch(index)
    {
    case SERVICE_BTN_STERILIZE:
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
        
            return;
        }
        break;
    case SERVICE_BTN_INSTALL:
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
        
            return;
        }
        break;
    }

    if (m_pSubPages[index])
    {
        show(false);
        //ex
        switch(index)
        {
        case SERVICE_BTN_INSTALL:
        case SET_BTN_HISTORY_RECORD:
        case SET_BTN_SYSTEM_ALLOCATION:
        case SERVICE_BTN_STERILIZE:
        case SERVICE_BTN_MANAGERCONFIG:
        {
            if(!user_LoginState.loginState()
            && !(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization)))
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
            else
            {
                m_pSubPages[index]->show(true);
            }
            break;
        }
        case SET_BTN_PERMISSION:
        case SET_BTN_USER_CFG:
        {
            Ex_UserInfo userInfo;
            DUserInfo userlog = m_wndMain->getLoginfo();
            bool isManager = userInfo.checkManagerInfo(userlog.m_strUserName);
            if((!user_LoginState.loginState())
               || (!isManager))
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
            else
            {
                m_pSubPages[index]->show(true);
            }
            break;
        }
        case SET_BTN_SERVICE:
        {
            Ex_UserInfo userInfo;
            DUserInfo userlog = m_wndMain->getLoginfo();
            bool iEngineer = userInfo.checkEngineerInfo(userlog.m_strUserName);
            if(!user_LoginState.loginState() || (!iEngineer))
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
            else
            {
                m_pSubPages[index]->show(true);
            }
        }
            break;
        default:
            break;

        }
    }
    //end
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

void ServicePage::userVerify(int index)
{
}

void ServicePage::managerVerify(int index)
{
}

void ServicePage::serviceVerify(int index)
{
}

