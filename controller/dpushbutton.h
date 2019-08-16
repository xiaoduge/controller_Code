#ifndef DPUSHBUTTON_H
#define DPUSHBUTTON_H

#include <QPushButton>
#include <QMutex>

class DPushButton : public QPushButton
{
    Q_OBJECT
public:
    enum MoveRule
    {
        NormalMove = 0,
        CenterMove
    };

public:
    explicit DPushButton(QWidget* parent = NULL, int id = 0);
    explicit DPushButton(const QString &text, QWidget* parent = NULL, int id = 0);
    explicit DPushButton(const QPixmap &pixmap, QWidget* parent = NULL, int id = 0);
    explicit DPushButton(const QPixmap &pixmap, const QString &text, QWidget* parent = NULL, int id = 0);

    void setPixmapScale(double i);
    void setPixmap(const QPixmap &pixmap);
    void setTextColor(const QColor &color);
    void setFontSize(int size);
    void setText(const QString& text);
    void setID(int id);
    void setMoveRule(MoveRule rule);

    void cmove(int x, int y);
    void cmove(const QPoint& point);

    int id() const;
    const QPoint& originalPos() const;

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void initSize();


private:
    QString m_text;
    QPixmap m_pixmap;
    QColor m_penColor;
    QFont m_font;
    int m_id;

    int m_width;
    int m_height;
    MoveRule m_moveRule;
    QPoint m_point;

    float m_size;

    bool m_containText;
    bool m_containPixmap;

    bool m_isPress;
    bool m_initSize;

};

#endif // DPUSHBUTTON_H
