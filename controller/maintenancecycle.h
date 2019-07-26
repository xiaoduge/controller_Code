#ifndef MAINTENANCECYCLE_H
#define MAINTENANCECYCLE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>

#include "maintenancelistwidgtitem.h"
#include "Display.h"

#define MTCNUM  15

class MainWindow;

typedef struct
{
   int v1Min,v1Max;
   int v2Min,v2Max;
}VALIDATE_INFO_STRU;


class MaintenanceCyclePage : public CSubPage
{
    Q_OBJECT

public:
    MaintenanceCyclePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    
    virtual void update();


private:

    void setBackColor();

    void createList();

    void buildTitles();

    void save();

    struct MTC_ITEM_TYPE {
        int iDspType;
        int iId;
        VALIDATE_INFO_STRU vi;
    }aIds[MTCNUM];

    QListWidget *listWidget;

    QListWidgetItem *listWidgetIem[MTCNUM];

    MaintenanceListWidgtItem *pMaintenancelistItem[MTCNUM];

    CBitmapButton     *m_pBtnSave;

    int                m_iRealNum;
public slots:

    void on_btn_clicked(int tmp);
    void ItemClicked(QListWidgetItem * pItem);
};

#endif // MAINTENANCECYCLE_H
