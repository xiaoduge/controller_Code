#ifndef MAINTENANCELISTWIDGTITEM_H
#define MAINTENANCELISTWIDGTITEM_H

#include <QWidget>

#include <QLabel>
#include "dlineedit.h"

#define MTN_LIST_WIDGET_HEIGHT (60)

#define MTN_LIST_WIDGET_WIDTH (780)

class MaintenanceListWidgtItem : public QWidget
{
    Q_OBJECT
public:
    explicit MaintenanceListWidgtItem(QWidget *parent = 0,int items = 1);
    void setP1(const QString &strText){m_editP1Value->setText(strText);}
    void setP2(const QString &strText){if(m_editP2Value)m_editP2Value->setText(strText);}
    
    QString getP1(){return m_editP1Value->text();}
    QString getP2(){return (NULL != m_editP2Value) ? m_editP2Value->text() : "";}

    void setName(const QString &strText) {m_lbName->setText(strText);}
    void setP1Name(const QString &strText) {m_lbP1Name->setText(strText);}
    void setP2Name(const QString &strText) {if(m_lbP2Name)m_lbP2Name->setText(strText);}
    
    void setP1Range(int iMin,int iMax);
    
    void setP2Range(int iMin,int iMax);
signals:

public slots:

private:

    QLabel     *m_lbName;
    QLabel     *m_lbP1Name;
    DLineEdit  *m_editP1Value;
    QLabel     *m_lbP2Name;
    DLineEdit  *m_editP2Value;
    int        m_iItems;

    
};

#endif // MAINTENANCELISTWIDGTITEM_H
