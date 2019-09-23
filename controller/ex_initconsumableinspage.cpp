#include "ex_initconsumableinspage.h"
#include "mainwindow.h"
#include "ExtraDisplay.h"
#include <QTableWidget>
#include <QMouseEvent>
#include <QHeaderView>
#include <QVBoxLayout>
#include "dconsumableinstallwidget.h"
#include "ex_hintdialog.h"

Ex_InitConsumableInsPage::Ex_InitConsumableInsPage(QObject *parent, CBaseWidget *widget, MainWindow *wndMain)
                                                  :CSubPage(parent, widget, wndMain)
{
    initUi();
    buildTranslation();

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_timer_event()));
}

void Ex_InitConsumableInsPage::switchLanguage()
{
    buildTranslation();
}

void Ex_InitConsumableInsPage::buildTranslation()
{
    m_pExNextBtn->setText(tr("Next"));
    m_pExBackBtn->setText(tr("Back"));

    QString strSuggest = tr("Note:") + "\n"
            + tr("Install the AC Pack after all other components with RFID chip are installed.") + "\n"
            + tr("Scan the final filter and install it to the dispenser after finishing system setting-up.");
    //扫描耗材
    //安装纯化柱
    m_pTitleLb->setText(tr("Follow the steps below to install and scan the chip for the consumables."));
    m_pSuggestLb[Type1]->setText(tr("Step 1: ") + tr("Scan the consumables on the RFID tags position") + "\n" + tr("Note:") + tr("Scan the final filter and install it to the dispenser after finishing system setting-up."));
    m_pSuggestLb[Type0]->setText(tr("Step 2: ") + tr("Install cartridges"));

    initTable();
}

void Ex_InitConsumableInsPage::initUi()
{
    m_pTitleLb = new QLabel(m_widget);
    m_pTitleLb->setWordWrap(true);
    m_pTitleLb->setStyleSheet("font-size:18pt; color:#FF0000; QFont::Bold");
    m_pTitleLb->setGeometry(10, 30, 780, 25);
    m_pTitleLb->hide();

    for(int i = 0; i < Consumable_Type_Num; ++i)
    {
        m_pSuggestLb[i] = new QLabel(m_widget);
        m_pSuggestLb[i]->setWordWrap(true);
        m_pSuggestLb[i]->setStyleSheet("font-size:18pt; color:#FF0000; QFont::Bold");

        m_pTableWidget[i] = new QTableWidget(m_widget);
        m_pTableWidget[i]->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        m_pTableWidget[i]->horizontalHeader()->hide();
        m_pTableWidget[i]->verticalHeader()->hide();
    }
    m_pSuggestLb[Type1]->setGeometry(10, 70, 780, 45);

    m_pExNextBtn = new QPushButton(m_widget);
    m_pExBackBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_ExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_ExBackBtn_clicked()));

    m_pExBackBtn->move(200, 510);
    m_pExNextBtn->move(500, 510);
}

void Ex_InitConsumableInsPage::initTable()
{
    initTypeOtherTable();
    initTypePackTable();
}

void Ex_InitConsumableInsPage::initTypePackTable()
{
    initPackConfig();

    m_coorMap[Type0].clear();

    m_pTableWidget[Type0]->setColumnCount(2);
    int rows = (m_list[Type0].size() + 1) / 2;
    m_pTableWidget[Type0]->setRowCount(rows);

    for(int i = 0; i < m_list[Type0].size(); ++i)
    {
        int row = i/2;
        int col = i%2;
        DConsumableInstallWidget* listWidget = new DConsumableInstallWidget;
        listWidget->setID(m_list[Type0].at(i));
        listWidget->setTag(m_map[Type0].value(m_list[Type0].at(i)).strName);

        QTableWidgetItem* item = new QTableWidgetItem;

        m_pTableWidget[Type0]->setCellWidget(row, col, listWidget);
        m_pTableWidget[Type0]->setItem(row, col, item);
        m_coorMap[Type0].insert(m_list[Type0].at(i), QPoint(row, col));
    }
    for(int i = 0; i < rows; ++i)
    {
        m_pTableWidget[Type0]->setRowHeight(i, 40);
    }

    int height = m_firstTableHeight + 125 + 20;
    m_pSuggestLb[Type0]->setGeometry(10, height, 780, 25);
    m_pTableWidget[Type0]->setGeometry(10, height + 35, 780, 40*rows + 4);
}

