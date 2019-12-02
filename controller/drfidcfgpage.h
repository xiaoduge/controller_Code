#ifndef DRFIDCFGPAGE_H
#define DRFIDCFGPAGE_H

#include "subpage.h"

class MainWindow;
class QPushButton;
class QLabel;
class QCheckBox;

class DRFIDCfgPage : public CSubPage
{
    Q_OBJECT

public:
    DRFIDCfgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();
    void createHeader();

private:
    void buildTitles();
    void save();
    void setBackColor();
    void createControl();
    void connectData();

    QLabel* m_pExLbTitle;

    QCheckBox *m_chRfid;
    QPushButton* m_saveBtn;

private slots:
    void on_saveBtn_clicked();

};


#endif // EX_RFIDCFGPAGE_H
