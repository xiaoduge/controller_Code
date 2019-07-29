#ifndef SYSTESTLISTWIDGTITEM_H
#define SYSTESTLISTWIDGTITEM_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include "cbitmapbutton.h"

#define SYSTEM_TEST_LIST_WIDGET_HEIGHT (40)
#define SYSTEM_TEST_LIST_WIDGET_WIDTH  (760)

class SysTestPage;

typedef enum 
{
   SYSTEM_TEST_ITEM_TYPE_SWITCH = 0,
   SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE,
   SYSTEM_TEST_ITEM_TYPE_RADIO_AND_VALUE,
   SYSTEM_TEST_ITEM_TYPE_HO_SWITCH
   
}SYSTEM_TEST_ITEM_TYPE_ENUM;

class Systestlistwidgtitem : public QWidget
{
    Q_OBJECT
public:
    explicit Systestlistwidgtitem(QWidget *parent = 0,SysTestPage *owner=0,int iType =0,int id=0);
    
    void setP1(const QString &strText){m_lbP1Value->setText(strText);}
    void setP2(const QString &strText){if(m_lbP2Value)m_lbP2Value->setText(strText);}
    void setName(const QString &strText) {m_lbName->setText(strText);}
    void setP1Name(const QString &strText) {m_lbP1Name->setText(strText);}
    void setP2Name(const QString &strText) {if(m_lbP2Name)m_lbP2Name->setText(strText);}
    void setRadioName(const QStringList &list)
    {
        int iLoop;
        for (iLoop = 0; iLoop < list.size(); iLoop++)
        {
            m_cmbAction->setItemText(iLoop, list[iLoop]);
        }
    }
    int getType(){return m_iType;}
    
    int getSwitchState() {return  (NULL != m_pBtnSwitch) ? m_pBtnSwitch->getState() : BITMAPBUTTON_STATE_UNSEL;}

    int getCombSelection() {return  (NULL != m_cmbAction) ? m_cmbAction->currentIndex() : -1;}

    void toggleSwitchState() {if  (NULL != m_pBtnSwitch)  m_pBtnSwitch->toggleState();}

    void setSwitchState(int state) {if  (NULL != m_pBtnSwitch)  m_pBtnSwitch->setState(state);}

    void changeCombSelection()
    {
        int curSel = m_cmbAction->currentIndex();
        curSel = (curSel + 1) % 4;
        m_cmbAction->setCurrentIndex(curSel);
    }

signals:
    void combActionChange(int);
public slots:
    void on_combAction_change(int);    
private:

    QLabel *m_lbName;

    CBitmapButton *m_pBtnSwitch;
    
    QComboBox  *m_cmbAction;
    
    QLabel        *m_lbP1Name;
    QLabel        *m_lbP1Value;
    
    QLabel        *m_lbP2Name;
    QLabel        *m_lbP2Value;

    int            m_iType;
    int            m_iId;
    SysTestPage   *m_pOwner;
};
    

#endif // SYSTESTLISTWIDGTITEM_H
