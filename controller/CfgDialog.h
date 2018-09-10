#ifndef CFGDIALOG_H
#define CFGDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialog>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include "keyboard.h"
#include "MyParams.h"

namespace Ui {
class mainLayoutWindow;
}

class CfgDlg : public QDialog
{
    Q_OBJECT

public:
    CfgDlg(QWidget* parent=0);
    ~CfgDlg();
    void InfoUpdate(int iType,void *pData);

private slots:
    void CFGCom();
    void SaveCfg();
    void Exit();

public slots:
    bool eventFilter(QObject *, QEvent *);

    void Have_Checked(int state);
    void Alarm_Checked(int state);
    
    void PM_PM1_CmbIndexChange(int index);
    void PM_PM2_CmbIndexChange(int index);
    void PM_PM3_CmbIndexChange(int index);
    void SM_btnGroupClicked(int gemfield);

private:
    QLabel *m_pLabelCFGTYPE;
    QComboBox *m_pComboBoxCFG;

    QLabel *m_pLabelsp[MACHINE_PARAM_SP_NUM];
    QLineEdit *m_pEditSP[MACHINE_PARAM_SP_NUM];

    QLabel *m_pLabelTm[MACHINE_PARAM_TM_NUM];
    QLineEdit *m_pEditTm[MACHINE_PARAM_TM_NUM];

    typedef struct
    {
        QLineEdit *m_pEditName;
    }LineEdit_TYPE_STRU;


    QCheckBox    *m_pQCheckBoxSm[MACHINE_PARAM_SUBMODULE_NUM];

	/* for alarm setting */
    QCheckBox   *m_pQCheckBoxAlarm[DISP_ALARM_NUM];
    QLineEdit   *m_pEditAlarm[DISP_ALARM_PARAMS];

    /* for flow meter setting */
    QLabel      *m_pLabelFm[APP_FM_FLOW_METER_NUM];
    QLineEdit   *m_pEditFm[APP_FM_FLOW_METER_NUM];

    /* for Pressure Meter setting */
    QLabel      *m_pLabelPm[APP_EXE_PRESSURE_METER];
    QLineEdit   *m_pEditPm[APP_EXE_PRESSURE_METER];
    QComboBox   *m_pCombBox[APP_EXE_PRESSURE_METER];
    float        m_fPmCap[APP_EXE_PRESSURE_METER];
    
    /* for Misc setting */

	/* Misc.1 tube cir setting */
    QCheckBox        *m_pQCheckBoxWeekday[7]; /* for every day */
    QDateTimeEdit    *m_pStartTime[1];   
    QDateTimeEdit    *m_pEndTime[1]; 
    QLineEdit        *m_pEditTC[1];
	/* Misc.2 Regulating pump setting */
    QSlider           *m_pRPumpSlider;
	

    /* for calibrating */
    QLabel      *m_pLabelCal4Eco[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + DISP_CAL_P_NUM];
    QLineEdit   *m_pEditCalState[DISP_CAL_I_NUM*2 + DISP_CAL_F_NUM + DISP_CAL_P_NUM];
    QLineEdit   *m_pEditCal[DISP_CAL_NUM];

    QPushButton *m_pBtnSave;
    QPushButton *m_pBtnExit;

    QGridLayout *m_pMainLayout;

    QTabWidget  *m_pTblWidget;

    QWidget     *m_pWidgets[8];	

	int          m_nWidgets;

    LineEdit_TYPE_STRU LineEdit_TYPE[LESUM] ;
    
    DISP_ALARM_SETTING_STRU      AlarmSettting;
    DISP_SUB_MODULE_SETTING_STRU SubModSetting;
    DISP_TIME_PARAM_STRU         TMParam;
    DISP_MACHINE_PARAM_STRU      MMParam; 
    DISP_CONSUME_MATERIAL_STRU   CMParam; 
    DISP_FM_SETTING_STRU         FmParam; 
    DISP_PM_SETTING_STRU         PmParam; 
    DISP_CAL_SETTING_STRU        CalParam; 
    DISP_MISC_SETTING_STRU       MiscParam; 
    DISP_CLEAN_SETTING_STRU      CleanParam; 

    QFont        mFontTime ;

    void closeEvent(QCloseEvent *event);

};


#endif // CFGDIALOG_H
