#include "ex_usercfgpage.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>

#include "ex_comboboxdelegate.h"
#include "mainwindow.h"

#include <unistd.h>


Ex_UserCfgPage::Ex_UserCfgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
    initDBTabelModel();
}

void Ex_UserCfgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_UserCfgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("User Config");

    setTitles(stringList);

}

void Ex_UserCfgPage::buildTranslation()
{
    m_pQueryBtn->setText(tr("Query"));
    m_pDisplayAllBtn->setText(tr("Display All"));
    m_pSubmitBtn->setText(tr("Submit"));
    m_pRevertBtn->setText(tr("Revert"));
    m_pAddBtn->setText(tr("Add"));
    m_pDeleteRowBtn->setText(tr("Delete"));

    m_pPageHeaderLB->setText(tr("User Config"));
    m_pNameLB->setText(tr("Name"));
}

void Ex_UserCfgPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_UserCfgPage::setBackColor()
{
    QSize size(width(),height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(228, 231, 240));
    QPalette pal(m_widget->palette());
    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));
    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void Ex_UserCfgPage::createControl()
{
    QWidget* tmpWidget = new QWidget(m_widget);
//    QPalette pal(tmpWidget->palette());
//    pal.setColor(QPalette::Background, QColor(255,255,255));
//    tmpWidget->setPalette(pal);
    tmpWidget->setAutoFillBackground(false);

    tmpWidget->setGeometry(QRect(50, 65, 700, 500));

    m_pPageHeaderLB = new QLabel;
    QString qssHeaderLB = "QLabel{font-size:18px;\
                                  color:#000000;\
                                  font-family:Arial;\
                                  font-weight:bold;}";
    m_pPageHeaderLB->setStyleSheet(qssHeaderLB);

    m_pTableView = new QTableView;

    m_pNameLB = new QLabel;
    m_pNameLineEdit = new QLineEdit;
    m_pQueryBtn = new QPushButton;
    m_pDisplayAllBtn = new QPushButton;

    m_pSubmitBtn = new QPushButton;
    m_pRevertBtn = new QPushButton;
    m_pAddBtn = new QPushButton;
    m_pDeleteRowBtn = new QPushButton;

    QHBoxLayout* h0Layout = new QHBoxLayout;
    QHBoxLayout* h1Layout = new QHBoxLayout;
    QHBoxLayout* h2Layout = new QHBoxLayout;
    QHBoxLayout* h3Layout = new QHBoxLayout;

    h0Layout->addStretch(1);
    h0Layout->addWidget(m_pPageHeaderLB);
    h0Layout->addStretch(29);

    h1Layout->addWidget(m_pTableView);

    m_pNameLineEdit->setMaximumWidth(250);
    m_pNameLineEdit->setMinimumWidth(200);
    m_pQueryBtn->setMinimumWidth(150);
    m_pDisplayAllBtn->setMinimumWidth(150);
    h2Layout->addWidget(m_pNameLB);
    h2Layout->addWidget(m_pNameLineEdit);
    h2Layout->addStretch(1);
    h2Layout->addWidget(m_pQueryBtn);
    h2Layout->addWidget(m_pDisplayAllBtn);

    h3Layout->addWidget(m_pSubmitBtn);
    h3Layout->addWidget(m_pRevertBtn);
    h3Layout->addWidget(m_pAddBtn);
    h3Layout->addWidget(m_pDeleteRowBtn);

    QVBoxLayout* vLayout = new QVBoxLayout;

    vLayout->addLayout(h0Layout);
    vLayout->addLayout(h1Layout);
    vLayout->addLayout(h2Layout);
    vLayout->addLayout(h3Layout);

    tmpWidget->setLayout(vLayout);

    connect(m_pQueryBtn, SIGNAL(clicked()), this, SLOT(on_QueryBtn_clicked()));
    connect(m_pDisplayAllBtn, SIGNAL(clicked()), this, SLOT(on_DisplayAllBtn_Clicked()));
    connect(m_pSubmitBtn, SIGNAL(clicked()), this, SLOT(on_SubmitBtn_clicked()));
    connect(m_pRevertBtn, SIGNAL(clicked()), this, SLOT(on_RevertBtn_clicked()));
    connect(m_pAddBtn, SIGNAL(clicked()), this, SLOT(on_AddBtn_clicked()));
    connect(m_pDeleteRowBtn, SIGNAL(clicked()), this, SLOT(on_DeleteRowBtn_clicked()));
}

void Ex_UserCfgPage::initDBTabelModel()
{
    m_pTableModel = new QSqlTableModel(m_widget);
    m_pTableModel->setTable("User");
    m_pTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pTableModel->select();
    initTabelHeaderData();// ex
    m_pTableView->setModel(m_pTableModel);

    Ex_ComboBoxDelegate* comboBoxDeletagate = new Ex_ComboBoxDelegate(m_widget);
    m_pTableView->setItemDelegateForColumn(3, comboBoxDeletagate);

    m_pTableView->verticalHeader()->setDefaultSectionSize(50);
    m_pTableView->verticalHeader()->setMinimumSectionSize(50);
    m_pTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Ex_UserCfgPage::initTabelHeaderData()
{
    m_pTableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pTableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_pTableModel->setHeaderData(2, Qt::Horizontal, tr("Password"));
    m_pTableModel->setHeaderData(3, Qt::Horizontal, tr("Permission"));
}

void Ex_UserCfgPage::initUi()
{
    setBackColor();
    createControl();
}

void Ex_UserCfgPage::update()
{
    //
}

void Ex_UserCfgPage::on_QueryBtn_clicked()
{
    QString strName = m_pNameLineEdit->text();
    m_pTableModel->setFilter(QString("name='%1'").arg(strName));
    m_pTableModel->select();
    initTabelHeaderData();// ex

    m_wndMain->prepareKeyStroke();

}

void Ex_UserCfgPage::on_DisplayAllBtn_Clicked()
{
    m_pTableModel->setTable("User");
    m_pTableModel->select();
    initTabelHeaderData();// ex
    m_wndMain->prepareKeyStroke();
}

void Ex_UserCfgPage::on_SubmitBtn_clicked()
{
    m_pTableModel->database().transaction(); //
    if(m_pTableModel->submitAll())
    {
        m_pTableModel->database().commit();
    }
    else
    {
        m_pTableModel->database().rollback(); //
        QMessageBox::warning(m_widget, tr("tableModel"),
                             tr("Database Error:%1").arg(m_pTableModel->lastError().text()));
    }
    sync();
    m_wndMain->prepareKeyStroke();
}

void Ex_UserCfgPage::on_RevertBtn_clicked()
{
    m_pTableModel->revertAll();
    m_wndMain->prepareKeyStroke();
}

void Ex_UserCfgPage::on_AddBtn_clicked()
{
    int rowNum = m_pTableModel->rowCount();
    m_pTableModel->insertRow(rowNum);
//    m_pTableModel->submitAll();
    m_wndMain->prepareKeyStroke();
}

void Ex_UserCfgPage::on_DeleteRowBtn_clicked()
{
    int curRow = m_pTableView->currentIndex().row();
    m_pTableModel->removeRow(curRow);

    int ok = QMessageBox::warning(m_widget, tr("Delete"), tr("Are you sure delete current row?"),
                                  QMessageBox::Yes, QMessageBox::No);
    if(QMessageBox::No == ok)
    {
        m_pTableModel->revertAll();
    }
    else
    {
        m_pTableModel->submitAll();
    }
    m_wndMain->prepareKeyStroke();
}













