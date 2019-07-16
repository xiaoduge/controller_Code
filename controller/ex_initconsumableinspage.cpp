#include "ex_initconsumableinspage.h"
#include "mainwindow.h"
#include <QTableWidget>
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
    m_pSuggestLb->setText(strSuggest);
    initTable();
}

void Ex_InitConsumableInsPage::initUi()
{
    m_pSuggestLb = new QLabel(m_widget);
    m_pSuggestLb->setWordWrap(true);
    m_pSuggestLb->setGeometry(10, 50, 780, 80);
    m_pSuggestLb->setStyleSheet("font-size:18pt; color:#FF0000; QFont::Bold");

    m_pTableWidget = new QTableWidget(m_widget);
    m_pTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_pTableWidget->horizontalHeader()->hide();
    m_pTableWidget->verticalHeader()->hide();

    m_pTableWidget->setGeometry(10, 140, 780, 350);

    m_pExNextBtn = new QPushButton(m_widget);
    m_pExBackBtn = new QPushButton(m_widget);

    connect(m_pExNextBtn, SIGNAL(clicked()), this, SLOT(on_ExNextBtn_clicked()));
    connect(m_pExBackBtn, SIGNAL(clicked()), this, SLOT(on_ExBackBtn_clicked()));

    m_pExBackBtn->move(200, 510);
    m_pExNextBtn->move(500, 510);
}

void Ex_InitConsumableInsPage::initTable()
{
    initVector();
    m_coorMap.clear();

    m_pTableWidget->setColumnCount(2);
    int rows = (m_list.size() + 1) / 2;
    m_pTableWidget->setRowCount(rows);

    for(int i = 0; i < m_list.size(); ++i)
    {
        int row = i/2;
        int col = i%2;
        DConsumableInstallWidget* listWidget = new DConsumableInstallWidget;
        listWidget->setID(m_list.at(i));
        listWidget->setTag(m_map.value(m_list.at(i)).strName);

        QTableWidgetItem* item = new QTableWidgetItem;

        m_pTableWidget->setCellWidget(row, col, listWidget);
        m_pTableWidget->setItem(row, col, item);
        m_coorMap.insert(m_list.at(i), QPoint(row, col));
    }
    for(int i = 0; i < rows; ++i)
    {
        m_pTableWidget->setRowHeight(i, 40);
    }
}

void Ex_InitConsumableInsPage::on_ExNextBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_InstallConsumable, 0);
}

void Ex_InitConsumableInsPage::on_ExBackBtn_clicked()
{
    m_wndMain->naviInitPage(Ex_Init_InstallConsumable, 1);
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
    QPoint point = m_coorMap.value(type);
    int row = point.x();
    int col = point.y();
    QWidget* widget = m_pTableWidget->cellWidget(row, col);
    DConsumableInstallWidget* listWidget = qobject_cast<DConsumableInstallWidget*>(widget);
    listWidget->setInstallStatus(true);

//    Ex_HintDialog::getInstance(QString("install: %1").arg(type));
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

void Ex_InitConsumableInsPage::initVector()
{
    m_list.clear();
    m_map.clear();
    Consumable_Install_Info install_info;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
        install_info.strName = tr("U Pack");
        m_map.insert(DISP_U_PACK, install_info);
        m_list.append(DISP_U_PACK);
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
        install_info.strName = tr("H Pack");
        m_map.insert(DISP_H_PACK, install_info);
        m_list.append(DISP_H_PACK);
        break;
    case MACHINE_L_EDI_LOOP:
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
        install_info.strName = tr("H Pack");
        m_map.insert(DISP_H_PACK, install_info);
        m_list.append(DISP_H_PACK);
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
    case MACHINE_ADAPT:
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
        install_info.strName = tr("P Pack");
        m_map.insert(DISP_P_PACK, install_info);
        m_list.append(DISP_P_PACK);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("AC Pack");
        m_map.insert(DISP_AC_PACK, install_info);
        m_list.append(DISP_AC_PACK);
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
    case MACHINE_ADAPT:
        install_info.iType = 1; //0
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("RO Membrane");
        m_map.insert(DISP_MACHINERY_RO_MEMBRANE, install_info);
        m_list.append(DISP_MACHINERY_RO_MEMBRANE);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Final Fliter B");
        m_map.insert(DISP_T_B_FILTER, install_info);
        m_list.append(DISP_T_B_FILTER);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Final Fliter A");
        m_map.insert(DISP_T_A_FILTER, install_info);
        m_list.append(DISP_T_A_FILTER);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("254 UV Lamp");
        m_map.insert(DISP_N1_UV, install_info);
        m_list.append(DISP_N1_UV);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("185 UV Lamp");
        m_map.insert(DISP_N2_UV, install_info);
        m_list.append(DISP_N2_UV);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Tank UV Lamp");
        m_map.insert(DISP_N3_UV, install_info);
        m_list.append(DISP_N3_UV);
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
        install_info.iType = 0;
        install_info.iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        install_info.strName = tr("Tank Vent Filter");
        m_map.insert(DISP_W_FILTER, install_info);
        m_list.append(DISP_W_FILTER);
        break;
    default:
        break;
    }
}

