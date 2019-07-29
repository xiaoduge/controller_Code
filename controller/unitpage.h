#ifndef UNITPAGE_H
#define UNITPAGE_H


#include "subpage.h"
#include "Display.h"

#define UnitNum DISPLAY_UNIT_NUM
#define UnitChoice_NUM 3

class MainWindow;
class QRadioButton;
class QButtonGroup;
class QHBoxLayout;
class CBitmapButton;

enum UNITPAGE_BTN_ENUM
{
   UNITPAGE_BTN_SAVE = 0,
   UNITPAGE_BTN_NUM
};

class UnitPage : public CSubPage
{
    Q_OBJECT
public:
    UnitPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    
    virtual void update();
    
    virtual void leaveSubPage();

private:
    void buildTitles();
    void save();
    void setBackColor();

private:
    QLabel        *m_laName[UnitNum];

    QRadioButton *m_btnradios[UnitNum][UnitChoice_NUM];

    QButtonGroup *m_btngrp[UnitNum];

    QHBoxLayout  *m_pLayout[UnitNum];
    
    QWidget       *m_pBackWidget[UnitNum];

    int           m_aiUnit[UnitNum];

    CBitmapButton     *m_pBtnSave;   


public slots:
    void on_btn_clicked(int);
    void on_btn_clicked(QAbstractButton *button);
};

#endif // UNITPAGE_H