void Ex_InitConsumableInsPage::initTypeOtherTable()
{
    initOtherConfig();
    m_coorMap[Type1].clear();

    m_pTableWidget[Type1]->setColumnCount(2);
    int rows = (m_list[Type1].size() + 1) / 2;
    m_pTableWidget[Type1]->setRowCount(rows);

    for(int i = 0; i < m_list[Type1].size(); ++i)
    {
        int row = i/2;
        int col = i%2;
        DConsumableInstallWidget* listWidget = new DConsumableInstallWidget;
        listWidget->setID(m_list[Type1].at(i));
        listWidget->setTag(m_map[Type1].value(m_list[Type1].at(i)).strName);

        QTableWidgetItem* item = new QTableWidgetItem;

        m_pTableWidget[Type1]->setCellWidget(row, col, listWidget);
        m_pTableWidget[Type1]->setItem(row, col, item);
        m_coorMap[Type1].insert(m_list[Type1].at(i), QPoint(row, col));
    }
    for(int i = 0; i < rows; ++i)
    {
        m_pTableWidget[Type1]->setRowHeight(i, 40);
    }
    m_firstTableHeight = 40 * rows + 4;
    m_pTableWidget[Type1]->setGeometry(10, 125, 780, m_firstTableHeight);
}

void Ex_InitConsumableInsPage::updatePackInstall(int type)
{
    QPoint point = m_coorMap[Type0].value(type);
    int row = point.x();
    int col = point.y();
    QWidget* widget = m_pTableWidget[Type0]->cellWidget(row, col);
    DConsumableInstallWidget* listWidget = qobject_cast<DConsumableInstallWidget*>(widget);
    listWidget->setInstallStatus(true);
}

void Ex_InitConsumableInsPage::updateOtherInstall(int type)
{
    QPoint point = m_coorMap[Type1].value(type);
    int row = point.x();
    int col = point.y();
    QWidget* widget = m_pTableWidget[Type1]->cellWidget(row, col);
    DConsumableInstallWidget* listWidget = qobject_cast<DConsumableInstallWidget*>(widget);
    listWidget->setInstallStatus(true);
}

void Ex_InitConsumableInsPage::on_ExNextBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_InstallConsumable, 0);
    m_wndMain->prepareKeyStroke();
}

void Ex_InitConsumableInsPage::on_ExBackBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_InstallConsumable, 1);
    m_wndMain->prepareKeyStroke();
}

void Ex_InitConsumableInsPage::on_timer_event()
{
    activeReadRFID();
    m_pTimer->stop();
}

void Ex_InitConsumableInsPage::show(bool bShow)
{
    if (bShow)
    {
        m_wndMain->setStartCheckConsumable(true);
        m_pTimer->start(1000);
    }
    else
    {
        m_wndMain->setStartCheckConsumable(false);
    }
    CSubPage::show(bShow);
}

void Ex_InitConsumableInsPage::activeReadRFID()
{
    for(int i = 0; i < APP_RFID_SUB_TYPE_NUM; ++i)
    {
        m_wndMain->readRfid(i);
        m_wndMain->checkConsumableInstall(i);
    }
}

void Ex_InitConsumableInsPage::updateConsumableInstall(int type)
{
    if(m_list[Type1].contains(type))
    {
        updateOtherInstall(type);
        return;
    }

    if(m_list[Type0].contains(type))
    {
        updatePackInstall(type);
        return;
    }
}

