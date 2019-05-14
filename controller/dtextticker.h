#ifndef DTEXTTICKER_H
#define DTEXTTICKER_H

#include <QLabel>

class DTextTicker : public QLabel
{
    Q_OBJECT
public:
    DTextTicker(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

    void updateShowText(const QString& msg, bool add);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void ticker();

private:
    QStringList m_strList;
    QString m_showText;

    int m_charWidth;
    int m_posX;

    QTimer *m_timer;
};

#endif // DTEXTTICKER_H
