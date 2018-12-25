#ifndef EX_LANGUAGEPAGE_H
#define EX_LANGUAGEPAGE_H

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

#define LanguageNum  (APP_LAN_NUM)

class MainWindow;
class QComboBox;

class Ex_Languagepage : public CSubPage
{
    Q_OBJECT

public:
    Ex_Languagepage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void leaveSubPage();

private:
    void buildTitles();
    void setBackColor();

private:
    QComboBox* m_pCbLan;
    QPushButton* m_pBtnSave;
    QLabel* m_pLbWorld;
    int    m_iLanguage;

public slots:
    void on_saveBtn_clicked();
    void on_cbLan_currentIndexChanged(int index);
};

#endif // EX_LANGUAGEPAGE_H