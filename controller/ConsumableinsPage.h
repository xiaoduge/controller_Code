#ifndef CONSUMABLEINSPAGE_H
#define CONSUMABLEINSPAGE_H


#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "consumableinslistwidgtitem.h"
#include "Display.h"

#define MAX_ConsumableNum (DISP_CM_NAME_NUM  + DISP_MACHINERY_NAME_NUM)

class MainWindow;

class titleBar;


class ConsumableInsPage : public CSubPage
{
    Q_OBJECT

public:
    ConsumableInsPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    
    virtual void update();

    virtual void fade();

    void updateRfidInfo(int iRfId);

private:
    void createList();

    void setBackColor();

    void buildTitles();

    void toCurrentItem(int index);

    void initTypeMap();

private:
    QListWidget *listWidget;

    QListWidgetItem *listWidgetIem[MAX_ConsumableNum];

    CosumableInsListWidgtItem *m_aInsListItem[MAX_ConsumableNum];

    QString m_strUserName;

    int     m_iRealItemNum;

    int     m_iViewItemNum;

    int     m_iCurrentItem;

    struct CI_INFO{
       int iType;
       int iId;
       int iRfid; /*0xff for items not using rfid or having no corresponding RFID reader */
    }aIds[MAX_ConsumableNum];

    QMap<short int, short int> m_typeMap;

public slots:
    void on_btn_clicked(int index);

    void ItemClicked(QListWidgetItem *item);
};

#endif // CONSUMABLEINSPAGE_H
