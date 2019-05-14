#ifndef CPAGE_H
#define CPAGE_H

#include <QObject>
#include <QDebug>
#include <QLabel>

#include "basewidget.h"

class CBaseWidget;
class MainWindow;

class CPage : public QObject
{
    Q_OBJECT
public:
    explicit CPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow  *wndMain = 0);

    void setTitles(QStringList &lstTitle) {m_titleList = lstTitle;}
    
    void selectTitle(int iSel) {m_curTitle = iSel;if (m_pLabelTitle) m_pLabelTitle->setText(m_titleList[m_curTitle]);}

    QString getTitle() {return m_titleList[m_curTitle];}

    int  titleIndex() {return m_curTitle;}

    virtual void creatTitle();
    
    virtual void update(){}

    virtual void fade(){};

    virtual void subPageEnd(bool ) {}

    virtual void switchLanguage(){}

    virtual void buildTranslation(){};

    virtual void initUi() {};

    virtual void show(bool bShow);


    virtual void mousePressEvent(QMouseEvent *){}
    virtual void mouseReleaseEvent(QMouseEvent *){}
    virtual void mouseMoveEvent(QMouseEvent * ){}

    
    bool isVisible() { return m_widget->isVisible();}
    void setWidgetVisible(bool isVisible);

    int width() {return m_widget->geometry().width();}
    int height() {return m_widget->geometry().height();}

protected:
    CBaseWidget *m_widget  ;
    MainWindow  *m_wndMain;
    CPage       *m_parent;
    QStringList m_titleList;
    int         m_curTitle; // index into m_titleList
    QLabel      *m_pLabelTitle;
    int         m_lstX,m_lstY,m_lstFlag;
    int         m_curX,m_curY;
signals:

public slots:
};

#endif // CPAGE_H
