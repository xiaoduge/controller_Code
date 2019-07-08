#ifndef SYSTEMCFGPAGE_H
#define SYSTEMCFGPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>

#include "Display.h"
#include "ctrlapplication.h"
#include "dlineedit.h"


class MainWindow;


enum SYSCFGPAGE_BTN_ENUM
{
   SYSCFGPAGE_BTN_SAVE = 0,
   SYSCFGPAGE_BTN_NUM,
};

class SystemCfgPage : public CSubPage
{
    Q_OBJECT

public:
    SystemCfgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

protected slots:
    void on_CmbIndexChange_DefaultState(int index);

private:

    void buildTitles();
    void save();
    void setBackColor();
    void createControl();
    void connectData();

private:
    QLabel    *m_lbPWTankName;
    QComboBox *m_cmbPWTankVolume;
    DLineEdit *m_lePWTankHeight;
    QLabel    *m_lbPWHUnit;
    DLineEdit *m_lePWTankCap;
    QLabel    *m_lbPWCUnit;

    QLabel    *m_lbSWTankName;
    QComboBox *m_cmbSWTankVolume;
    DLineEdit *m_leSWTankHeight;
    QLabel    *m_lbSWHUnit;
    DLineEdit *m_leSWTankCap;
    QLabel    *m_lbSWCUnit;

    QCheckBox *m_chkPWTankUV;
    QLabel    *m_lbPWTankUVName;
    DLineEdit *m_lePWTankUVValue;
    QLabel    *m_lbPWTankUVUnit;
    
    QLabel    *m_lbPOweronFlushName;
    DLineEdit *m_lePOweronFlushValue;
    QLabel    *m_lbPOweronFlushUnit;

    QLabel    *m_lbLoginLingerName;
    DLineEdit *m_leLoginLingerValue;
    QLabel    *m_lbLoginLingerUnit;

//    QLabel    *m_lbDeviceTypeName;
//    QLabel *m_lbDeviceType;

    //2019.3.14
    QLabel    *m_lbDefaultState;
    QComboBox *m_cmbDefaultState;

    QCheckBox *m_chkDeviceTypeTOC;

    CBitmapButton     *m_pBtnSave;   

    bool               m_bHaveToc;

    QCheckBox *m_aChks[32];

    struct CHK_ITEM_TYPE {
        int iId;
    }aCHKsIds[32];

    int m_iRealChkNum;
    
public slots:
    void on_btn_clicked(int);
    void on_CmbIndexChange_pw(int index);
    void on_CmbIndexChange_sw(int index);
};

extern CtrlApplication *gApp;

#endif // STERILIZEPAGE_H
