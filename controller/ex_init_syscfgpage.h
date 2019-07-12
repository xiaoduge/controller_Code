#ifndef EX_INIT_SYSCFGPAGE_H
#define EX_INIT_SYSCFGPAGE_H

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
class QCheckBox;

class Ex_Init_Syscfgpage : public CSubPage
{
    Q_OBJECT

public:
    enum BTN_NAME
    {
        DISP_CFG_ElecLeakProtector = 0,
        DISP_CFG_TankUV,
        DISP_CFG_NUMBER,
    };

public:
    Ex_Init_Syscfgpage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();
    void createHeader();
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void buildTitles();
    void save();
    void setBackColor();
    void createControl();
    void connectData();

    QLabel* m_pExLbTitle;

    QCheckBox *m_chCfg[DISP_CFG_NUMBER];

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

public slots:
    void on_ExNextBtn_clicked();
    void on_ExBackBtn_clicked();

};

extern CtrlApplication *gApp;


#endif // EX_INIT_SYSCFGPAGE_H
