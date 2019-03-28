#ifndef EX_USERCFGPAGE_H
#define EX_USERCFGPAGE_H

#include <QObject>
#include "basewidget.h"
#include "subpage.h"
#include "ctrlapplication.h"

class QPushButton;
class QLineEdit;
class QLabel;
class QTableView;
class QSqlTableModel;

class Ex_UserCfgPage : public CSubPage
{
    Q_OBJECT

public:
    Ex_UserCfgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void update();

protected slots:
    void on_QueryBtn_clicked();
    void on_DisplayAllBtn_Clicked();
    void on_SubmitBtn_clicked();
    void on_RevertBtn_clicked();
    void on_AddBtn_clicked();
    void on_DeleteRowBtn_clicked();

private:
    void buildTitles();
    void setBackColor();
    void createControl();

    void initDBTabelModel();
    void initTabelHeaderData();

private:

    QPushButton* m_pQueryBtn;
    QPushButton* m_pDisplayAllBtn;
    QPushButton* m_pSubmitBtn;
    QPushButton* m_pRevertBtn;
    QPushButton* m_pAddBtn;
    QPushButton* m_pDeleteRowBtn;

    QLineEdit* m_pNameLineEdit;
    QLabel* m_pPageHeaderLB;
    QLabel* m_pNameLB;

    QTableView* m_pTableView;

    QSqlTableModel* m_pTableModel;

};


#endif // EX_USERCFGPAGE_H
