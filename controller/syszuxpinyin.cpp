/***************************************************************************
**
**  <SYSZUXpinyin 1.0 , a chinese input method based on Qt for Embedded linux>
**  Copyright (C) <2010> <Gemfield> <gemfield@civilnet.cn>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License version 3 as published
**  by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**  If you have questions regarding the use of this file, please contact
**  Gemfield at gemfield@civilnet.cn or post your questions at
**  http://civilnet.cn/syszux/bbs
**
****************************************************************************/
#ifndef QT_WEBKIT

#include <QtGui>
#include "syszuxpinyin.h"
#include "syszuxim.h"
QString syszux_lower_letter[52]={"1","2","3","4","5","6","7","8","9","0","-","=","backspace","q","w","e","r","t","y","u","i",
                            "o","p","[","]","\\","a","s","d","f","g","h","j","k","l",";","\'","enter","z","x","c","v",
                            "b","n","m",",",".","/","sh"," "};
QString syszux_upper_letter[52]={"!","@","#","$","%","^","&&","*","(",")","_","+","backspace","Q","W","E","R","T","Y","U","I",
                            "O","P","{","}","|","A","S","D","F","G","H","J","K","L",":","\"","enter","Z","X","C","V",
                            "B","N","M","<",">","?","SH"," "};
