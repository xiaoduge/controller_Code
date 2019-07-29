#ifndef DWIFICONFIGDIALOG_H
#define DWIFICONFIGDIALOG_H

#include "dwificonfigwidget.h"

class DLineEdit;
class QLabel;
class QPushButton;

class DWifiConfigDialog : public DWifiConfigWidget
{
    Q_OBJECT
public:
    DWifiConfigDialog(QWidget *parent = 0);

    void updatePage();

    void setSSIDName(const QString &name);

private slots:
    void on_configBtn_clicked();
    void on_cancelBtn_clicked();

private:
    void initUI();

private:
    QPushButton *m_pConfigBtn;
    QPushButton *m_pCancelBtn;

    DLineEdit *m_pPSKLineEdit;
    QLabel *m_pPSKLabel;
    QLabel *m_pSSIDLabel;
};

#endif // DWIFICONFIGDIALOG_H
