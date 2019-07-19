#ifndef CMDIALOG_H
#define CMDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

#include <QGridLayout>
#include <QLineEdit>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include "MyParams.h"

namespace Ui {
class mainLayoutWindow;
}

class CMDlg : public QDialog
{
    Q_OBJECT

public:
    CMDlg(QWidget* parent=0);
    ~CMDlg();

private slots:
    void CFGCom();
    void SaveCfg();
    void Exit();

public slots:
    bool eventFilter(QObject *, QEvent *);
    void resetClick(int iId);
    void resetItem(int iId,QString &strName);

private:

    QLabel *m_pLabelCM[DISP_CM_NUM];
    QLineEdit *m_pEditCMCfg[DISP_CM_NUM];
    QLineEdit *m_pEditCMCur[DISP_CM_NUM];
    QPushButton *m_pBtnCM[DISP_CM_NUM];
    
    typedef struct
    {
        QLineEdit *m_pEditName;
    }LineEdit_TYPE_STRU;


    QPushButton *m_pBtnSave;
    QPushButton *m_pBtnExit;

    QGridLayout *m_pMainLayout;

    QButtonGroup *m_pBtnGroup;

    LineEdit_TYPE_STRU LineEdit_TYPE[DISP_CM_NUM*2] ;
    
    void closeEvent(QCloseEvent *event);
    void showCmInfo();

};


#endif // CMDIALOG_H
