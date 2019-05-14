#ifndef OPT_H
#define OPT_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>

#include "CfgDialog.h"
#include "usermanage.h"
#include "DbDialog.h"
#include "CMDialog.h"
#include "EngDialog.h"

class COptDlg : public QDialog
{
    Q_OBJECT
public:
    explicit COptDlg(QWidget *parent = 0);
    ~COptDlg();
    void EngNotify(int iType,void *pItem);    
    void  CfgNotify(int iType,void *pItem);
    void  EngRfMsg(IAP_NOTIFY_STRU *pNotify);
    
signals:

private:

    QPushButton *m_pBtnUserM;
    QPushButton *m_pBtnHInfQ;
    QPushButton *m_pBtnCFG;
    
    QPushButton *m_pBtnRO;
    QPushButton *m_pBtnRH;
    QPushButton *m_pBtnReset;
    
    QPushButton *m_pBtnRepS;
    QPushButton *m_pBtnCM;
    QPushButton *m_pBtnEngineer;
    
    QPushButton *m_pBtnExit;

    QGridLayout *m_pMainLayout;

    QHBoxLayout   *m_pTopLayout;
    QHBoxLayout   *m_pMiddleLayout;
    QHBoxLayout   *m_pBottomLayout;
    QHBoxLayout   *m_pBottom2Layout;
    CfgDlg        *m_pCfgDlg;
    UserManageDlg *m_pUserMgrDlg;
    DbDlg         *m_pDbDlg ;
    CMDlg         *m_pCMDlg ;
    EngineerDlg   *m_EngDlg;
public slots:
    void on_pushButton_UserM();
    void on_pushButton_HInfQ();
    void on_pushButton_CFG();
    void on_pushButton_RO();
    void on_pushButton_RH();
    void on_pushButton_RepS();
    void on_pushButton_Exit();
    void on_pushButton_CM();
    void on_pushButton_Reset();
    void on_pushButton_Engineer();


};

#endif // OPT_H
