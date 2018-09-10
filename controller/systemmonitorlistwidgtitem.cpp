#include "systemmonitorlistwidgtitem.h"

#define LEFT_MARGIN       10
#define TOP_MARGIN        2
#define BOTTOM_MARGIN     2

#define NAME_ITEM_WIDTH   180
#define VALUE_ITEM_WIDTH  150
#define ITEM_SPACE       (20)
#define SHORT_ITEM_WIDTH  60

#define LONG_ITEM_WIDTH  400

#define MIDDLE_POSITON (SYSMONI_ITEM_WIDTH/2)

SystemMonitorListWidgtItem::SystemMonitorListWidgtItem(QWidget *parent,int iType,int iId,int iSingleIdx) :
    QWidget(parent),m_iId(iId),m_iType(iType),m_iSingleIdx(iSingleIdx)
{
    int xOffset = 0;
    m_lbName1 = NULL;
    m_lbSwitch1 = NULL;
    m_lbValue1 = NULL;
    m_lbName2 = NULL;
    m_lbSwitch2 = NULL;
    m_lbValue2 = NULL;
    m_lbValue3 = NULL;

    m_imgSwitchOn = NULL;
    m_imgSwitchOff = NULL;


    switch(m_iType)
    {
    case SYSMONI_LIST_ITEM_SINGLE_TEXT:
        xOffset = LEFT_MARGIN;
        m_lbName1 = new QLabel(this);
        m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbValue1 = new QLabel(this);
        m_lbValue1->setGeometry(QRect(xOffset, TOP_MARGIN ,LONG_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        break;
    case SYSMONI_LIST_ITEM_TRIPLE_TEXT:
        xOffset = LEFT_MARGIN;
        m_lbName1 = new QLabel(this);
        m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbValue1 = new QLabel(this);
        m_lbValue1->setGeometry(QRect(xOffset, TOP_MARGIN ,LONG_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbValue2 = new QLabel(this);
        m_lbValue2->setGeometry(QRect(xOffset, TOP_MARGIN ,LONG_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbValue3 = new QLabel(this);
        m_lbValue3->setGeometry(QRect(xOffset, TOP_MARGIN ,LONG_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue3->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        break;
        
    case SYSMONI_LIST_ITEM_SINGLE_SWITCH:

        if (0 == iSingleIdx)
        {
            m_lbName1 = new QLabel(this);
            m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += NAME_ITEM_WIDTH;
            
            xOffset += ITEM_SPACE;
            
            m_lbSwitch1 = new QLabel(this);
            m_lbSwitch1->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));

            xOffset = MIDDLE_POSITON;
        }
        else
        {
            m_lbName2 = new QLabel(this);
            m_lbName2->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbName2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += NAME_ITEM_WIDTH;
            
            xOffset += ITEM_SPACE;
            
            m_lbSwitch2 = new QLabel(this);
            m_lbSwitch2->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));

            xOffset = MIDDLE_POSITON;
        }
        break;
    case SYSMONI_LIST_ITEM_DOUBLE_SWITCH:
        m_lbName1 = new QLabel(this);
        m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbSwitch1 = new QLabel(this);
        m_lbSwitch1->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));

        xOffset = MIDDLE_POSITON;

        xOffset += LEFT_MARGIN;
        m_lbName2 = new QLabel(this);
        m_lbName2->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbSwitch2 = new QLabel(this);
        m_lbSwitch2->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        
        break;
    case SYSMONI_LIST_ITEM_SINGLE_SWITCH_VALUE:
        if (0 == iSingleIdx)
        {
            m_lbName1 = new QLabel(this);
            m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += NAME_ITEM_WIDTH;
            
            xOffset += ITEM_SPACE;
            
            m_lbSwitch1 = new QLabel(this);
            m_lbSwitch1->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            xOffset += SHORT_ITEM_WIDTH;
            xOffset += ITEM_SPACE;
        
            
            m_lbValue1 = new QLabel(this);
            m_lbValue1->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbValue1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += VALUE_ITEM_WIDTH;
            xOffset += ITEM_SPACE;
        
            xOffset = MIDDLE_POSITON;   
        }
        else
        {
            m_lbName2 = new QLabel(this);
            m_lbName2->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbName2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += NAME_ITEM_WIDTH;
            
            xOffset += ITEM_SPACE;
            
            m_lbSwitch2 = new QLabel(this);
            m_lbSwitch2->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            xOffset += SHORT_ITEM_WIDTH;
            xOffset += ITEM_SPACE;
        
            
            m_lbValue2 = new QLabel(this);
            m_lbValue2->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
            m_lbValue2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            xOffset += VALUE_ITEM_WIDTH;
            xOffset += ITEM_SPACE;
        
            xOffset = MIDDLE_POSITON;           
        }
        break;
    case SYSMONI_LIST_ITEM_DOUBLE_SWITCH_VALUE:
        m_lbName1 = new QLabel(this);
        m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbSwitch1 = new QLabel(this);
        m_lbSwitch1->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        xOffset += SHORT_ITEM_WIDTH;
        xOffset += ITEM_SPACE;

        
        m_lbValue1 = new QLabel(this);
        m_lbValue1->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_SPACE;

        xOffset = MIDDLE_POSITON;

        xOffset += LEFT_MARGIN;
        m_lbName2 = new QLabel(this);
        m_lbName2->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbSwitch2 = new QLabel(this);
        m_lbSwitch2->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        xOffset += SHORT_ITEM_WIDTH;
        xOffset += ITEM_SPACE;

        
        m_lbValue2 = new QLabel(this);
        m_lbValue2->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_SPACE;
        break;   
    case SYSMONI_LIST_ITEM_SINGLE_SWITCH_DOUBLE_VALUE:
        m_lbName1 = new QLabel(this);
        m_lbName1->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbName1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_WIDTH;
        
        xOffset += ITEM_SPACE;
        
        m_lbSwitch1 = new QLabel(this);
        m_lbSwitch1->setGeometry(QRect(xOffset, TOP_MARGIN ,SHORT_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        xOffset += SHORT_ITEM_WIDTH;
        xOffset += ITEM_SPACE;

        
        m_lbValue1 = new QLabel(this);
        m_lbValue1->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue1->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_SPACE;

        xOffset = MIDDLE_POSITON;

        xOffset += LEFT_MARGIN;

        m_lbValue2 = new QLabel(this);
        m_lbValue2->setGeometry(QRect(xOffset, TOP_MARGIN ,VALUE_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbValue2->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_SPACE;
        break;              
    }
}


