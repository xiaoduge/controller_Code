#ifndef CONSUMABLE_INS_LISTWIDGTITEM_H
#define CONSUMABLE_INS_LISTWIDGTITEM_H

#include <QWidget>
#include <QLabel>
#include "cbitmapbutton.h"
#include "dlineedit.h"

#define CI_LIST_WIDGET_HEIGHT (50)
#define CI_LIST_WIDGET_WIDTH  (760)

class ConsumableInsPage;

class CosumableInsListWidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit CosumableInsListWidgtItem(QWidget *parent = 0,ConsumableInsPage * owner=0,int id=0);
    
    void setP1(const QString &strText){m_editP1Value->setText(strText);}
    
    QString getP1(){return m_editP1Value->text();}

    void setP2(const QString &strText){m_editP2Value->setText(strText);}
    
    QString getP2(){return m_editP2Value->text();}

    void setName(const QString &strText) {m_lbName->setText(strText);}

    void setCatName(const QString &strText) {m_lbCatName->setText(strText);}
    void setLotName(const QString &strText) {m_lbLotName->setText(strText);}
    
    void setP2Name(const QString &strText) {m_btnInstall->setTip(strText);}
    
    void setItemId(int id) {m_iId = id;}

    void setBtnId(int id) {m_btnInstall->setId(id);}

    int  getItemId() {return m_iId;}
    
signals:

public slots:

private:

    QLabel     *m_lbName;

    QLabel     *m_lbCatName;
    QLabel     *m_lbLotName;
    DLineEdit  *m_editP1Value;
    DLineEdit  *m_editP2Value;

    CBitmapButton *m_btnInstall;

    int        m_iId;

    ConsumableInsPage *m_owner;
};

#endif // CONSUMABLE_INS_LISTWIDGTITEM_H
