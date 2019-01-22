#include "parameterlistwidgtitem.h"


#define LEFT_MARGIN       10
#define TOP_MARGIN        5
#define BOTTOM_MARGIN     5
#define NAME_ITEM_WIDTH   220   //160
#define VALUE_ITEM_WIDTH  120
#define ITEM_SPACE       (20)
#define ITEM_UINT_SPACE  (5)

#define NAME_ITEM_SHORT_LENGTH (60)
#define ITEM_MAX_CHARS (9)


Parameterlistwidgtitem::Parameterlistwidgtitem(QWidget *parent,int iType,int iId) :
    QWidget(parent)
{
    int xOffset = LEFT_MARGIN;

    m_lbName      = NULL;
    m_editP1Value = NULL;
    m_editP2Value = NULL;
    m_editP3Value = NULL;
    m_lbP1Name    = NULL;
    m_lbP2Name    = NULL;
    m_lbP3Name    = NULL;

    m_iId   = iId;   
    m_iType = iType;
    
    m_lbName = new QLabel(this);
    m_lbName->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += NAME_ITEM_WIDTH;

    m_lbP1Name = new QLabel(this);
    m_lbP1Name->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_SHORT_LENGTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_lbP1Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    xOffset += NAME_ITEM_SHORT_LENGTH;

    m_editP1Value = new QLineEdit(this);
    m_editP1Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
    m_editP1Value->setAlignment(Qt::AlignCenter);
    m_editP1Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
    m_editP1Value->setMaxLength (ITEM_MAX_CHARS);
    xOffset += VALUE_ITEM_WIDTH;

    xOffset += ITEM_SPACE;

    switch(iType)
    {
    case PARAMETER_CALIBRATION_FORMAT0:

        m_lbP2Name = new QLabel(this);
        m_lbP2Name->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_SHORT_LENGTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbP2Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_SHORT_LENGTH;
        m_lbP2Name->hide(); //
        
        m_editP2Value = new QLineEdit(this);
        m_editP2Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_editP2Value->setAlignment(Qt::AlignCenter);
        m_editP2Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
        m_editP2Value->setMaxLength (ITEM_MAX_CHARS);
        xOffset += VALUE_ITEM_WIDTH;
        m_editP2Value->hide(); //

        xOffset += ITEM_SPACE ;
        m_editP3Value = new QLineEdit(this);
        m_editP3Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_editP3Value->setAlignment(Qt::AlignCenter);
        m_editP3Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
        m_editP3Value->setMaxLength (ITEM_MAX_CHARS);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_UINT_SPACE ;
        m_lbP3Name = new QLabel(this);
        m_lbP3Name->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_SHORT_LENGTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbP3Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_SHORT_LENGTH;
        m_editP3Value->hide();//
        m_lbP3Name->hide();//
        
        break;
    case PARAMETER_CALIBRATION_FORMAT1:

        xOffset += NAME_ITEM_SHORT_LENGTH + VALUE_ITEM_WIDTH + ITEM_SPACE ;
        m_editP3Value = new QLineEdit(this);
        m_editP3Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_editP3Value->setAlignment(Qt::AlignCenter);
        m_editP3Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
        m_editP3Value->setMaxLength (ITEM_MAX_CHARS);
        xOffset += VALUE_ITEM_WIDTH;
        xOffset += ITEM_UINT_SPACE ;
        m_lbP3Name = new QLabel(this);
        m_lbP3Name->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_SHORT_LENGTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbP3Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_SHORT_LENGTH;
        m_lbP3Name->hide(); //
        m_editP3Value->hide();//
        break;
    case PARAMETER_CALIBRATION_FORMAT2:
        m_lbP2Name = new QLabel(this);
        m_lbP2Name->setGeometry(QRect(xOffset, TOP_MARGIN ,NAME_ITEM_SHORT_LENGTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_lbP2Name->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        xOffset += NAME_ITEM_SHORT_LENGTH;
        m_lbP2Name->hide();

        m_editP2Value = new QLineEdit(this);
        m_editP2Value->setGeometry(QRect(xOffset, TOP_MARGIN  ,VALUE_ITEM_WIDTH , PC_LIST_WIDGET_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN)));
        m_editP2Value->setAlignment(Qt::AlignCenter);
        m_editP2Value->setStyleSheet("QLineEdit{border-width:2;border-style:outset;}");
        m_editP2Value->setMaxLength (ITEM_MAX_CHARS);
        xOffset += VALUE_ITEM_WIDTH;
        m_editP2Value->hide();
        break;
    }

}

