#ifndef EX_PERMISSIONSETPAGE_H
#define EX_PERMISSIONSETPAGE_H

#include "subpage.h"

class MainWindow;
class QTabWidget;
class QCalendarWidget;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class DLineEdit;
class QSqlTableModel;
class QTableView;

class Ex_PermissionSetPage:  public CSubPage
{
    Q_OBJECT
public:
    enum TABPAGE
    {
        PAGE_RFID = 0,
        PAGE_PERMISSION,
        PAGE_SUBACCOUNT,
        PERMISSION_PAGE_NUM
    };
public:
    Ex_PermissionSetPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void update();

protected slots:
    void on_RfidsaveBtn_clicked();
    void on_PermissionsaveBtn_clicked();
    void on_chSubAccount_stateChanged (int state);
    void on_QueryBtn_clicked();
    void on_DisplayAllBtn_clicked();
    void on_DeleteOneBtn_clicked();
    void on_DeleteAllBtn_clicked();

private:
    void initRFIDConfigPage();
    void initInstallPermissionPage();
    void initSubAccountPage();
    void initDBTabelModel();
    void initTabelHeaderData();

    void buildTitles();
    void setBackColor();

private:
    QWidget *m_mainWidget;
    QWidget *m_pageWidget[PERMISSION_PAGE_NUM];
    QTabWidget *m_tabWidget;

    //RFID
    QLabel* m_pRfidTitle;
    QCheckBox *m_chRfid;
    QPushButton* m_saveRfidBtn;

    //Consumable Install Permission
    QLabel* m_pPermissionTitle;
    QCheckBox *m_chPermission;
    QPushButton* m_savePermissionBtn;

    //2019.10.15 add, for Sub-account
    QCheckBox *m_chSubAccount;

    QPushButton *m_pQueryBtn;
    QPushButton *m_pDisplayAllBtn;
    QPushButton *m_pDeleteOneBtn;
    QPushButton *m_pDeleteAllBtn;

    QLabel* m_pNameLB;
    DLineEdit* m_pNameLineEdit;
    QSqlTableModel *m_pTableModel;
    QTableView* m_pTableView;
};

#endif // EX_PERMISSIONSETPAGE_H
