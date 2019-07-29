#ifndef ADCSLISTITEM_H
#define ADCSLISTITEM_H

#include <QLabel>

class AlarmDisplayPage;
class QLabel;

#define ADCS_LIST_WIDGET_HEIGHT (170)   //100
#define ADCS_LIST_WIDGET_WIDTH  (350)

class AdcsListItem : public QWidget
{
    Q_OBJECT
public:
    explicit AdcsListItem(QWidget *parent = 0, AlarmDisplayPage *owner=0);

    void setName(QString strText) {if (lbName) lbName->setText(strText);} 
    void setValue(QString &strText) {if (lbWaterValue) lbWaterValue->setText(strText);} 
    void setInstDate(QString &strText) {if (lbInstDate) lbInstDate->setText(strText);} 
    void setChangeDate(QString &strText) {if (lbChangeDate) lbChangeDate->setText(strText);} 
    void setCatNo(QString &strText) {if (lbCatNo) lbCatNo->setText(strText);} 
    void setLotNo(QString &strText) {if (lbLotNo) lbLotNo->setText(strText);} 

    void updateState(int type);
    void setId(int id){m_id = id;}
    
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

    AlarmDisplayPage *m_pOwner;
    int  m_id;
    

};

#define ADAS_LIST_WIDGET_HEIGHT (30)
#define ADAS_LIST_WIDGET_WIDTH  (350)


class AdasListItem : public QWidget
{
    Q_OBJECT
public:
    explicit AdasListItem(QWidget *parent = 0,AlarmDisplayPage *owner=0);
    
    void setName(const QString &strText) {m_lbName->setText(strText);}
    void setId(int id){m_iId = id;}

signals:
    
public slots:
    
private:

    QLabel        *m_lbName;

    int            m_iId;
    AlarmDisplayPage   *m_pOwner;
};
    

#endif // CSLISTITEM_H
