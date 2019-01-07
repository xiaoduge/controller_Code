#ifndef EX_MANAGERSETPAGE_H
#define EX_MANAGERSETPAGE_H

#include "subpage.h"
#include "Display.h"

#define UnitNum DISPLAY_UNIT_NUM
#define UnitChoice_NUM 3

class MainWindow;
class QTabWidget;
class QCalendarWidget;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;

class Ex_ManagerSetPage : public CSubPage
{
    Q_OBJECT

public:
    enum TIME_SET_ENUM
    {
        DATE_SET = 0,
        TIME_SET,
        DATE_NUM
    };

    enum TIMEPAGE_BTN_ENUM
    {
        TIMEPAGE_BTN_DATE_SET = 0,
        TIMEPAGE_BTN_TIME_SET,
        TIMEPAGE_BTN_CANCEL,
        TIMEPAGE_BTN_OK,
        TIMEPAGE_BTN_NUM
    };

    enum TABPAGE
    {
        MANGER_PAGE_FLOW = 0,
        MANGER_PAGE_TIME,
        MANGER_PAGE_LAN,
        MANGER_PAGE_AUDIO,
        MANGER_PAGE_UNITS,
        MANGER_PAGE_LCD,
        MANGER_PAGE_NUM
    };

public:
    Ex_ManagerSetPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void update();

protected slots:
    //Flow Rate
    void on_flowSaveBtn_clicked();

    //time page
    void on_timeDateSet_clicked();
    void on_timeTimeSet_clicked();
    void on_timeOkBtn_clicked();
    void on_timeCancelBtn_clicked();

    //language
    void on_lanSaveBtn_clicked();
    void on_cbLan_currentIndexChanged(int index);

    //Audio
    void on_checkBox_changeState(int state);
    void on_audioBtnSavebtn_clicked();

    //Units
    void on_unitsSaveBtn_clicked();
    void on_btn_clicked(QAbstractButton *button);

    //LCD
    void on_LcdSaveBtn_clicked();
    void on_CheckEnergySave_stateChanged(int state);
    void on_comboBox_currentIndexChanged(int index);
    void setValue(int);

private:
    void initFlowPage();
    void initTimePage();
    void initLanguagePage();
    void initAudioPage();
    void initUnitsPage();
    void initLcdPage();

    void changeTime();

    void buildTitles();
    void setBackColor();
    void save();

private:
    QWidget *m_mainWidget;
    QWidget *m_pageWidget[MANGER_PAGE_NUM];
    QTabWidget *m_tabWidget;

    //Flow Rate
    QWidget *m_pFlowBackWidget;
    QLabel *m_flowLabel;
    QLineEdit *m_flowLineEdit;
    QLabel *m_flowUnit;
    QPushButton *m_flowSaveBtn;

    //Time
    QLabel *lbTitPic;
    QLabel *lbTitName;
    QCalendarWidget *CalS;
    QLabel *lbName[DATE_NUM];
    QWidget   *ShowWidget[DATE_NUM];
    QComboBox *TimeHCbox;
    QComboBox *TimeMCbox;
    QComboBox *TimeSCbox;
    QPushButton *m_pBtns[TIMEPAGE_BTN_NUM];
    QString m_astrDateName[DATE_NUM];

    //Language
    QComboBox* m_pCbLan;
    QPushButton* m_pLanBtnSave;
    QLabel* m_pLbWorld;
    int    m_iLanguage;

    //Audio
    QLabel        *m_lblNames[DISPLAY_SOUND_NUM];
    QCheckBox     *m_chkSwitchs[DISPLAY_SOUND_NUM];
    QWidget       *m_pAudioBackWidget[DISPLAY_SOUND_NUM];
    QString       m_strSounds[DISPLAY_SOUND_NUM];
    QString       m_strQss4Chk;
    int           m_iSoundMask;
    QPushButton   *m_pAudioBtnSave;

    //units
    QLabel        *m_laName[UnitNum];
    QRadioButton  *m_btnradios[UnitNum][UnitChoice_NUM];
    QButtonGroup  *m_btngrp[UnitNum];
    QHBoxLayout   *m_pLayout[UnitNum];
    QWidget       *m_pUnitsBackWidget[UnitNum];
    int           m_aiUnit[UnitNum];
    QPushButton   *m_pUnitsBtnSave;
    QString strItemName[UnitNum];
    QString strUnitChoice[UnitNum][UnitChoice_NUM];

    //LCD
    QLabel        *laName[2]; //2
    QComboBox     *m_comboBox;
    QLabel        *m_sleepLabel;
    QCheckBox     *m_pCheckEnergySave; //debut
    QWidget       *m_pLcdBackWidget[3];
    QSlider       *pSlider[1];
    QString       m_DispNames[2];
    QPushButton   *m_pLcdBtnSave;
    int            m_iEnergySave;
    int            m_iBrightness;
    int            m_iSleepTime;


};

#endif // EX_MANAGERSETPAGE_H
