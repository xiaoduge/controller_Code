#include "usermanage.h"
#include "mainwindow.h"
#include "user.h"
#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QMouseEvent>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QTextCodec>

#include <QDebug>

#include "keyboard.h"
#include "MyParams.h"
#include "user.h"

QString INSERT_sql_User = "INSERT INTO User (id, Name, PassWord , Permission) VALUES (:id, :Name , :PassWord, :Permission)";
QString Delete_sql_User = "Delete from User where Name = ?";
QString SELECT_sql_User = "SELECT id, Name, PassWord , Permission FROM User";

UserManageDlg::UserManageDlg(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(300,200); //设置窗体固定大小

    Qt::WindowFlags flags=Qt::Dialog;
    //flags |=Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    m_pBtnAddU  = new QPushButton;
    m_pBtnAddU->setText("AddU");

    m_pBtnDelU  = new QPushButton;
    m_pBtnDelU->setText("DelU");

    m_pBtnCancelU  = new QPushButton;
    m_pBtnCancelU->setText("Exit");

    m_pEditUserName = new QLineEdit;
    m_pEditPassWord = new QLineEdit;

    m_pLUserNameL = new QLabel;
    m_pLUserNameL->setText("UserName:");
    m_pLPassWordL = new QLabel;
    m_pLPassWordL->setText("PassWord:");

    m_pQlpermission = new QLabel;
    m_pQlpermission->setObjectName(QString::fromUtf8("permission"));
    m_pQlpermission->setText("permission:");

    m_pROPBOX    = new QCheckBox(this);
    m_pROPBOX->setText("RO");
    m_pPHPBOX    = new QCheckBox(this);
    m_pPHPBOX->setText("PH");
    m_pDelPBOX   = new QCheckBox(this);
    m_pDelPBOX->setText("DelDB");
    m_pAdminPBOX = new QCheckBox(this);
    m_pAdminPBOX->setText("Admin");

    m_pMainLayout   = new QGridLayout(this);
    m_pTopLayout    = new QGridLayout();
    m_pPLayout      = new QGridLayout();
    m_pBottomLayout = new QHBoxLayout();

    m_pTopLayout->addWidget(m_pLUserNameL,1,1);
    m_pTopLayout->addWidget(m_pEditUserName,1,2);
    m_pTopLayout->addWidget(m_pLPassWordL,2,1);
    m_pTopLayout->addWidget(m_pEditPassWord,2,2);

    m_pPLayout->addWidget(m_pQlpermission,0,0);
    m_pPLayout->addWidget(m_pROPBOX,0,1);
    m_pPLayout->addWidget(m_pPHPBOX,0,2);
    m_pPLayout->addWidget(m_pDelPBOX,1,1);
    m_pPLayout->addWidget(m_pAdminPBOX,1,2);

    m_pBottomLayout->addWidget(m_pBtnAddU);
    m_pBottomLayout->addWidget(m_pBtnDelU);
    m_pBottomLayout->addWidget(m_pBtnCancelU);

    m_pMainLayout->addLayout(m_pTopLayout,0,0);
    m_pMainLayout->addLayout(m_pPLayout,1,0);
    m_pMainLayout->addLayout(m_pBottomLayout,2,0,1,2);

    this->Permission = 0;

    m_pEditUserName->installEventFilter(this);
    m_pEditPassWord->installEventFilter(this);

    setLayout(m_pMainLayout);

    connect(m_pROPBOX, SIGNAL(stateChanged(int)), this, SLOT(ROPBOX_Checked(int)));
    connect(m_pPHPBOX, SIGNAL(stateChanged(int)), this, SLOT(PHPBOX_Checked(int)));
    connect(m_pDelPBOX, SIGNAL(stateChanged(int)), this, SLOT(DelPBOX_Checked(int)));
    connect(m_pAdminPBOX, SIGNAL(stateChanged(int)), this, SLOT(AdminPBOX_Checked(int)));

    connect(m_pBtnAddU, SIGNAL(clicked()), this, SLOT(on_pushButton_AddU()));
    connect(m_pBtnDelU, SIGNAL(clicked()), this, SLOT(on_pushButton_DelU()));
    connect(m_pBtnCancelU, SIGNAL(clicked()), this, SLOT(on_pushButton_CancelU()));

}

