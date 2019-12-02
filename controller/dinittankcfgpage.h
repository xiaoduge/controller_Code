#ifndef DINITTANKCFGPAGE_H
#define DINITTANKCFGPAGE_H

#include "subpage.h"

class MainWindow;
class QPushButton;
class DLineEdit;
class QLabel;
class QComboBox;

class DInitTankcfgpage : public CSubPage
{
    Q_OBJECT

public:
    DInitTankcfgpage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

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
    DLineEdit *m_lePWTankHeight;
    QLabel    *m_lbPWHUnit;
    DLineEdit *m_lePWTankCap;
    QLabel    *m_lbPWCUnit;

    QLabel    *m_lbSWTankName;
    QComboBox *m_cmbSWTankVolume;
    DLineEdit *m_leSWTankHeight;
    QLabel    *m_lbSWHUnit;
    DLineEdit *m_leSWTankCap;
    QLabel    *m_lbSWCUnit;

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

public slots:
    void on_CmbIndexChange_pw(int index);
    void on_CmbIndexChange_sw(int index);

    void on_ExNextBtn_clicked();
    void on_ExBackBtn_clicked();

};

#endif // EX_INIT_TANKCFGPAGE_H
