#ifndef SETPOINT_H
#define SETPOINT_H

#include "subpage.h"


#define SETPNUM  22

class MainWindow;
class QListWidget;
class QListWidgetItem;
class SetPlistwidgtitem;
class CBitmapButton;

class SetPoint : public CSubPage
{
    Q_OBJECT

public:
    SetPoint(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

private:

    void buildTitles();

    void setBackColor();

    void createList();
    
    void save();

private:
    QListWidget *listWidget;

    QListWidgetItem *listWidgetIem[SETPNUM];

    SetPlistwidgtitem *pSetPlistItem[SETPNUM];
    CBitmapButton     *m_pBtnSave;

    int              m_iRealNum;
    
    struct PARAM_ITEM_TYPE {
        int iDspType;
        int iParamId[2];
    }aIds[SETPNUM];


public slots:
    void on_btn_clicked(int tmp);
    void ItemClicked(QListWidgetItem * pItem);
};

#endif // SETPOINT_H
