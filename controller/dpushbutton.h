#ifndef DPUSHBUTTON_H
#define DPUSHBUTTON_H

#include <QPushButton>

class DPushButton : public QPushButton
{
public:
    explicit DPushButton(QWidget* parent = NULL);
    explicit DPushButton(const QString &text, QWidget* parent = NULL);
    explicit DPushButton(const QPixmap &pixmap, QWidget* parent = NULL);
    explicit DPushButton(const QPixmap &pixmap, const QString &text, QWidget* parent = NULL);

    void setPixmapScale(double i);
    void setPixmap(const QPixmap &pixmap);
    void setTextColor(const QColor &color);
    void setTextFont(const QFont &font);
    void setText(const QString& text);

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QString m_text;
    QPixmap m_pixmap;
    QColor m_penColor;
    QFont m_font;

    float m_size;

    bool m_containText;
    bool m_containPixmap;

    bool m_isPress;

};

#endif // DPUSHBUTTON_H
