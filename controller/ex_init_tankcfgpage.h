#ifndef EX_INIT_TANKCFGPAGE_H
#define EX_INIT_TANKCFGPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>

#include "basewidget.h"
#include "subpage.h"

#include "Display.h"
#include "ctrlapplication.h"


class MainWindow;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;

class Ex_Init_Tankcfgpage : public CSubPage
{
    Q_OBJECT

public:
    Ex_Init_Tankcfgpage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

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

    QLabel    *m_lbPWTankName;
    QComboBox *m_cmbPWTankVolume;
    QLineEdit *m_lePWTankHeight;
    QLabel    *m_lbPWHUnit;
    QLineEdit *m_lePWTankCap;
    QLabel    *m_lbPWCUnit;

    QLabel    *m_lbSWTankName;
    QComboBox *m_cmbSWTankVolume;
    QLineEdit *m_leSWTankHeight;
    QLabel    *m_lbSWHUnit;
    QLineEdit *m_leSWTankCap;
    QLabel    *m_lbSWCUnit;

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

public slots:
    void on_CmbIndexChange_pw(int index);
    void on_CmbIndexChange_sw(int index);

    void on_ExNextBtn_clicked();
    void on_ExBackBtn_clicked();

};

extern CtrlApplication *gApp;

#endif // EX_INIT_TANKCFGPAGE_H