SyszuxPinyin::SyszuxPinyin(QWSInputMethod* im) :QDialog(0,Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),button_group(new QButtonGroup(this)),input_method(IME_STATE_DIGI),lower_upper(0),page_count(0)
{
    int ret=0;
    setupUi(this);

    shift_state = SHIFT_STATE_LOW_CASE;

    desktop = QApplication::desktop()->geometry();

    //resize(800,480);
    initGb();

    CustomLayout();

    pinyin_file.setFileName(":/syszux/syszuxpinyin_utf8");
    //pinyin_file.setFileName(":/syszux/syszuxpinyin");

    if( !pinyin_file.open(QIODevice::ReadOnly) )
        QMessageBox::warning(0,tr("syszuxpinyin"),tr("can't load"));

    QTextStream in(&pinyin_file);
    in.setCodec("UTF-8"); 

    regExp.setCaseSensitivity(Qt::CaseSensitive);
    regExp.setPattern(QString("([a-z]+)"));

    while(!in.atEnd())
    {
        //QByteArray data = in.readLine();
        QString strData = in.readLine();
        ret = regExp.indexIn(strData,0,QRegExp::CaretAtZero);
        pinyin_map.insert(regExp.cap(1),strData.left(ret));
    }
    connect(this,SIGNAL(sendPinyin(QString)),im,SLOT(confirmString(QString)));
}
SyszuxPinyin::~SyszuxPinyin()
{
}
void SyszuxPinyin::initGb()
{
    SyszuxPingyinCtlInfo *pCtrlInfo;
    QPushButton *pushButton = new QPushButton(this);
    pushButton->hide();
    pushButton = pushButton_hanzi_left;
    for(int i = 1;i < 67;i++)
    {
        // shzn add begin
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = i;
        pCtrlInfo->rect = pushButton->geometry();
        ctl_geo_ec_vector.push_back(pCtrlInfo);
        pCtrlInfo->widget = pushButton;
        // shzn add end
        
        button_vector.push_back(pushButton);
        button_group->addButton(pushButton,i);
        pushButton = qobject_cast<QPushButton *>(pushButton->nextInFocusChain());
    }

    // shzn add begin
    pCtrlInfo = new SyszuxPingyinCtlInfo(); 
    pCtrlInfo->id = 68;
    pCtrlInfo->rect = lineEdit_pinyin->geometry();
    pCtrlInfo->widget = lineEdit_pinyin;
    ctl_geo_ec_vector.push_back(pCtrlInfo);

    // for digi keyboard
    {
        int voffset,hoffset;
        int hmargin = 5,vmargin = 5,id;

        voffset = 5;
        hoffset = 5;

        /* digi 1 */
        id = 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        digikbd_width = hoffset;

        hoffset = 5;
        voffset += pCtrlInfo->rect.height() + vmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
            
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        hoffset = 5;
        voffset += pCtrlInfo->rect.height() + vmargin;

        /* dot */
        id = 46 + 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* backspace */
        id = 12 + 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* ime selector */
        id = 61;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;    

        /* affirm */
        id = 62;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_digi_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;  
        

        voffset += pCtrlInfo->rect.height() + vmargin;
        digikbd_height = voffset;

        
    }
    /* for ENG keyboard */
    {
        int voffset,hoffset;
        int hmargin = 2,vmargin = 2,id;

        voffset = 5;
        hoffset = 5;
        /* row 0 */
        /* digi 1 */
        id = 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        engkbd_width = hoffset;

        /* row 1*/
        /* q */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5; 
        id = 24;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        /* row 2 */
        id = 37;
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5 + pCtrlInfo->rect.width() / 2;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* row 3*/
        /* shift */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5; 
        id = 59;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        id = 49;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   

        /* backspace */
        id = 12 + 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* row 4*/
        /* shift */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5;        

        /* ime selector */
        id = 61;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;    

        /* affirm */
        id = 62;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_eng_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;  

        voffset += pCtrlInfo->rect.height() + vmargin;

        engkbd_height = voffset;
        
    }

    /* for CHN keyboard */
    {
        int voffset,hoffset;
        int hmargin = 2,vmargin = 2,id;

        voffset = 5;
        hoffset = 5;

        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = 68;
        pCtrlInfo->rect = lineEdit_pinyin->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = lineEdit_pinyin;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
    
        voffset += pCtrlInfo->rect.height() + vmargin;
        
        /* row 0 */
        /* digi 1 */
        id = 1;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        
        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        chnkbd_width = hoffset;

        /* row 1*/
        /* q */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5; 
        id = 24;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        /* row 2 */
        id = 37;
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5 + pCtrlInfo->rect.width() / 2;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 
    
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* row 3*/
        /* shift */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5; 
        id = 59;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin; 

        id = 49;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       

        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;       
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   
        
        id ++;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;   

        /* backspace */
        id = 12 + 11;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;

        /* row 4*/
        /* shift */
        voffset += pCtrlInfo->rect.height() + vmargin;
        hoffset = 5;        

        /* ime selector */
        id = 61;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;    

        /* affirm */
        id = 62;
        pushButton = button_vector.at(id-1);
        pCtrlInfo = new SyszuxPingyinCtlInfo(); 
        pCtrlInfo->id = id;
        pCtrlInfo->rect = pushButton->geometry();
        pCtrlInfo->rect.moveTo(hoffset,voffset);
        pCtrlInfo->widget = pushButton;
        ctl_geo_chn_vector.push_back(pCtrlInfo);
        hoffset += pCtrlInfo->rect.width() + hmargin;  

        voffset += pCtrlInfo->rect.height() + vmargin;

        chnkbd_height = voffset;
        
    }
    
    connect(button_group,SIGNAL(buttonClicked(int)),SLOT(buttonClickResponse(int)));
}
void SyszuxPinyin::buttonClickResponse(int gemfield)
{
    QKeyEvent *event = NULL;

    if(gemfield==1)
    {
        selectHanziPre();
        return;
    }
    else if(gemfield == 10)
    {
        selectHanziNext();
        return;
    }
    else if(gemfield < 10)
    {
        //lineEdit_window->insert(button_vector.at(gemfield-1)->text());
        emit sendPinyin(button_vector.at(gemfield-1)->text());
        
        lineEdit_pinyin->clear();
        clearString();
        return;
    }
    else if(gemfield==23)
    {
        deleteString();
        return;
    }
    else if(gemfield==59)
    {
         changeLowerUpper();
         return;
    }
    else if(gemfield > 10 && gemfield <= 60)
    {
        if (1 == input_method)
        {
            if(lower_upper)
                event = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier,syszux_upper_letter[gemfield-11]);
            else
                event = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier,syszux_lower_letter[gemfield-11]);
        }
        else
        {
            if(lower_upper)
                emit sendPinyin(syszux_upper_letter[gemfield-11]);
            else
                emit sendPinyin(syszux_lower_letter[gemfield-11]);

            return ;
        
        }
    }
    else if(gemfield==61)
    {
        changeInputMethod();
        return;
    }
    else if(gemfield==62)
    {
        affirmString();
        return;
    }
    else if(gemfield>62)
    {
        if (1 == input_method)
        {
            switch(gemfield)
            {
            case 63:
                event=new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
                break;
            case 64:
                event=new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
                break;
            case 65:
                event=new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
                break;
            case 66:
                event=new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
                break;
            }
        }
    }
    if(1 == input_method)
    {
        if (event)
        {
            lineEdit_pinyin->setFocus();
            QApplication::sendEvent(focusWidget(),event);
            matching(lineEdit_pinyin->text());
            return ;
        }
    }
    else
    {
        //lineEdit_window->setFocus();
        //QApplication::sendEvent(focusWidget(),event);
    }

    if (event)
    {
        delete event;
    }
}
void SyszuxPinyin::matching(QString gemfield)
{
    pinyin_list = pinyin_map.values(gemfield);
    changePage(0);
    page_count=0;
}