UserManageDlg::~UserManageDlg()
{
    delete m_pBtnAddU;
    delete m_pBtnDelU;
    delete m_pBtnCancelU;

    delete m_pEditUserName;
    delete m_pEditPassWord;

    delete m_pLUserNameL;
    delete m_pLPassWordL;

    delete m_pButtonGroup;
    delete m_pCheckBox;

    delete m_pQlpermission;

    delete m_pROPBOX;
    delete m_pPHPBOX;
    delete m_pDelPBOX;
    delete m_pAdminPBOX;


    delete m_pTopLayout;
    delete m_pPLayout;
    delete m_pBottomLayout;
    delete m_pMainLayout;
    
}

void UserManageDlg::write_back_str(QString str)
{
    QWidget *fWidget = this->focusWidget();
    QLineEdit *sel_line_edit= qobject_cast<QLineEdit*>(fWidget);
    if (!sel_line_edit)
    {
        return ;
    }
    
    if(str=="Backspace")
         sel_line_edit->backspace();
    else sel_line_edit->insert(str);
}


void UserManageDlg::on_pushButton_AddU()
{
    QSqlQuery query;

    //UserName->setText("123");
    //PassWord->setText("123");

    if(m_pEditUserName->text().isEmpty()|| m_pEditPassWord->text().isEmpty())
    {
        QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );
        QMessageBox::warning(this,tr("warning"),tr("User name or password NULL!"),QMessageBox::Yes);
        m_pEditUserName->clear();
        m_pEditPassWord->clear();
        m_pEditUserName->setFocus();
    }
    else
    {
        query.prepare(INSERT_sql_User);
        query.bindValue(":Name",m_pEditUserName->text());
        query.bindValue(":PassWord",m_pEditPassWord->text());
        query.bindValue(":Permission",this->Permission);

        qDebug()<<"INSERT_sql_User "<<query.exec();

        if(!query.exec())
        {
            QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );
            QMessageBox::warning(this,tr("warning"),tr("Add User false!"),QMessageBox::Yes);
        }

        this->Permission = 0;
    }
}

void UserManageDlg::on_pushButton_DelU()
{
    QSqlQuery query;
    query.prepare(Delete_sql_User);
    query.addBindValue(m_pEditUserName->text());
    qDebug()<<"Delete_sql_User "<<query.exec();
}

void UserManageDlg::on_pushButton_CancelU()
{
    close();
}

void UserManageDlg::ROPBOX_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        this->Permission |= (1 << ROP);
    }
    else
    {
        this->Permission &= ~(1 << ROP);
    }
}

void UserManageDlg::PHPBOX_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        this->Permission |= (1 << PHP);
    }
    else
    {
        this->Permission &= ~(1 << PHP);
    }
}

void UserManageDlg::DelPBOX_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        this->Permission |= (1 << DelP);
    }
    else
    {
        this->Permission &= ~(1 << DelP);
    }
}

void UserManageDlg::AdminPBOX_Checked(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        this->Permission |= AdminP;
    }
    else
    {
        this->Permission &= ~AdminP;
    }
}

bool UserManageDlg::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==m_pEditPassWord)
    {
        if (event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
            }
        }
        /*else if(event->type()==QEvent::FocusOut)
        {
         }*/
        /*if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
            }
        }*/
    }

    else if(watched==m_pEditUserName)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
            }
        }
    }
    return QDialog::eventFilter(watched,event);
}

void UserManageDlg::closeEvent(QCloseEvent *event)
{
    (void)event;
}
