#ifndef PARAMETERLISTWIDGTITEM_H
#define PARAMETERLISTWIDGTITEM_H

#include <QWidget>

#include <QLabel>
#include <QLineEdit>

enum PARAMETER_CALIBRATION_ENUM
{
    PARAMETER_CALIBRATION_FORMAT0 = 0,
    PARAMETER_CALIBRATION_FORMAT1,
    PARAMETER_CALIBRATION_FORMAT2,
};

#define PC_LIST_WIDGET_HEIGHT (50)
#define PC_LIST_WIDGET_WIDTH  (780)

class Parameterlistwidgtitem : public QWidget
{
    Q_OBJECT
public:
    explicit Parameterlistwidgtitem(QWidget *parent = 0,int iType=PARAMETER_CALIBRATION_FORMAT0,int id=0);
    
    void setP1(const QString &strText){m_editP1Value->setText(strText);}
    
    QString getP1(){return m_editP1Value->text();}

    void setP2(const QString &strText){if (m_editP2Value) m_editP2Value->setText(strText);}
    
    QString getP2(){return m_editP2Value ? m_editP2Value->text() : "0";}

    void setP3(const QString &strText){if (m_editP3Value) m_editP3Value->setText(strText);}
    
    QString getP3(){return m_editP3Value ?  m_editP3Value->text() : "0";}

    void setName(const QString &strText) {m_lbName->setText(strText);}
    
    void setP1Name(const QString &strText) {m_lbP1Name->setText(strText);}
    void setP2Name(const QString &strText) {if (m_lbP2Name) m_lbP2Name->setText(strText);}
    void setP3Name(const QString &strText) {if (m_lbP3Name) m_lbP3Name->setText(strText);}

signals:

public slots:

private:
    
    QLabel     *m_lbName;
    QLineEdit  *m_editP1Value;
    QLineEdit  *m_editP2Value;
    QLineEdit  *m_editP3Value;
    QLabel     *m_lbP1Name;
    QLabel     *m_lbP2Name;
    QLabel     *m_lbP3Name;

    int        m_iId;
    int        m_iType;

    
};

#endif // PARAMETERLISTWIDGTITEM_H
