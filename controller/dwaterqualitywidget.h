#ifndef DWATERQUALITYWIDGET_H
#define DWATERQUALITYWIDGET_H

#include <QWidget>
#include <QMap>
#include <QMutex>

class QTableWidget;


class DTags
{
public:
    DTags(){}
    DTags(const QString& str, int iType) : m_tag(str), m_type(iType)
    {

    }

    void setValue(const QString& str, int iType)
    {
        m_tag = str;
        m_type = iType;
    }

    const QString tag() const
    {
        return m_tag;
    }

    int type() const
    {
        return m_type;
    }

    bool operator ==(const DTags& obj)
    {
        return (this->m_tag == obj.m_tag) && (this->m_type == obj.m_type);
    }
private:
    QString m_tag;
    int m_type;
};

class DWaterQualityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DWaterQualityWidget(QWidget *parent = 0);

    void setConfigList(const QList<DTags>& list);
    void updateValue(const DTags& t, const QString& value1, const QString& value2 = "--");

private:
    void initUI();
    void initLabel(const QList<DTags>& configList);

    void updateValueOne(const QPoint& point, const QString& value);
    void updateValueTwo(const QPoint& point, const QString& value1, const QString& value2);

private:
    QTableWidget* m_pTableWidget;

    QMap<QString, QPoint> m_coorMap;

    QMutex m_mutex;
};

#endif // DWATERQUALITYWIDGET_H
