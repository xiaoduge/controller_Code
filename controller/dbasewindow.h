#ifndef DBASEWINDOW_H
#define DBASEWINDOW_H

#include <QWidget>

class DDialogTitle;

class DBaseWindow : public QWidget
{
    Q_OBJECT

public:
    DBaseWindow(QWidget *parent = 0);
    ~DBaseWindow();

    void setTitleText(const QString &titleContent, const QColor &textColor = Qt::black, int titleFontSize = 10);
    virtual void updatePage() = 0;

protected:
    void showEvent(QShowEvent *event);

private:
    void initTitleBar();
    void paintEvent(QPaintEvent *event);

private:
    DDialogTitle* m_titleBar;

};


#endif // BASEWINDOW_H
