#ifndef DSLEEPWIDGET_H
#define DSLEEPWIDGET_H

#include <QWidget>

class DSleepWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DSleepWidget(const QString& msg, int screenWidth, int screenHeight, QWidget *parent = 0);

    void setMsg(const QString& msg);
    void setPos(int x, int y);

protected:
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:

private:
    int m_screenWidth;
    int m_screenHeight;
    QString m_msg;
    QPoint m_pos;
    
};

#endif // EX_DSLEEPWIDGET_H
