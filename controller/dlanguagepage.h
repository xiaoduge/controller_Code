#ifndef DLANGUAGEPAGE_H
#define DLANGUAGEPAGE_H

#include "subpage.h"

#define LanguageNum  (APP_LAN_NUM)

class MainWindow;
class QComboBox;

class DLanguagepage : public CSubPage
{
    Q_OBJECT

public:
    DLanguagepage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

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
