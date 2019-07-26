#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include <QComboBox>
#include <QCalendarWidget>

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
    
signals:
    
public slots:

    void on_Ok_clicked(int);

    void on_Cal_clicked(int);
    
};

#endif // MSGBOX_H
