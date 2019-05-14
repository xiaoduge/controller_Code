#ifndef SYSTEMMONITORLISTWIDGTITEM_H
#define SYSTEMMONITORLISTWIDGTITEM_H

#include <QWidget>
#include <QLabel>

#include <QWidget>
#include "cbitmapbutton.h"

enum SYSMONI_LIST_ITEM_ENUM
{
    SYSMONI_LIST_ITEM_SINGLE_TEXT = 0,
    SYSMONI_LIST_ITEM_SINGLE_SWITCH,
    SYSMONI_LIST_ITEM_DOUBLE_SWITCH,
    SYSMONI_LIST_ITEM_SINGLE_SWITCH_VALUE,
    SYSMONI_LIST_ITEM_DOUBLE_SWITCH_VALUE,
    SYSMONI_LIST_ITEM_SINGLE_SWITCH_DOUBLE_VALUE,
    SYSMONI_LIST_ITEM_TRIPLE_TEXT,
    SYSMONI_LIST_ITEM_NUM,

};

#define SYSMONI_ITEM_HEIGHT 40
#define SYSMONI_ITEM_WIDTH (780)

class SystemMonitorListWidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit SystemMonitorListWidgtItem(QWidget *parent,int iType,int iId,int iSingleIdx);

    void setSwitchImage(QPixmap *imgOn,QPixmap *imgOff){m_imgSwitchOn = imgOn;m_imgSwitchOff = imgOff;};
    void setName1(QString &strName) {if (m_lbName1) m_lbName1->setText(strName);}
    void setSwitch1(bool bOn) {if (m_lbSwitch1) m_lbSwitch1->setPixmap(bOn ? *m_imgSwitchOn : *m_imgSwitchOff);}
    void setValue1(QString &strValue) {if (m_lbValue1) m_lbValue1->setText(strValue);}
    void setName2(QString &strName) {if (m_lbName2) m_lbName2->setText(strName);}
    void setSwitch2(bool bOn) {if (m_lbSwitch2) m_lbSwitch2->setPixmap(bOn ? *m_imgSwitchOn : *m_imgSwitchOff);}
    void setValue2(QString &strValue) {if (m_lbValue2) m_lbValue2->setText(strValue);}
    void setValue3(QString &strValue) {if (m_lbValue3) m_lbValue3->setText(strValue);}

signals:
    
public slots:
    
private:

    QLabel *m_lbName1;
    QLabel *m_lbSwitch1;
    QLabel *m_lbValue1;

    QLabel *m_lbName2;
    QLabel *m_lbSwitch2;
    QLabel *m_lbValue2;

    QLabel *m_lbValue3;
    
    QPixmap *m_imgSwitchOn;
    QPixmap *m_imgSwitchOff;

    int m_iId;
    int m_iType;  /* refer SYSMONI_LIST_ITEM_ENUM */
    int m_iSingleIdx;

};

#endif // WARNLISTWIDGTITEM_H
