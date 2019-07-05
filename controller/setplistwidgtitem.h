#ifndef SETPLISTWIDGTITEM_H
#define SETPLISTWIDGTITEM_H

#include <QWidget>
#include <QLabel>
#include "dlineedit.h"

enum SET_POINT_ENUM
{
    SET_POINT_FORMAT1 = 1,
    SET_POINT_FORMAT2,
};

#define SET_P_LIST_WIDGET_HEIGHT (80)

class SetPlistwidgtitem : public QWidget
{
    Q_OBJECT
public:
    explicit SetPlistwidgtitem(QWidget *parent = 0,int items = 1);
    void setP1(const QString &strText){m_editP1Value->setText(strText);}
    void setP2(const QString &strText){if(m_editP2Value)m_editP2Value->setText(strText);}
    
    QString getP1(){return m_editP1Value->text();}
    QString getP2(){return (NULL != m_editP2Value) ? m_editP2Value->text() : "";}

    void setName(const QString &strText) {m_lbName->setText(strText);}
    void setP1Name(const QString &strText) {m_lbP1Name->setText(strText);}
    void setP2Name(const QString &strText) {if(m_lbP2Name)m_lbP2Name->setText(strText);}
    void setP1Unit(const QString &strText) {if (m_lbP1Unit) m_lbP1Unit->setText(strText);}
    void setP2Unit(const QString &strText) {if (m_lbP2Unit) m_lbP2Unit->setText(strText);}
    

signals:

public slots:

private:

    QLabel     *m_lbName;
    QLabel     *m_lbP1Name;
    DLineEdit  *m_editP1Value;
    QLabel     *m_lbP1Unit;
    
    QLabel     *m_lbP2Name;
    DLineEdit  *m_editP2Value;
    QLabel     *m_lbP2Unit;
    
    int        m_iItems;
};

#endif // SETPLISTWIDGTITEM_H
