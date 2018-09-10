#include "sysmsgpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#define WIDGET_BACK_WIDTH  (550)
#define WIDGET_BACK_HEIGHT (350)

#define SYSMS_ITEM_HEIGHT (40)

QRect   QrectSysAry[SYSMS_PAGE_TE_NUM] = {
        QRect(150,  50,                       350 ,                50) ,
        QRect(100, 130 + 0*SYSMS_ITEM_HEIGHT, 350 , SYSMS_ITEM_HEIGHT) ,
        QRect(100, 130 + 1*SYSMS_ITEM_HEIGHT, 350 , SYSMS_ITEM_HEIGHT) ,
        QRect(100, 130 + 2*SYSMS_ITEM_HEIGHT, 350 , SYSMS_ITEM_HEIGHT) ,
        QRect(100, 130 + 3*SYSMS_ITEM_HEIGHT, 350 , SYSMS_ITEM_HEIGHT) ,
        QRect(100, 130 + 4*SYSMS_ITEM_HEIGHT, 350 , SYSMS_ITEM_HEIGHT) ,
    };


SysMsgPage::SysMsgPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();

    initUi();
    
    buildTranslation();

}

void SysMsgPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SysMsgPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Information");

    setTitles(stringList);

}

void SysMsgPage::buildTranslation()
{
   switch(gGlobalParam.iMachineType)
   {
   case MACHINE_L_Genie:
        m_TitleMsg = tr("SuperGenie G 250");
        break;
   case MACHINE_L_UP:
        m_TitleMsg = tr("SuperGenie U 300");
        break;
   case MACHINE_L_EDI_LOOP:
        m_TitleMsg = tr("SuperGenie E 250");
        break;
   case MACHINE_L_RO_LOOP:
        m_TitleMsg = tr("SuperGenie R 300");
        break;
   case MACHINE_Genie:
       m_TitleMsg = tr("Genie G 15");
        break;
   case MACHINE_UP:
       m_TitleMsg = tr("Genie U");
        break;
   case MACHINE_EDI:
       m_TitleMsg = tr("Genie E 15");
        break;
   case MACHINE_RO:
       m_TitleMsg = tr("Genie R 20");
        break;
   case MACHINE_PURIST:
       m_TitleMsg = tr("PURIST");
        break;
	case MACHINE_ADAPT:
       m_TitleMsg = tr("adept");
        break;
   }
   
   m_astrMsgName[0] = tr("Catalog No.")       + " :  %1";
   m_astrMsgName[1] = tr("Serial No.")        + " :  %1";
   m_astrMsgName[2] = tr("Production Date")   + " :  %1";
   m_astrMsgName[3] = tr("Installation Date") + " :  %1";
   m_astrMsgName[4] = tr("Software Version")  + " :  %1";
   
   setTitleMsg();
   
   setNameMsg(SYSMS_PAGE_TE_1,"unknow");
   setNameMsg(SYSMS_PAGE_TE_2,"S8PW070801");
   setNameMsg(SYSMS_PAGE_TE_3,"2018-07-19");
   setNameMsg(SYSMS_PAGE_TE_4,"2018-07-20");
   setNameMsg(SYSMS_PAGE_TE_5,"1.0");
   
}

void SysMsgPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}


void SysMsgPage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void SysMsgPage::setTitleMsg()
{
    m_pMsgTextEdit[SYSMS_PAGE_TE_TITLE]->setText("");

    QTextCursor cursor = m_pMsgTextEdit[SYSMS_PAGE_TE_TITLE]->textCursor();

    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);

    m_pMsgTextEdit[SYSMS_PAGE_TE_TITLE]->setTextCursor(cursor);

    cursor.insertText(m_TitleMsg);
}

void SysMsgPage::setNameMsg(int iIdx,QString strInfo)
{
    QString tmpMsg , strName , strValue;
    
    m_pMsgTextEdit[iIdx]->setText("");

    QTextCursor cursor = m_pMsgTextEdit[iIdx]->textCursor();
    
    tmpMsg = m_astrMsgName[iIdx-1].arg(strInfo);

    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);

    m_pMsgTextEdit[iIdx]->setTextCursor(cursor);

    if(-1 == tmpMsg.indexOf(":"))
    {
        strName = tmpMsg;

        strValue = "";
    }
    else
    {
        strName  = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);
        strValue = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
    }

    cursor.setCharFormat(charFormatName);
    cursor.insertText(strName); 
    
    cursor.setCharFormat(charFormatValue);
    cursor.insertText(strValue); 

}

void SysMsgPage::initUi()
{
    int iLoop;

    QFont fontName("Arial",24,QFont::Bold);

    QFont fontValue("Arial",24,0);

    setBackColor();

    charFormatName.setFont(fontName);
    
    charFormatValue.setFont(fontValue);

    
    m_pWidget = new QWidget(m_widget);

    QPalette pal(m_pWidget->palette());
    
    pal.setColor(QPalette::Background, QColor(255, 255, 255));
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal);
    m_pWidget->setGeometry(QRect(124,124,550,350));

    for(iLoop = 0 ; iLoop < SYSMS_PAGE_TE_NUM; iLoop++)
    {
        m_pMsgTextEdit[iLoop] = new QTextEdit(m_pWidget);

        //m_pMsgTextEdit[iLoop]->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));

        m_pMsgTextEdit[iLoop]->setReadOnly(true);

        m_pMsgTextEdit[iLoop]->setStyleSheet("border: none;");

        m_pMsgTextEdit[iLoop]->setGeometry(QrectSysAry[iLoop]);
    }

    m_pMsgTextEdit[SYSMS_PAGE_TE_TITLE]->setAlignment(Qt::AlignCenter);
    m_pMsgTextEdit[SYSMS_PAGE_TE_TITLE]->setStyleSheet(" font-size:32pt;color:#16181e;font-family:Arial;QFont::Bold;border:0px");

   for(iLoop = SYSMS_PAGE_TE_1 ; iLoop < SYSMS_PAGE_TE_NUM; iLoop++)
   {
       m_pMsgTextEdit[iLoop]->setAlignment(Qt::AlignLeft);
       m_pMsgTextEdit[iLoop]->setStyleSheet("color:#667587;border:0px");
   }


}
