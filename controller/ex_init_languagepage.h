#ifndef EX_INIT_LANGUAGEPAGE_H
#define EX_INIT_LANGUAGEPAGE_H


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

#include "Display.h"

#define Ex_LanguageNum  (APP_LAN_NUM)

class MainWindow;
class QPushButton;
class QComboBox;

class Ex_Init_Languagepage : public CSubPage
{
    Q_OBJECT
public:
    Ex_Init_Languagepage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void leaveSubPage();

private:

    void buildTitles();
    void setBackColor();

    QComboBox* m_pCbLan;
//    QPushButton* m_pBtnSave;
    QLabel* m_pLbWorld;
    int    m_iLanguage;

    QPushButton* m_nextBtn;

signals:
    void languageSwitchBtnClicked(int);

public slots:
    void on_saveBtn_clicked();
    void on_cbLan_currentIndexChanged(int index);

    void on_nextBtn_clicked();
};


#endif // EX_INIT_LANGUAGEPAGE_H
