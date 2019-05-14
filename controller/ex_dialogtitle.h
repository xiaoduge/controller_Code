#ifndef EX_DIALOGTITLE_H
#define EX_DIALOGTITLE_H

#include <QWidget>

class QLabel;

class Ex_DialogTitle : public QWidget
{
    Q_OBJECT
public:
    explicit Ex_DialogTitle(QWidget *parent = 0);

    // 设置标题栏背景色及是否设置标题栏背景色透明;
    void setBackgroundColor(const QColor &color);
    // 设置标题内容;
    void setTitleContent(const QString &titleContent, const QColor &textColor = Qt::black, int titleFontSize = 10);
    // 设置标题栏长度;
    void setTitleWidth(int width);
    // 设置窗口边框宽度;
    void setWindowBorderWidth(int borderWidth);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void initUI();

signals:

public slots:

private:
    QColor m_backgroundColor;
    QLabel *m_pTitleLabel;

    QPoint m_startMovePos;
    bool m_isPressed;
    int m_borderWidth;
};

#endif // EX_DIALOGTITLE_H