void Ex_InitConsumableInsPage::mousePressEvent(QMouseEvent *e)
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

void Ex_InitConsumableInsPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void Ex_InitConsumableInsPage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        m_wndMain->naviInitPage(Ex_Init_InstallConsumable, m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}

void Ex_InitConsumableInsPage::setBackColor()
{
}

void Ex_InitConsumableInsPage::initPackConfig()
{
    m_list[Type0].clear();
    m_map[Type0].clear();
    Consumable_Install_Info install_info;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
        install_info.strName = tr("U Pack");
        m_map[Type0].insert(DISP_U_PACK, install_info);
        m_list[Type0].append(DISP_U_PACK);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        install_info.iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
        install_info.strName = tr("H Pack");
        m_map[Type0].insert(DISP_H_PACK, install_info);
        m_list[Type0].append(DISP_H_PACK);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("P Pack");
        m_map[Type0].insert(DISP_P_PACK, install_info);
        m_list[Type0].append(DISP_P_PACK);
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
        install_info.strName = tr("P Pack");
        m_map[Type0].insert(DISP_P_PACK, install_info);
        m_list[Type0].append(DISP_P_PACK);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("AC Pack");
        m_map[Type0].insert(DISP_AC_PACK, install_info);
        m_list[Type0].append(DISP_AC_PACK);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        install_info.iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
        install_info.strName = tr("AT Pack");
        m_map[Type0].insert(DISP_AT_PACK, install_info);
        m_list[Type0].append(DISP_AT_PACK);
        break;
    default:
        break;
    }
}

void Ex_InitConsumableInsPage::initOtherConfig()
{
    m_list[Type1].clear();
    m_map[Type1].clear();
    Consumable_Install_Info install_info;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("RO Membrane");
        m_map[Type1].insert(DISP_MACHINERY_RO_MEMBRANE, install_info);
        m_list[Type1].append(DISP_MACHINERY_RO_MEMBRANE);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        if(0 == ex_gGlobalParam.Ex_System_Msg.Ex_iCompany)
        {
             install_info.strName = tr("Final Fliter B");
        }
        else
        {
            install_info.strName = tr("Bio-filter");
        }
        m_map[Type1].insert(DISP_T_B_FILTER, install_info);
        m_list[Type1].append(DISP_T_B_FILTER);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Final Fliter A");
        m_map[Type1].insert(DISP_T_A_FILTER, install_info);
        m_list[Type1].append(DISP_T_A_FILTER);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("254 UV Lamp");
        m_map[Type1].insert(DISP_N1_UV, install_info);
        m_list[Type1].append(DISP_N1_UV);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("185 UV Lamp");
        m_map[Type1].insert(DISP_N2_UV, install_info);
        m_list[Type1].append(DISP_N2_UV);
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Tank UV Lamp");
        m_map[Type1].insert(DISP_N3_UV, install_info);
        m_list[Type1].append(DISP_N3_UV);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Tank Vent Filter");
        m_map[Type1].insert(DISP_W_FILTER, install_info);
        m_list[Type1].append(DISP_W_FILTER);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV))
        {
            install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            install_info.strName = tr("Tube UV");
            m_map[Type1].insert(DISP_N4_UV, install_info);
            m_list[Type1].append(DISP_N4_UV);
        }
        break;
    }
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTubeFilter))
        {
            install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            install_info.strName = tr("Loop Filter");
            m_map[Type1].insert(DISP_TUBE_FILTER, install_info);
            m_list[Type1].append(DISP_TUBE_FILTER);
        }
        break;
    }
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeDI))
        {
            install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            install_info.strName = tr("Loop DI");
            m_map[Type1].insert(DISP_TUBE_DI, install_info);
            m_list[Type1].append(DISP_TUBE_DI);
        }
        break;
    }
    default:
        break;
    }
}

