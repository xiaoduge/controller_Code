#include "systestlistwidgtitem.h"
#include "cbitmapbutton.h"
#include "mainwindow.h"
#include "systestpage.h"

#define LEFT_MARGIN       5
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   180  //
#define VALUE_ITEM_WIDTH  80
#define ITEM_SPACE        (100)

Systestlistwidgtitem::Systestlistwidgtitem(QWidget *parent,SysTestPage *owner,int iType,int id) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;

    m_lbP1Name  = NULL;
    m_lbP1Value = NULL;
    m_lbP2Name  = NULL;
    m_lbP2Value = NULL;
    m_cmbAction    = NULL;
    m_pBtnSwitch= NULL;

    m_iType  = iType ;
    m_pOwner = owner;
    m_iId    = id;

    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(LEFT_MARGIN, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - TOP_MARGIN*2));
    xOffset += NAME_ITEM_WIDTH;

    xOffset += ITEM_SPACE;

    switch(iType)
    {
    case SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE:
        m_lbP1Name = new QLabel(this);
        m_lbP1Name->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
        m_lbP1Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += VALUE_ITEM_WIDTH;

        xOffset += 2;
        m_lbP1Value = new QLabel(this);
        m_lbP1Value->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
        m_lbP1Value->setAlignment(Qt::AlignCenter);  
        m_lbP1Value->setStyleSheet("QLabel{border-width:2;border-style:outset}");
        
    case SYSTEM_TEST_ITEM_TYPE_SWITCH:
    case SYSTEM_TEST_ITEM_TYPE_HO_SWITCH:
        m_pBtnSwitch = new CBitmapButton(this,BITMAPBUTTON_STYLE_SWITCH,BITMAPBUTTON_PIC_STYLE_NORMAL,m_iId);
        m_pBtnSwitch->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_SWITCH_OFF]);
        m_pBtnSwitch->setPressPicture (gpGlobalPixmaps[GLOBAL_BMP_SWITCH_ON]);
        m_pBtnSwitch->setGeometry(SYSTEM_TEST_LIST_WIDGET_WIDTH - 50 - m_pBtnSwitch->getPicWidth(),TOP_MARGIN,m_pBtnSwitch->getPicWidth(),m_pBtnSwitch->getPicHeight());
        m_pBtnSwitch->setStyleSheet("background-color:transparent");

        connect(m_pBtnSwitch, SIGNAL(clicked(int)), owner, SLOT(on_btn_clicked(int)));
        
        break;
    case SYSTEM_TEST_ITEM_TYPE_RADIO_AND_VALUE:
        {
            //int iLoop;
            m_lbP1Name = new QLabel(this);
            m_lbP1Name->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
            m_lbP1Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += VALUE_ITEM_WIDTH;

            xOffset += 2;
            m_lbP1Value = new QLabel(this);
            m_lbP1Value->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
            m_lbP1Value->setAlignment(Qt::AlignCenter);        
            xOffset += VALUE_ITEM_WIDTH;
            m_lbP1Value->setStyleSheet("QLabel{border-width:2;border-style:outset}");

            xOffset += 10;
            m_lbP2Name = new QLabel(this);
            m_lbP2Name->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
            m_lbP2Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += VALUE_ITEM_WIDTH;

            xOffset += 2;
            m_lbP2Value = new QLabel(this);
            m_lbP2Value->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN));
            m_lbP2Value->setAlignment(Qt::AlignCenter);        
            m_lbP2Value->setStyleSheet("QLabel{border-width:2;border-style:outset}");

            m_cmbAction = new QComboBox(this);
            m_cmbAction->addItem(tr("OFF"));
            m_cmbAction->addItem(tr("LOW"));
            m_cmbAction->addItem(tr("MID"));
            m_cmbAction->addItem(tr("HIGH"));
            m_cmbAction->setCurrentIndex(0);

            m_cmbAction->setGeometry(SYSTEM_TEST_LIST_WIDGET_WIDTH - 50 - 100,TOP_MARGIN,100,SYSTEM_TEST_LIST_WIDGET_HEIGHT - 2*TOP_MARGIN);

            connect(m_cmbAction,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combAction_change(int)));
            
        }
        break;
    }

}

void Systestlistwidgtitem::on_combAction_change(int index)
{
    emit combActionChange((m_iId << 8) | index);
}

