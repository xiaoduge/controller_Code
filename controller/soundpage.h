#ifndef SOUNDPAGE_H
#define SOUNDPAGE_H

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
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include <QComboBox>
#include "cbitmapbutton.h"
#include <QCheckBox>

#include "Display.h"

class MainWindow;

enum SOUNDPAGEPAGE_BTN_ENUM
{
   SOUNDPAGE_BTN_SAVE = 0,
   SOUNDPAGEPAGE_BTN_NUM,
};


class SoundPage : public CSubPage
{
    Q_OBJECT

public:
    SoundPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    virtual void leaveSubPage();

private:

    void setBackColor();
    void save();
    void buildTitles();


    QLabel        *m_lblNames[DISPLAY_SOUND_NUM];

    QCheckBox     *m_chkSwitchs[DISPLAY_SOUND_NUM];

    QWidget       *m_pBackWidget[DISPLAY_SOUND_NUM];

    QString       m_strSounds[DISPLAY_SOUND_NUM];

    QString       m_strQss4Chk;
    int           m_iSoundMask;

    CBitmapButton     *m_pBtnSave;   

public slots:

    void on_checkBox_changeState(int state);

    void on_btn_clicked(int tmp);
};

#endif // SOUNDPAGE_H
