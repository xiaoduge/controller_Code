#ifndef DHINTDIALOG_H
#define DHINTDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;

class DHintDialog : public QDialog
{
    Q_OBJECT
private:
    explicit DHintDialog(QWidget *parent = 0);
    DHintDialog(const DHintDialog &obj);
    DHintDialog& operator =(const DHintDialog &obj);

public:
    static void getInstance(QString strText);
    ~DHintDialog();

protected slots:
    void on_timer_event();

private:
    void setInfo(QString strText);
    void start();

private:
    QLabel *m_pLbInfo;
    QVBoxLayout *m_pBottomLayout;
    QTimer* m_timer;

    static DHintDialog *instance;
    
};

#endif // EX_HINTDIALOG_H
