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

#ifndef SYSZUXPINYIN_H
#define SYSZUXPINYIN_H

#ifndef QT_WEBKIT

#include <QFile>
#include <QWSInputMethod>
#include <QMouseEvent> 
#include "ui_syszuxpinyin.h"

enum
{
    SHIFT_STATE_LOW_CASE = 0,
    SHIFT_STATE_HIGH_CASE,
};

enum
{
    IME_STATE_ENG = 0,
    IME_STATE_HZ,
    IME_STATE_DIGI,
};

class SyszuxPingyinCtlInfo : public QObject
{
    Q_OBJECT
public:
    SyszuxPingyinCtlInfo(){widget = NULL;};
    int      id;
    QRect    rect;
    QWidget *widget;
};

class SyszuxPinyin : public QDialog ,public Ui::IMEall
{
    Q_OBJECT
public:
    SyszuxPinyin(QWSInputMethod* im);
    ~SyszuxPinyin();
    void changePage(int index);
    void matching(QString gemfield);
    void initGb();
    void selectHanziPre();
    void selectHanziNext();
    void changeInputMethod();
    void changeLowerUpper();
    void clearString();
    void affirmString();
    void deleteString();
    void CustomLayout();
public slots:
    void buttonClickResponse(int gemfield);
signals:
    void sendPinyin(QString gemfield);

protected:  
    void mousePressEvent(QMouseEvent *e);  
    void mouseMoveEvent(QMouseEvent *e);  
    void mouseReleaseEvent(QMouseEvent *e);  
private:  
    QPoint last;      
private:
    QFile pinyin_file;
    QRegExp regExp;
    QButtonGroup *button_group;
    QMultiMap<QString,QString> pinyin_map;
    QList<QString> pinyin_list;
    QVector<QPushButton*> button_vector;
    QVector<SyszuxPingyinCtlInfo*> ctl_geo_ec_vector; // for english / chinese layout
    QVector<SyszuxPingyinCtlInfo*> ctl_geo_digi_vector; // for digi layout
    QVector<SyszuxPingyinCtlInfo*> ctl_geo_chn_vector; // for english / chinese layout
    QVector<SyszuxPingyinCtlInfo*> ctl_geo_eng_vector; // for english / chinese layout
    
    int input_method,lower_upper,page_count;
    int digikbd_width,digikbd_height;
    int engkbd_width,engkbd_height;
    int chnkbd_width,chnkbd_height;
	
    QRect desktop;

    int shift_state;
    
};
#endif

#endif

