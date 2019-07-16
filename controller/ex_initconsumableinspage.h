#ifndef EX_INITCONSUMABLEINSPAGE_H
#define EX_INITCONSUMABLEINSPAGE_H

#include "subpage.h"

class QTableWidget;

class Ex_InitConsumableInsPage : public CSubPage
{
    Q_OBJECT

    struct Consumable_Install_Info
    {
       int iType;
       int iRfid;
       QString strName;
    };

public:
    Ex_InitConsumableInsPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

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
    void initVector();
    void initTable();

private slots:
    void on_ExNextBtn_clicked();
    void on_ExBackBtn_clicked();

    void on_timer_event();

private:
    QMap<int, Consumable_Install_Info> m_map;
    QMap<int, QPoint> m_coorMap;
    QList<int> m_list;

    QLabel* m_pSuggestLb;
    QTableWidget* m_pTableWidget;

    QPushButton* m_pExNextBtn;
    QPushButton* m_pExBackBtn;

    QTimer* m_pTimer;
};

#endif // EX_INITCONSUMABLEINSPAGE_H
