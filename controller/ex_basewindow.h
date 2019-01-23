#ifndef BASEWINDOW_H
#define BASEWINDOW_H


#include <QWidget>

class Ex_DialogTitle;

class Ex_BaseWindow : public QWidget
{
    Q_OBJECT

public:
    Ex_BaseWindow(QWidget *parent = 0);
    ~Ex_BaseWindow();

    void setTitleText(const QString &titleContent, const QColor &textColor = Qt::black, int titleFontSize = 10);
    virtual void updatePage() = 0;

protected:
    void showEvent(QShowEvent *event);

private:
    void initTitleBar();
    void paintEvent(QPaintEvent *event);

private:
    Ex_DialogTitle* m_titleBar;

};


#endif // BASEWINDOW_H
