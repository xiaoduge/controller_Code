#ifndef SYSTESTPAGE_H
#define SYSTESTPAGE_H

#include "subpage.h"
#include "Display.h"


class QListWidget;
class QListWidgetItem;
class Systestlistwidgtitem;

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
