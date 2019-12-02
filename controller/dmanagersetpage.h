#ifndef DMANAGERSETPAGE_H
#define DMANAGERSETPAGE_H

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
class DLineEdit;
class QSlider;
class QButtonGroup;

class DManagerSetPage : public CSubPage
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
        MANAGER_PAGE_TIME = 0,
        MANAGER_PAGE_CALIBRATION,
        MANAGER_PAGE_AUDIO,
        MANAGER_PAGE_LCD,
        MANAGER_PAGE_ADDSETTINGS, //Additional settings
        MANAGER_PAGE_NUM
    };

    enum ADDITIONAL_SETTINGS_NUM
    {
        HPCIR_SETTING,
        ADDITIONAL_NUM
    };

public:
    DManagerSetPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    ~DManagerSetPage();
	virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void update();
    void show(bool bShow);

protected:
    void timerEvent(QTimerEvent *event);

protected slots:
    //time page
    void on_timeDateSet_clicked();
    void on_timeTimeSet_clicked();
    void on_timeOkBtn_clicked();
    void on_timeCancelBtn_clicked();

    //Dispense Rate
    void on_caliSaveBtn_clicked();

    //Audio
    void on_checkBox_changeState(int state);
    void on_audioBtnSavebtn_clicked();

    //LCD
    void on_LcdSaveBtn_clicked();
    void on_CheckEnergySave_stateChanged(int state);
    void on_comboBox_currentIndexChanged(int index);
    void setValue(int);

    //Additional Settings
    void on_AdditionalBtnSave_clicked();
    void on_HPCircheckBox_changeState(int state);

private:
    void initTimePage();
    void initCalibrationPage();
    void initAudioPage();

    void initLcdPage();
    void initAdditionalSettingsPage();

    void changeTime();

    void buildTitles();
    void setBackColor();
    void save();

private:
    QWidget *m_mainWidget;
    QWidget *m_pageWidget[MANAGER_PAGE_NUM];
    QTabWidget *m_tabWidget;

    //Time
    int m_secondTimer;
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

    //calibration
    QWidget* m_pCaliS1Widget;
    QLabel* m_pCaliS1Label;
    DLineEdit* m_pCaliS1LineEdit;
    QPushButton* m_pCaliBtn;
	int m_caliId;

    //Audio
    QLabel        *m_lblNames[DISPLAY_SOUND_NUM];
    QCheckBox     *m_chkSwitchs[DISPLAY_SOUND_NUM];
    QWidget       *m_pAudioBackWidget[DISPLAY_SOUND_NUM];
    QString       m_strSounds[DISPLAY_SOUND_NUM];
    QString       m_strQss4Chk;
    int           m_iSoundMask;
    QPushButton   *m_pAudioBtnSave;

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

    //Additional Settings
    QPushButton   *m_pAddBtnSave;
    QLabel        *m_pAdditionalLb[ADDITIONAL_NUM];
    QCheckBox     *m_pAdditionalCheck[ADDITIONAL_NUM];
    QWidget       *m_pAdditionalWidget[ADDITIONAL_NUM];
};

#endif // EX_MANAGERSETPAGE_H
