#ifndef PARAMETERCALIBRATION_H
#define PARAMETERCALIBRATION_H

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
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "parameterlistwidgtitem.h"
#include "Display.h"

#define ParameterNUM DISP_PC_COFF_NUM

class MainWindow;

class ParameterCalibrationPage : public CSubPage
{
    Q_OBJECT

public:
    ParameterCalibrationPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    
    virtual void update();

private:

    void buildTitles();
    void save();
    void Create_list();

    void Set_Back_Color();


    QListWidget *listWidget;

    QListWidgetItem *listWidgetIem[ParameterNUM];

    Parameterlistwidgtitem *m_aParameterlistItem[ParameterNUM];

    CBitmapButton     *m_pBtnSave;

    int              m_iRealItems;

    struct EXE_VALVE_NAME
    {
        int      type;
        int      id;
    }aParameters[ParameterNUM];

public slots:
    
    void on_btn_clicked(int tmp);

    void ItemClicked(QListWidgetItem * item);
};

#endif // PARAMETERCALIBRATION_H
