#ifndef ENGDIALOG_H
#define ENGDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>
#include "keyboard.h"
#include "MyParams.h"
#include "qsliderex.h"
#include "Display.h"

namespace Ui {
class mainLayoutWindow;
}

class SwitchButton;
class CSliderEx;

class EngineerDlg : public QDialog
{
    Q_OBJECT

public:
    EngineerDlg(QWidget* parent=0);
    ~EngineerDlg();
    void InfoUpdate(int iType,void *pItem);
    void RfNotify(IAP_NOTIFY_STRU *pNotify);
    
private slots:
    void Exit();
    void onSwitchBtnChanged(bool bState,int iCtrlId);
    void onSliderChanged(int id, int value);
    void rfBtnsClicked(int id);
    void RfId_CmbIndexChange(int index);

public slots:
    bool eventFilter(QObject *, QEvent *);

private:

    QWidget     *m_pWidgets[8];	

	int          m_nWidgets;

    QPushButton *m_pBtnExit;
    QLabel      *m_pLabelStatus;

    QGridLayout *m_pMainLayout;

    QTabWidget  *m_pTblWidget;

    SwitchButton **m_ppSwitchBtns;
    QLabel       **m_ppSwitchLabel;

    SwitchButton **m_ppIBBtns;
    QLabel       **m_ppIBLabels;
    QLineEdit    **m_ppEditIBs;
    QLineEdit    **m_ppEditTemps;

    SwitchButton **m_ppFMBtns;
    QLabel       **m_ppFMLabels;
    QLineEdit    **m_ppEditFMs;

    CSliderEx    **m_ppRPSliders;
    QLabel       **m_ppRPLabels;

    
    QLabel       **m_ppRfLabels;
    QLineEdit    **m_ppEditRfs;
    QComboBox    **m_ppCombBoxRfs;
    QPushButton  **m_ppBtnRfs;
    QButtonGroup *m_btnGroupRf;

    RF_DATA_LAYOUT_ITEMS m_RfDataItems;
    RF_DATA_LAYOUT_STRU  m_rfLayout;

	unsigned int	m_ulHeaderSize;
	QString	        m_strEditLotNumber;
	unsigned int	m_ulPropertySize;
	unsigned int	m_ulType;
	QString	        m_strUnknow;
	QString	        m_strTradeMark;
	QString	        m_strTradeName;
	QString	        m_strCatalogueNumber;
	unsigned int	m_ulDevice;
    QString         m_strInstallDate;

    unsigned int    m_ulUnknowData;

    unsigned int    m_ulRfIdIdx;

    void closeEvent(QCloseEvent *event);
    void ParseData(RF_DATA_LAYOUT_STRU &rfDataLayout) ;
    void MakeData(RF_DATA_LAYOUT_STRU &rfDataLayout); 

};


#endif // CFGDIALOG_H
