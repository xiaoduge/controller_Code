#include "dwaterqualitywidget.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QLabel>

DWaterQualityWidget::DWaterQualityWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void DWaterQualityWidget::setConfigList(const QList<DTags> &list)
{
    initLabel(list);
}

void DWaterQualityWidget::updateValue(const DTags& t, const QString& value1, const QString& value2)
{
    QMutexLocker locker(&m_mutex);

    DTags tag = t;

    int iType = tag.type();
    QString str = tag.tag();
    QPoint point = m_coorMap.value(str);

    switch (iType)
    {
    case 1:
        updateValueOne(point, value1);
        break;
    case 2:
        updateValueTwo(point, value1, value2);
        break;
    default:
        break;
    }
}

void DWaterQualityWidget::initUI()
{
    m_pTableWidget = new QTableWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_pTableWidget);
    this->setLayout(layout);
    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pTableWidget->setColumnCount(6);
    for(int i = 0; i < 6; ++i)
    {
        if(0 == i || 3 == i)
        {
            m_pTableWidget->horizontalHeader()->setResizeMode(i, QHeaderView::Fixed);
            m_pTableWidget->setColumnWidth(i, 150);
            m_pTableWidget->setColumnWidth(i, 150);
        }
        else
        {
            m_pTableWidget->horizontalHeader()->setResizeMode(i, QHeaderView::Stretch);
        }
    }
    m_pTableWidget->horizontalHeader()->hide();
    m_pTableWidget->verticalHeader()->hide();
}

void DWaterQualityWidget::initLabel(const QList<DTags>& configList)
{
    QMutexLocker locker(&m_mutex);

    if(configList.isEmpty())
    {
        return;
    }

    int count = configList.size();
    int rows = count%2 == 0 ? count/2 : count/2+1;
    m_pTableWidget->setRowCount(rows);
    if(count%2)
    {
        count += 1;
    }
    for(int i = 0; i < count; ++i)
    {
        DTags tags;
        QString str;
        QTableWidgetItem* item;
        QLabel* label = new QLabel(m_pTableWidget);
        label->setFrameStyle(QFrame::Panel | QFrame::Raised);

        if(i < configList.size())
        {
            tags = configList.at(i);
            str = tags.tag();
            item = new QTableWidgetItem(str);
        }
        else
        {
            item = new QTableWidgetItem("/");
        }

        item->setBackgroundColor(QColor(192,192,192));
        item->setTextAlignment(Qt::AlignCenter);

//        item->setTextColor(Qt::white);
        int row = i/2;
        int col = i%2;
        if(col != 0)
        {
            col += 2;
        }
        m_pTableWidget->setCellWidget(row, col, label);
        m_pTableWidget->setItem(row, col, item);
        m_coorMap.insert(str, QPoint(row, col));

        if(1 == tags.type())
        {
            m_pTableWidget->setSpan(row, col + 1, 1, 2);
        }
    }

    for(int i = 0; i < rows; ++i)
    {
        m_pTableWidget->setRowHeight(i, 40);
    }
}

void DWaterQualityWidget::updateValueOne(const QPoint &point, const QString &value)
{
    int row = point.x();
    int col = point.y() + 1;

    QTableWidgetItem* item = new QTableWidgetItem(value);
    item->setTextAlignment(Qt::AlignCenter);
    m_pTableWidget->setItem(row, col, item);

}

void DWaterQualityWidget::updateValueTwo(const QPoint &point, const QString &value1, const QString &value2)
{
    updateValueOne(point, value1);
    QPoint point2(point.x(), point.y() + 1);
    updateValueOne(point2, value2);
}

