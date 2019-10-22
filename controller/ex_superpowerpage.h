#ifndef EX_SUPERPOWERPAGE_H
#define EX_SUPERPOWERPAGE_H

#include "subpage.h"

class QComboBox;
class QLabel;
class MainWindow;
class QPushButton;
class DLineEdit;
class CBitmapButton;

class Ex_SuperPowerPage : public CSubPage
{
    Q_OBJECT
public:
    enum SYSCFGPAGE_BTN_ENUM
    {
        SYSCFGPAGE_BTN_SAVE = 0,
        SYSCFGPAGE_BTN_NUM
    };

    enum SYSCFGPAGE_LABEL_ENUM
    {
        SYSCFGPAGE_LB_CATALOGNO = 0,
        SYSCFGPAGE_LB_SERIALNO,
        SYSCFGPAGE_LB_PRODATE,
        SYSCFGPAGE_LB_INSTALLDATE,
        SYSCFGPAGE_LB_SOFTVER,
        SYSCFGPAGE_LB_NUM
    };

public:
    Ex_SuperPowerPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

private:
    void buildTitles();
    void save();
    void setBackColor();
    void createControl();
    void connectData();

    void checkLoginInfo();

    //delete db table data;
    bool deleteDbAll();
    bool deleteDbWater();
    bool deleteDbAlarm();
    bool deleteDbGetWater();
    bool deleteDbPWater();
    bool deleteDbLog();
    bool deleteDbConsumables();
    bool deleteDbSubAccount();

    bool deleteInfoConfig();
    bool deleteConfig();
    bool deleteCaliParamConfig();

private:
    QLabel    *m_lbDefaultState;
    QComboBox *m_cmbDefaultState;

    QLabel    *m_lbDeviceTypeName;
    QComboBox *m_cmbDeviceFlow;
    QComboBox *m_cmbDeviceType;

    CBitmapButton* m_pBtnSave;

    QLabel* m_pCompanyLabel;
    QComboBox* m_pCompanyComboBox;
    QLabel* m_pExLabel[SYSCFGPAGE_LB_NUM];
    DLineEdit* m_ExLineEdit[SYSCFGPAGE_LB_NUM];


    QWidget* m_pDeleteWidget;
    QLabel* m_pLbDbDel;
    QComboBox* m_pCmDbDel;
    QPushButton* m_pBtnDbDel;

    QComboBox* m_pCmConfigDel;
    QPushButton* m_pBtnDelCfg;

public slots:
    void on_btn_clicked(int);
    void on_CmbIndexChange_DefaultState(int index);

    void on_CmbIndexChange_deviceType(int index);

    void on_btnDbDel_clicked();
    void on_btnDelCfg_clicked();
};


#endif // EX_SUPERPOWERPAGE_H
