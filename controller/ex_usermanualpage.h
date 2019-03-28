#ifndef EX_USERMANUALPAGE_H
#define EX_USERMANUALPAGE_H


#include <QObject>
#include <QPixmap>
#include <QPalette>

#include "basewidget.h"
#include "subpage.h"

#include "Display.h"
#include "ctrlapplication.h"


class MainWindow;
class QPushButton;
class QLabel;


class Ex_UserManualPage : public CSubPage
{
    Q_OBJECT

public:
    Ex_UserManualPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();


protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void initMaxPageNum();
    void initPixNameList();

    void buildTitles();
    void setBackColor();
    void createControl();
    void loadNewContent(bool next);

private:
    QLabel* m_pContentLb;

    int m_lstX,m_lstY,m_lstFlag;
    int m_curX,m_curY;

    int m_curPage;

    QPixmap m_pixmap;

    QStringList m_strList;

    int m_maxPageNum[MACHINE_NUM];


private slots:


};

extern CtrlApplication *gApp;



#endif // EX_USERMANUALPAGE_H
