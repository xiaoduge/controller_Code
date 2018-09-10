#ifndef CSLISTITEM_H
#define CSLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "cbitmapbutton.h"

class ConsumableStatePage;

#define CS_LIST_WIDGET_HEIGHT (65)
#define CS_LIST_WIDGET_WIDTH  (760)

class CsListItem : public QWidget
{
    Q_OBJECT
public:
    explicit CsListItem(QWidget *parent = 0, ConsumableStatePage *owner=0, int id=0);

    void setName(QString strText) {if (lbName) lbName->setText(strText);} 
    void setValue(QString &strText) {if (lbWaterValue) lbWaterValue->setText(strText);} 
    void setInstDate(QString &strText) {if (lbInstDate) lbInstDate->setText(strText);} 
    void setChangeDate(QString &strText) {if (lbChangeDate) lbChangeDate->setText(strText);} 
    void setCatNo(QString &strText) {if (lbCatNo) lbCatNo->setText(strText);} 
    void setLotNo(QString &strText) {if (lbLotNo) lbLotNo->setText(strText);} 

    void updateState(int type);
    
signals:
    
public slots:
    
private:
    QLabel *lbName;
    
    QLabel *lbPic;
    QLabel *lbWaterValue;
    
    QLabel *lbInstDate;
    QLabel *lbChangeDate;

    QLabel *lbCatNo;
    QLabel *lbLotNo;

    CBitmapButton *btnShopCar;
    ConsumableStatePage *m_pOwner;
    int  m_id;
    

};

#endif // CSLISTITEM_H
