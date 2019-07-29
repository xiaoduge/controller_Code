#ifndef SYSMSGPAGE_H
#define SYSMSGPAGE_H

#include "subpage.h"
#include <QTextCharFormat>

class MainWindow;
class QTextEdit;


enum SYSMS_PAGE_TE_NAME {
     SYSMS_PAGE_TE_TITLE = 0,
     SYSMS_PAGE_TE_1,
     SYSMS_PAGE_TE_2,
     SYSMS_PAGE_TE_3,
     SYSMS_PAGE_TE_4,
     SYSMS_PAGE_TE_5,
     SYSMS_PAGE_TE_NUM
};

#define SYSMS_PAGE_INFO_NUM (SYSMS_PAGE_TE_NUM - 1)

class SysMsgPage : public CSubPage
{
    Q_OBJECT

public:
    SysMsgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

private:

    void setBackColor();

    void setTitleMsg();

    void setNameMsg(int iIdx,QString strInfo);

    void buildTitles();

private:
    QWidget       *m_pWidget;

    QTextEdit     *m_pMsgTextEdit[SYSMS_PAGE_TE_NUM]; 
    
    QString       m_TitleMsg;
    QString       m_astrMsgName[SYSMS_PAGE_INFO_NUM];

    QTextCharFormat charFormatName;
    QTextCharFormat charFormatValue;

public slots:
};

#endif // SYSMSGPAGE_H
