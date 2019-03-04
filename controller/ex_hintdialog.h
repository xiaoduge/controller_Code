#ifndef EX_HINTDIALOG_H
#define EX_HINTDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;

class Ex_HintDialog : public QDialog
{
    Q_OBJECT
public:
    explicit Ex_HintDialog(QWidget *parent = 0);
    Ex_HintDialog(const Ex_HintDialog &obj) = delete;
    Ex_HintDialog& operator =(const Ex_HintDialog &obj) = delete;
    ~Ex_HintDialog();
    
signals:
    
public:
    static void getInstance(QString strText);

protected slots:
    void on_timer_event();

private:
    void setInfo(QString strText);
    void start();

private:
    QLabel *m_pLbInfo;
    QVBoxLayout *m_pBottomLayout;
    QTimer* m_timer;

    static Ex_HintDialog *instance;
    
};

#endif // EX_HINTDIALOG_H