void SyszuxPinyin::changePage(int index)
{
    int count = pinyin_list.size();
    int i = index*8,j=0;
    while(++j != 9 )
    {
        ++i;
        QString strText = pinyin_list.value(count-(i));

        //QString strTest = "确定";
        button_vector.at(j)->setText(strText);

        //qDebug()<<strText;
    }
    if(index==0)
        pushButton_hanzi_left->setEnabled(false);
    else
        pushButton_hanzi_left->setEnabled(true);
    if(pinyin_list.size()>(index*8+8))
        pushButton_hanzi_right->setEnabled(true);
    else
        pushButton_hanzi_right->setEnabled(false);
}

void SyszuxPinyin::selectHanziPre()
{
    changePage(--page_count);
}

void SyszuxPinyin::selectHanziNext()
{
    changePage(++page_count);
}
void SyszuxPinyin::clearString()
{
    int i=0;
    while(++i!=9)
        button_vector.at(i)->setText("");
}

void SyszuxPinyin::CustomLayout()
{
    int iLoop;

    switch(input_method)
    {
    case 0:
        for (iLoop = 0; iLoop < ctl_geo_ec_vector.size(); iLoop++)
        {
            SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_ec_vector.at(iLoop);
            QWidget *pWgt = ctlInfo->widget;
            pWgt->hide();
        }
        {
            for (iLoop = 0; iLoop < ctl_geo_eng_vector.size(); iLoop++)
            {
                SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_eng_vector.at(iLoop);
                QWidget *pWgt = ctlInfo->widget;
                pWgt->setGeometry(ctlInfo->rect);
                pWgt->show();
            }
        }       
        resize(engkbd_width,engkbd_height);
        break;
    case 1:
        for (iLoop = 0; iLoop < ctl_geo_ec_vector.size(); iLoop++)
        {
            SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_ec_vector.at(iLoop);
            QWidget *pWgt = ctlInfo->widget;
            pWgt->hide();
        }
        
        {
            for (iLoop = 0; iLoop < ctl_geo_chn_vector.size(); iLoop++)
            {
                SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_chn_vector.at(iLoop);
                QWidget *pWgt = ctlInfo->widget;
                pWgt->setGeometry(ctlInfo->rect);
                pWgt->show();
            }
        }
        resize(chnkbd_width,chnkbd_height);
        break;
    case 2:
        for (iLoop = 0; iLoop < ctl_geo_ec_vector.size(); iLoop++)
        {
            SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_ec_vector.at(iLoop);
            QWidget *pWgt = ctlInfo->widget;
            pWgt->hide();
        }

        for (iLoop = 0; iLoop < ctl_geo_digi_vector.size(); iLoop++)
        {
            SyszuxPingyinCtlInfo* ctlInfo = ctl_geo_digi_vector.at(iLoop);
            QWidget *pWgt = ctlInfo->widget;

            if (pWgt)
            {
                pWgt->setGeometry(ctlInfo->rect);
                pWgt->show();
            }
        }
        resize(digikbd_width,digikbd_height);
        break;
    }
}

