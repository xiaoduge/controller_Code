#ifndef SUBPAGE_H
#define SUBPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QListWidget>

#include "basewidget.h"
#include "page.h"
#include "cbitmapbutton.h"

class titleBar;

class CSubPage : public CPage
{
    Q_OBJECT
public:
    CSubPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();    
    virtual void leaveSubPage();
    void hideTitleBar();
protected:

    titleBar      *m_pTitleBar;

public slots:

    void on_navi_clicked(int tmp);

};

#endif // SUBPAGE_H
