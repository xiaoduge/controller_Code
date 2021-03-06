#ifndef DINITLANGUAGEPAGE_H
#define DINITLANGUAGEPAGE_H


#include "subpage.h"
#include "Display.h"

#define Ex_LanguageNum  (APP_LAN_NUM)

class MainWindow;
class QPushButton;
class QComboBox;

class DInitLanguagepage : public CSubPage
{
    Q_OBJECT
public:
    DInitLanguagepage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void leaveSubPage();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:

    void buildTitles();
    void setBackColor();

    QComboBox* m_pCbLan;
//    QPushButton* m_pBtnSave;
    QLabel* m_pLbWorld;
    int    m_iLanguage;

    QPushButton* m_nextBtn;

public slots:
    void on_saveBtn_clicked();
    void on_cbLan_currentIndexChanged(int index);

    void on_nextBtn_clicked();
};


#endif // EX_INIT_LANGUAGEPAGE_H
