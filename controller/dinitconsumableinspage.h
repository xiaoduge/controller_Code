#ifndef DINITCONSUMABLEINSPAGE_H
#define DINITCONSUMABLEINSPAGE_H

#include "subpage.h"

class QTableWidget;

class DInitConsumableInsPage : public CSubPage
{
    Q_OBJECT

    enum Consumable_Type
    {
        Type0 = 0,  //Pack
        Type1,
        Consumable_Type_Num
    };

    struct Consumable_Install_Info
    {
       int iRfid;
       QString strName;
    };

public:
    DInitConsumableInsPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    void switchLanguage();
    void buildTranslation();
    void initUi();

    void show(bool bShow);
    void activeReadRFID();

public slots:
    void updateConsumableInstall(int type);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void setBackColor();

    void initPackConfig();
    void initOtherConfig();

    void initTable();
    void initTypePackTable();
    void initTypeOtherTable();

    void updatePackInstall(int type);
    void updateOtherInstall(int type);

private slots:
    void on_ExNextBtn_clicked();
    void on_ExBackBtn_clicked();

    void on_timer_event();

private:
    QMap<int, Consumable_Install_Info> m_map[Consumable_Type_Num];
    QMap<int, QPoint> m_coorMap[Consumable_Type_Num];
    QList<int> m_list[Consumable_Type_Num];

    QLabel* m_pTitleLb;
    QLabel* m_pSuggestLb[Consumable_Type_Num];

    QTableWidget* m_pTableWidget[Consumable_Type_Num];

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

    int m_firstTableHeight;

    QTimer* m_pTimer;
};

#endif // EX_INITCONSUMABLEINSPAGE_H
