#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QCheckBox>
#include "Display.h"


class MainWindow;

enum NETWORKPAGE_BTN_ENUM
{
   NETWORKPAGE_BTN_SAVE = 0,
   NETWORKPAGE_BTN_NUM,
};

class NetworkPage : public CSubPage
{
    Q_OBJECT
public:
    NetworkPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    void leaveSubPage();

private:

    void buildTitles();
    
    void save();

    void setBackColor();

    QLabel        *m_laName[DISPLAY_NETWORK_NUM];
    QCheckBox     *m_chkSwitchs[DISPLAY_NETWORK_NUM];
    QWidget       *m_pBackWidget[DISPLAY_NETWORK_NUM];
    QString        m_astrNetName[DISPLAY_NETWORK_NUM];
    QString       m_strQss4Chk;
    int           m_iNetworkMask;
    
    CBitmapButton     *m_pBtnSave;   

public slots:

    void on_btn_clicked(int);
    void on_checkBox_changeState(int state);


};

#endif // NETWORKPAGE_H
