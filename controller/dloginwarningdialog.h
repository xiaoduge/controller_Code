#ifndef DLOGINWARNINGDIALOG_H
#define DLOGINWARNINGDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;
class QPushButton;

class DLoginWarningDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DLoginWarningDialog(QWidget *parent = 0);
    DLoginWarningDialog(const DLoginWarningDialog &obj) = delete;
    DLoginWarningDialog& operator =(const DLoginWarningDialog &obj) = delete;
    ~DLoginWarningDialog();

signals:

public:
    static void getInstance(QString strText);

protected slots:
    void on_closeBtn_clicked();

    void on_timer_event();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    void setInfo(QString strText);
    void start();
    void deleteInstance();

private:
    QLabel *m_pLbInfo;
    QVBoxLayout *m_pBottomLayout;
    QTimer* m_timer;
    QPushButton* m_pColoseBtn;
    QPoint m_offsetPoint;

    static DLoginWarningDialog *instance;
};

#endif // DLOGINWARNINGDIALOG_H
