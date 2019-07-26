#ifndef SYSTESTPAGE_H
#define SYSTESTPAGE_H


#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "systestlistwidgtitem.h"
#include "notify.h"
#include "Display.h"


#define SYSTESTNUM 21

class MainWindow;

class SysTestPage : public CSubPage
{
    Q_OBJECT

public:
    SysTestPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

            void  updateInfo(int major,int minor,float fValue);  

    virtual void update();

    virtual void fade();
    
private:

    void buildTitles();

    void createList();

    void setBackColor();


    QListWidget *listWidget;

    QListWidgetItem *listWidgetIem[SYSTESTNUM];

    Systestlistwidgtitem *m_pSysTestlistItem[SYSTESTNUM];

    int              m_iRealItems;
    int              m_aiIndex[DISP_NOT_NUM];

    struct EXE_VALVE_NAME
    {
        int      type;
        int      id;
    }aSwitchs[SYSTESTNUM];

public slots:

    void ItemClicked(QListWidgetItem * item);
    void on_btn_clicked(int);
    void on_item_cmb_changed(int);
};

#endif // SYSTESTPAGE_H
