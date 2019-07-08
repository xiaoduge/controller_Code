#ifndef EX_INIT_HANDLECFGPAGE_H
#define EX_INIT_HANDLECFGPAGE_H

#include "setdevicepage.h"
#include "ctrlapplication.h"

class MainWindow;
class QPushButton;

class Ex_Init_HandleCfgpage : public CSubPage
{
    Q_OBJECT

public:
    Ex_Init_HandleCfgpage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    void addHandler(int iType,const QString& text0,const QString& text1);
    void cfgHandlerRsp();

signals:
    void exInitFinished();
    void handlercfgSwitchBtnClicked(int);
private:
    void buildTitles();
    void setBackColor();
    void finishSave();

private:

    QListWidgetItem * searchHandler(const QString& info,int iIndex);
    QString getHandler(int iDevIdx,int iInfoIdx);
    QListWidgetItem * searchHandlerBySN(const QString& info);
    QListWidgetItem * searchHandlerByAddress(const QString& info);

    /* for handler */
    QPushButton *m_pBtnQueryHandler;
    QPushButton *m_pBtnCfgHandler;
    QPushButton *m_pBtnResetHandler;
//    QPushButton *m_pBtnRmvHandler;
    QPushButton *m_pBtnSaveHandler;
    QListWidget *m_pListWgtHandler;
    QComboBox   *m_pcombTrxType;
    QLabel      *m_plbHandlerSN;
    QLabel      *m_plbHandlerAdr;
    QLabel      *m_plbHandlerDef;
    QLabel      *m_plbHandlerType;
    QLabel      *m_plbHandlerOper;

    QPushButton* m_pExFinishBtn;
    QPushButton* m_pbackBtn;


    /* common */
    QTabWidget	*m_pTblWidget;
    QWidget     *m_widgetLayout;
    QString     m_strQss4Chk;
public slots:
    void on_btn_clicked(int);

    void on_pushButton_QueryHandler();
    void on_pushButton_CfgHandler();
    void on_pushButton_ResetHandler();
    void on_pushButton_RmvHandler();
    void on_pushButton_SaveHandler();

    void on_checkBox_changeState(int state);
    void on_pushButton_DeleteHandler();
    void on_CmbIndexChange_trx_type(int index);

    void on_pushButton_FinishBtn();
    void on_pushButton_BackBtn();
};

extern CtrlApplication *gApp;

#endif // EX_INIT_HANDLECFGPAGE_H
