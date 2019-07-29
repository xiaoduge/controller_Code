#ifndef ALARMSETLISTWIDGTITEM_H
#define ALARMSETLISTWIDGTITEM_H

#include <QLabel>
#include "cbitmapbutton.h"


#define ALARM_SET_LIST_WIDGET_HEIGHT (30)
#define ALARM_SET_LIST_WIDGET_WIDTH  (760)

class AlarmSetPage;
class CBitmapButton;
class CBitmapButton;

typedef enum 
{
  ALARMSET_ITEM_TYPE_SWITCH = 0
   
}ALARM_SET_ITEM_TYPE_ENUM;

class AlarmSetListwidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmSetListwidgtItem(QWidget *parent = 0,AlarmSetPage *owner=0,int iType =0,int id=0);
    
    void setName(const QString &strText) {m_lbName->setText(strText);}

    int getType(){return m_iType;}
    
    int getSwitchState() {return  (NULL != m_pBtnSwitch) ? m_pBtnSwitch->getState() : BITMAPBUTTON_STATE_UNSEL;}

    void toggleSwitchState() {if  (NULL != m_pBtnSwitch)  m_pBtnSwitch->toggleState();}

    void setSwitchState(int state) {if  (NULL != m_pBtnSwitch)  m_pBtnSwitch->setState(state);}

private:

    QLabel *m_lbName;

    CBitmapButton *m_pBtnSwitch;

    int            m_iType;
    int            m_iId;
    AlarmSetPage   *m_pOwner;
};
    

#endif // SYSTESTLISTWIDGTITEM_H
