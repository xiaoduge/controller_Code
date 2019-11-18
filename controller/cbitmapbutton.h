#ifndef CBITMAPBUTTON_H
#define CBITMAPBUTTON_H

#include <QWidget>

typedef enum
{
    BITMAPBUTTON_ICON_TOPLEFT,
    BITMAPBUTTON_ICON_CENTER,
    BITMAPBUTTON_ICON_VCENTER,
    BITMAPBUTTON_ICON_TOPCENTER,
    BITMAPBUTTON_ICON_BOTTOMCENTER,
    BITMAPBUTTON_ICON_BCENTER,
    BITMAPBUTTON_ICON_COCENTER,
    BITMAPBUTTON_ICON_NUM
}BITMAPBUTTON_ICON_ENUM;

typedef enum
{
    BITMAPBUTTON_TIP_TOPCENTER,
    BITMAPBUTTON_TIP_CENTER,
    BITMAPBUTTON_TIP_BOTTOMCENTER,
    BITMAPBUTTON_TIP_LEFTCENTER,
    BITMAPBUTTON_TIP_RIGHTCENTER,
    BITMAPBUTTON_TIP_NUM
}BITMAPBUTTON_TIP_ENUM;


typedef enum
{
    BITMAPBUTTON_STYLE_PUSH,
    BITMAPBUTTON_STYLE_CHECK,
    BITMAPBUTTON_STYLE_SWITCH,
    BITMAPBUTTON_STYLE_NUM
}BITMAPBUTTON_STYLE_ENUM;

typedef enum
{
    BITMAPBUTTON_STATE_UNSEL = 0,
    BITMAPBUTTON_STATE_SEL,
    BITMAPBUTTON_STATE_NUM
}BITMAPBUTTON_STATE_ENUM;

typedef enum
{
    BITMAPBUTTON_PIC_STYLE_NORMAL = 0,
    BITMAPBUTTON_PIC_STYLE_SINGLE,
    BITMAPBUTTON_PIC_STYLE_NUM
}BITMAPBUTTON_PIC_STYLE_ENUM;


class CBitmapButton : public QWidget
{
    Q_OBJECT
public:
    explicit CBitmapButton(QWidget *parent = 0,int style = BITMAPBUTTON_STYLE_PUSH,int bgStyle = BITMAPBUTTON_PIC_STYLE_NORMAL,int id = 0);
    void setPressPicture(QPixmap *pic)  ;
    void setButtonPicture(QPixmap *pic) ;
    void setTip(QString str,QColor color ,QPixmap *pIcon=NULL,int iconpos = BITMAPBUTTON_ICON_VCENTER,int txtpos = 0);
    void setTip(QPixmap *pIcon=NULL,int iconpos = BITMAPBUTTON_ICON_VCENTER);
    void setTip(QString str,QColor color,int txtpos = 0);
    void setTip(QString str,int txtpos = 0);
    void setState(int state);
    int  getPicWidth(){return buttonPicture ? buttonPicture->width() : (pressPicture ? pressPicture->width(): 0);}
    int  getPicHeight(){return buttonPicture ? buttonPicture->height() : (pressPicture ? pressPicture->height(): 0);}
    int  getIconWidth(){return pmIcon ? pmIcon->width() : 0;}
    int  getIconHeight(){return pmIcon ? pmIcon->height() : 0;}
    int  getState(){return btnState;}
    void toggleState();
    void enable(bool bEnable);
    void setFont(QFont *font) {titleFont = font;}
    void setText(QString str,int state = BITMAPBUTTON_STATE_UNSEL);
    void setColor(QColor color,int state = BITMAPBUTTON_STATE_UNSEL);
    void setId(int id) {iBtnId = id;}
    QString& getTip() {return strTip;}

signals:
    void clicked();
    void clicked(int);
    void clicking(int);

public slots:

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    
protected:
    virtual void paintEvent(QPaintEvent *pe);

private:
    QPixmap *pressPicture;
    QPixmap *buttonPicture;
    QPixmap *curPicture;

    QString strTip;
    QColor  colorText;
    QPixmap *pmIcon;
    int     iconposType;
    int     txtposType;
    int     btnStyle;
    int     btnState;
    int     iBtnId;
    int     picStyle;

    bool    bEnable;

    QFont   *titleFont;

    /* 2017/10/24 add */
    QString strSelectTip;
    QColor  colorSelectText;

    void setIcon(QPixmap *pic);

};

#endif // CBITMAPBUTTON_H
