#ifndef QSLIDER_EX_H
#define QSLIDER_EX_H

#include <QSlider>


class CSliderEx : public QSlider
{
    Q_OBJECT
public:
    explicit CSliderEx(Qt::Orientation orientation, QWidget *parent = 0,int id=0):
             QSlider(orientation,parent)
    {
        iCtrlid = id;

        connect(this,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed(int)));
    }

    void setId(int id) {iCtrlid = id;}
signals:
    void valueChanged(int,int);
public slots:

    void on_value_changed(int ivalue) 
    {
        emit valueChanged(iCtrlid,ivalue);
    }
private:
    int     iCtrlid;

};

#endif // QSLIDER_EX_H
