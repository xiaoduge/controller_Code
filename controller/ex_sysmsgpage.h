#ifndef EX_SYSMSGPAGE_H
#define EX_SYSMSGPAGE_H


#include "subpage.h"


class MainWindow;


class Ex_SysMsgPage : public CSubPage
{
    Q_OBJECT
public:
    enum SYSMS_PAGE_LB_NAME {
         SYSMS_PAGE_LB_TITLE = 0,
         SYSMS_PAGE_LB_1,
         SYSMS_PAGE_LB_2,
         SYSMS_PAGE_LB_3,
         SYSMS_PAGE_LB_4,
         SYSMS_PAGE_LB_5,
         SYSMS_PAGE_LB_NUM
    };

public:
    Ex_SysMsgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    virtual void update();

private:
    void setBackColor();
    void setTitleMsg();
    void setNameMsg(int iIdx,QString strInfo);
    void updateNameMsg();

    void buildTranslationTitleRephile();
    void buildTranslationTitleVWR();

    void buildTitles();

private:
    QWidget       *m_pWidget;
    QLabel* m_pExmsgLabel[SYSMS_PAGE_LB_NUM];
    QString m_ExMsgName[SYSMS_PAGE_LB_NUM];

public slots:
};



#endif // EX_SYSMSGPAGE_H
