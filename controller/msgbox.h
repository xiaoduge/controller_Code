#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>

class QLabel;
class CBitmapButton;

class MsgBox : public QDialog
{
    Q_OBJECT
public:
    explicit MsgBox(QWidget *parent = 0);

    void setInfo(QString info);

private:
    QLabel * lbmsg;
    CBitmapButton *CancelBtn;
    CBitmapButton *SaveBtn;
    
public slots:
    void on_Ok_clicked(int);
    void on_Cal_clicked(int);
    
};

#endif // MSGBOX_H