void SyszuxPinyin::changeInputMethod()
{
   if(SHIFT_STATE_HIGH_CASE == shift_state)
   {
       return;
   }

   lineEdit_pinyin->clear();

   
   if(IME_STATE_ENG == input_method)
   {
      input_method = IME_STATE_HZ;
      
      CustomLayout();
   }
   else if (IME_STATE_HZ == input_method)
   {
      input_method = IME_STATE_DIGI;

      /* recal layout */
      CustomLayout();
   }
   else 
   {
      input_method = IME_STATE_ENG;
      
      /* recal layout */
      CustomLayout();
   }

}
void SyszuxPinyin::changeLowerUpper()
{
    switch(input_method)
    {
    case IME_STATE_ENG:
        if(SHIFT_STATE_LOW_CASE == shift_state)
        {
            lower_upper = 1;
            shift_state = SHIFT_STATE_HIGH_CASE;
            //pushButton_shift->setText("SH");
            int i=9;
            while(++i != 59)
            {
                if (i-10 == 12) // BS 
                {
                   continue;
                }
                if (i-10 == 37) // ENTER
                {
                   continue;
                }
                if (i-10 == 48) //  SHIFT
                {
                   continue;
                }
            
                button_vector.at(i)->setText(syszux_upper_letter[i-10]);
            }
        }
        else
        {
            lower_upper=0;
            //pushButton_shift->setText("sh");
            shift_state = SHIFT_STATE_LOW_CASE;
            
            int i=9;
            while(++i!=59)
            {
                if (i-10 == 12)
                {
                   continue;
                }
                if (i-10 == 37)
                {
                   continue;
                }
                
                if (i-10 == 48) //  SHIFT
                {
                   continue;
                }
                
                button_vector.at(i)->setText(syszux_lower_letter[i-10]);
            }
        }       
        break;
    default:
        break;
    }

}
void SyszuxPinyin::deleteString()
{
    QKeyEvent *event = NULL;

    if(IME_STATE_HZ == input_method)
    {
        if (!lineEdit_pinyin->text().isEmpty())
        {
            event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
            lineEdit_pinyin->setFocus();
            QApplication::sendEvent(focusWidget(),event);
            matching(lineEdit_pinyin->text());

            return ;
        }
        else
        {
        }
    }
    else
    {
        //lineEdit_window->setFocus();
        //QApplication::sendEvent(focusWidget(),event);
    }
#ifndef QT_WEBKIT
    QWSServer::sendKeyEvent(0x01000003,Qt::Key_Backspace,Qt::NoModifier,true,false); 
#endif
}
void SyszuxPinyin::affirmString()
{
    //emit sendPinyin(lineEdit_window->text());
    this->hide();
}


void SyszuxPinyin::mousePressEvent(QMouseEvent *e)  
{  
    last = e->globalPos();  
}  
void SyszuxPinyin::mouseMoveEvent(QMouseEvent *e)  
{  
    QRect rect = geometry(); 

    if (0 != e->globalX()
        || 0 != e->globalY())
    {
        int dx = e->globalX() - last.x();  
        int dy = e->globalY() - last.y();  
        int tx;
        int ty;
        last = e->globalPos();  
        
        //tx = x()+(input_method == 2 ? dx : 0);
        tx = x() + dx;
        ty = y() + dy;
        
        if (tx + rect.width() > desktop.width())
        {
            tx = desktop.width() - rect.width();
        }
        else if (tx < 0)
        {
            tx = 0;
        }
        
        if (ty + rect.height() > desktop.height())
        {
            ty = desktop.height() - rect.height();
        }   
        else if (ty < 0)
        {
            ty = 0;
        }
        move(tx, ty);   // only vertical
    }
    
}  
void SyszuxPinyin::mouseReleaseEvent(QMouseEvent *e)  
{  

}  
#endif

