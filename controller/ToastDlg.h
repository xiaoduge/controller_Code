#ifndef TOASTDLG_H
#define TOASTDLG_H

#include <QDialog>

class QLabel;
class QGridLayout;
class QVBoxLayout;

class ToastDlg : public QDialog
{
    Q_OBJECT
        
private:
    ToastDlg(QWidget *parent = 0);
    ToastDlg(const ToastDlg& obj);
    ToastDlg& operator =(const ToastDlg& obj);

public:    
    static void makeToast(QString strText);
    ~ToastDlg();

private slots:
    void on_timer_event();

private:

    void setInfo(QString strText);

    void start();

    QLabel *m_pLbInfo;

    QGridLayout *m_pMainLayout;

    QVBoxLayout *m_pBottomLayout;
    
    QTimer* m_timer;

    static ToastDlg *instance;

};
#endif // TOASTDLG_H
