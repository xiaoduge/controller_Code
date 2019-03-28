#ifndef EX_PERMISSIONSETPAGE_H
#define EX_PERMISSIONSETPAGE_H

#include "subpage.h"
#include "Display.h"

class MainWindow;
class QTabWidget;
class QCalendarWidget;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;

class Ex_PermissionSetPage:  public CSubPage
{
    Q_OBJECT
public:
    enum TABPAGE
    {
        PAGE_RFID = 0,
        PAGE_PERMISSION,
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

private:
    void initRFIDConfigPage();
    void initInstallPermissionPage();


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
};

#endif // EX_PERMISSIONSETPAGE_H
