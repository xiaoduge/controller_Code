#include "sendtopage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

SendToPage::SendToPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    // creatTitle();

    initUi();

    buildTranslation();
}

void SendToPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SendToPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("SendToPage");

    setTitles(stringList);

}

void SendToPage::buildTranslation()
{
    int iLoop;

    CancelBtn->setTip(tr("Cancel"));
    SaveBtn->setTip(tr("OK"));
    lbTitle->setText(tr("send to"));

    for(iLoop = 0 ; iLoop < RECEIVER ; iLoop++)
    {
        lbName[iLoop]->setText(tr("send to"));
    }    
}

void SendToPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void SendToPage::setBackColor()
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

QColor SetColor[RECEIVER] = {
    QColor(255,255,255),
    QColor(255,255,255),
    QColor(255,255,255),
};

QString img_name[RECEIVER] = {
    QString(":/pic/computer.png"),
    QString(":/pic/Email.png"),
    QString(":/pic/Udisk.png"),
};

void SendToPage::initUi()
{
    setBackColor();

    QPalette pal;

    QString strQss4Chk = m_wndMain->getQss4Chk();

    lbTitle = new QLabel(m_widget);
    lbTitle->setGeometry(QRect(20,56,100,25));
    lbTitle->setStyleSheet(" font-size:18pt;color:#15191d;font-family:Arial;QFont::Bold;border:0px solid #dfe7ed");

    //m_widget->setStyleSheet("background-color:#ffffff;border:1px solid #dfe7ed");

    QPixmap imgType;

    for(int iLoop = 0 ; iLoop < RECEIVER ; iLoop++)
    {
        m_pSetWidget[iLoop] = new QWidget(m_widget);

        pal.setBrush(m_pSetWidget[iLoop]->backgroundRole(),QBrush(QColor(255,255,255)));

        m_pSetWidget[iLoop]->setAutoFillBackground(true);
        m_pSetWidget[iLoop]->setPalette(pal);
        m_pSetWidget[iLoop]->setGeometry(QRect(20,110 + 40 * iLoop,280,40));

        if(iLoop % 2)
        {
            m_pSetWidget[iLoop]->setStyleSheet("background-color:#ffffff;border:1px solid #dfe7ed");
        }
        else
        {
            m_pSetWidget[iLoop]->setStyleSheet("background-color:#f4f9fa;border:1px solid #dfe7ed");
        }

        imgType.load(img_name[iLoop]);

        lbPic[iLoop] = new QLabel(m_pSetWidget[iLoop]);
        lbPic[iLoop]->setGeometry(QRect(10,10,32,28));
        lbPic[iLoop]->setPixmap(imgType);
        lbPic[iLoop]->setStyleSheet("border:0px");

        lbName[iLoop] = new QLabel(m_pSetWidget[iLoop]);
        lbName[iLoop]->setGeometry(QRect(52,10,100,28));
        lbName[iLoop]->setStyleSheet(" font-size:18pt;color:#15191d;font-family:Arial;border:0px");

        m_chkSwitchs[iLoop] = new QCheckBox(m_pSetWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(230 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));
    }

    CancelBtn = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SENDPAGE_BTN_CANCEL);
    CancelBtn->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_NORMAL]);
    CancelBtn->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_ACTIVE]);
    CancelBtn->setGeometry(QRect(60 , 285 ,CancelBtn->width(),CancelBtn->height()));
    CancelBtn->setStyleSheet("background-color:transparent");

    connect(CancelBtn, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    SaveBtn   = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SNEDPAGE_BTN_OK);
    SaveBtn->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_NORMAL]);
    SaveBtn->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_ACTIVE]);
    SaveBtn->setGeometry(QRect(182 , 285 ,SaveBtn->width(),SaveBtn->height()));
    SaveBtn->setStyleSheet("background-color:transparent");

    connect(SaveBtn, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
}

void SendToPage::on_btn_clicked(int)
{
    show(false);

}




void SendToPage::on_checkBox_changeState(int state)
{
    //int iLoop;
    
    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }
    
    //int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;

}

