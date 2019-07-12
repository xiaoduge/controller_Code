#ifndef DCONSUMABLEINSTALLWIDGET_H
#define DCONSUMABLEINSTALLWIDGET_H

#include <QWidget>

class QLabel;
class QCheckBox;

class DConsumableInstallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DConsumableInstallWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

signals:

public slots:
    void setTag(const QString& tags);
    void setInstallStatus(bool status);
    void setID(int id);
    int id();

private:
    void initUI();

private:
    QLabel* m_pTagLabel;
    QCheckBox* m_pInsCheckBox;

    int m_id;
};

#endif // DCONSUMABLEINSTALLWIDGET_H
