#ifndef DIAPLAYPAGE_H
#define DIAPLAYPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"

#include "cbitmapbutton.h"

class MainWindow;
class QComboBox;

enum DISPLAY_BTN_NAME
{
    DISPLAY_BTN_ENERGY_SAVE = 0, 
    DISPLAY_BTN_SAVE,
    DISPLAY_BTN_NUMBER,
};


class DisplayPage : public CSubPage
{
    Q_OBJECT
public:
    DisplayPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    virtual void update();

private:

    void Set_Back_Color();

    void buildTitles();
    
    void save();

    QLabel        *laName[2]; //2
    QComboBox     *m_comboBox;
    QLabel        *m_sleepLabel;

    CBitmapButton *m_pBtnEnergySave;

    QWidget       *m_pBackWidget[3];

    QSlider       *pSlider[1];

    QString       m_DispNames[2];
    
    CBitmapButton *m_pBtnSave;

    int            m_iEnergySave;
    int            m_iBrightness;
    int            m_iSleepTime;

public slots:

    void on_btn_clicked(int tmp);
    void on_comboBox_currentIndexChanged(int index);
    void setValue(int);
};


#endif // DIAPLAYPAGE_H
