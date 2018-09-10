#include "keyboard.h"


KeyBoard::KeyBoard(QWidget *parent) :
    QWidget(parent)
{
    //this->resize(120,160);
    setFixedSize(120, 160);
    
    Qt::WindowFlags flags= this->windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    init();
    setName_sn();
    setConnect();
    setPlace();
    this->setFocus();
    m_spellEdit->setReadOnly(true);
}

KeyBoard::~KeyBoard()
{
    delete m_spellEdit;

    delete m_leftBtn;
    delete m_rightBtn;

    delete m_ziBtn0;
    delete m_ziBtn1;
    delete m_ziBtn2;
    delete m_ziBtn3;
    delete m_ziBtn4;
    delete m_ziBtn5;
    delete m_ziBtn6;
    delete m_ziBtn7;

    delete m_zinum0;
    delete m_zinum1;
    delete m_zinum2;
    delete m_zinum3;
    delete m_zinum4;
    delete m_zinum5;
    delete m_zinum6;
    delete m_zinum7;

    delete m_QBtn;
    delete m_WBtn;
    delete m_EBtn;
    delete m_RBtn;
    delete m_TBtn;
    delete m_YBtn;
    delete m_UBtn;
    delete m_IBtn;
    delete m_OBtn;
    delete m_PBtn;
    delete m_ABtn;
    delete m_SBtn;
    delete m_DBtn;
    delete m_FBtn;
    delete m_GBtn;
    delete m_HBtn;
    delete m_JBtn;
    delete m_KBtn;
    delete m_LBtn;
    delete m_ZBtn;
    delete m_XBtn;
    delete m_CBtn;
    delete m_VBtn;
    delete m_BBtn;
    delete m_NBtn;
    delete m_MBtn;

    delete m_capsBtn;
    delete m_deleteBtn;
    delete m_signBtn;
    delete m_changeBtn;
    delete m_spaceBtn;
    delete m_sureBtn;
}

void KeyBoard::init()
{
    /*m_file = new QFile("ziku.txt");
    m_in   = new QTextStream(m_file);
    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "can't open this ziku.txt!!";
        return;
    }*/
    m_spellEdit = new QLineEdit();
    m_leftBtn   = new QToolButton();
    m_rightBtn  = new QToolButton();

    m_ziBtn0 = new QToolButton();
    m_ziBtn1 = new QToolButton();
    m_ziBtn2 = new QToolButton();
    m_ziBtn3 = new QToolButton();
    m_ziBtn4 = new QToolButton();
    m_ziBtn5 = new QToolButton();
    m_ziBtn6 = new QToolButton();
    m_ziBtn7 = new QToolButton();

    m_zinum0 = new QLabel();
    m_zinum1 = new QLabel();
    m_zinum2 = new QLabel();
    m_zinum3 = new QLabel();
    m_zinum4 = new QLabel();
    m_zinum5 = new QLabel();
    m_zinum6 = new QLabel();
    m_zinum7 = new QLabel();

    m_QBtn = new QToolButton();
    m_WBtn = new QToolButton();
    m_EBtn = new QToolButton();
    m_RBtn = new QToolButton();
    m_TBtn = new QToolButton();
    m_YBtn = new QToolButton();
    m_UBtn = new QToolButton();
    m_IBtn = new QToolButton();
    m_OBtn = new QToolButton();
    m_PBtn = new QToolButton();
    m_ABtn = new QToolButton();
    m_SBtn = new QToolButton();
    m_DBtn = new QToolButton();
    m_FBtn = new QToolButton();
    m_GBtn = new QToolButton();
    m_HBtn = new QToolButton();
    m_JBtn = new QToolButton();
    m_KBtn = new QToolButton();
    m_LBtn = new QToolButton();
    m_ZBtn = new QToolButton();
    m_XBtn = new QToolButton();
    m_CBtn = new QToolButton();
    m_VBtn = new QToolButton();
    m_BBtn = new QToolButton();
    m_NBtn = new QToolButton();
    m_MBtn = new QToolButton();

    m_capsBtn   = new QToolButton();
    m_deleteBtn = new QToolButton();
    m_signBtn   = new QLabel();//new QToolButton(this)
    m_changeBtn = new QToolButton();
    m_spaceBtn  = new QToolButton();
    m_sureBtn   = new QToolButton();

    mainLayout  = new QGridLayout(this);//


    m_leftBtn ->setText("<");
    m_rightBtn ->setText(">");
    m_capsBtn ->setText("CapsLock");
    m_deleteBtn ->setText("<--");
    m_signBtn ->setText("  */123");
    m_changeBtn ->setText("中/英/数");
    m_spaceBtn ->setText("Blank Space");
    m_sureBtn ->setText("Exit");

    m_zinum0->setText("1");
    m_zinum1->setText("2");
    m_zinum2->setText("3");
    m_zinum3->setText("4");
    m_zinum4->setText("5");
    m_zinum5->setText("6");
    m_zinum6->setText("7");
    m_zinum7->setText("8");

    m_stateFlag = 2;
    m_capsFlag = true;
    m_storeCount = 0;
    m_selectFlag = 1;
}
void KeyBoard::setName_EN()
{
    m_QBtn ->setText("Q");
    m_WBtn ->setText("W");
    m_EBtn ->setText("E");
    m_RBtn ->setText("R");
    m_TBtn ->setText("T");
    m_YBtn ->setText("Y");
    m_UBtn ->setText("U");
    m_IBtn ->setText("I");
    m_OBtn ->setText("O");
    m_PBtn ->setText("P");
    m_ABtn ->setText("A");
    m_SBtn ->setText("S");
    m_DBtn ->setText("D");
    m_FBtn ->setText("F");
    m_GBtn ->setText("G");
    m_HBtn ->setText("H");
    m_JBtn ->setText("J");
    m_KBtn ->setText("K");
    m_LBtn ->setText("L");
    m_ZBtn ->setText("Z");
    m_XBtn ->setText("X");
    m_CBtn ->setText("C");
    m_VBtn ->setText("V");
    m_BBtn ->setText("B");
    m_NBtn ->setText("N");
    m_MBtn ->setText("M");
}
void KeyBoard::setName_en()
{
    m_QBtn ->setText("q");
    m_WBtn ->setText("w");
    m_EBtn ->setText("e");
    m_RBtn ->setText("r");
    m_TBtn ->setText("t");
    m_YBtn ->setText("y");
    m_UBtn ->setText("u");
    m_IBtn ->setText("i");
    m_OBtn ->setText("o");
    m_PBtn ->setText("p");
    m_ABtn ->setText("a");
    m_SBtn ->setText("s");
    m_DBtn ->setText("d");
    m_FBtn ->setText("f");
    m_GBtn ->setText("g");
    m_HBtn ->setText("h");
    m_JBtn ->setText("j");
    m_KBtn ->setText("k");
    m_LBtn ->setText("l");
    m_ZBtn ->setText("z");
    m_XBtn ->setText("x");
    m_CBtn ->setText("c");
    m_VBtn ->setText("v");
    m_BBtn ->setText("b");
    m_NBtn ->setText("n");
    m_MBtn ->setText("m");
}
void KeyBoard::setName_sn()
{
    m_QBtn ->setText("1");
    m_WBtn ->setText("2");
    m_EBtn ->setText("3");
    m_RBtn ->setText("4");
    m_TBtn ->setText("5");
    m_YBtn ->setText("6");
    m_UBtn ->setText("7");
    m_IBtn ->setText("8");
    m_OBtn ->setText("9");
    m_PBtn ->setText("0");
    m_ABtn ->setText(",");
    m_SBtn ->setText("。");
    m_DBtn ->setText("、");
    m_FBtn ->setText("；");
    m_GBtn ->setText("：");
    m_HBtn ->setText("？");
    m_JBtn ->setText("！");
    m_KBtn ->setText("·");
    m_LBtn ->setText("＂");
    m_ZBtn ->setText("[");
    m_XBtn ->setText("]");
    m_CBtn ->setText("●");
    m_VBtn ->setText("※");
    m_BBtn ->setText("＃");
    m_NBtn ->setText("＠");
    m_MBtn ->setText("%");
}
void KeyBoard::setPlace()
{
    /*m_spellEdit ->setGeometry(10,10,60,30);//120,10,40,30

    m_leftBtn ->setGeometry(80,10,19,30);//165,10,19,30
    m_rightBtn ->setGeometry(483,10,19,30);//488,10,19,30

    m_ziBtn0 ->setGeometry(109,10,38,30);//184,10,38,30
    m_ziBtn1 ->setGeometry(157,10,38,30);
    m_ziBtn2 ->setGeometry(205,10,38,30);
    m_ziBtn3 ->setGeometry(253,10,38,30);
    m_ziBtn4 ->setGeometry(301,10,38,30);
    m_ziBtn5 ->setGeometry(349,10,38,30);
    m_ziBtn6 ->setGeometry(397,10,38,30);
    m_ziBtn7 ->setGeometry(445,10,38,30);

    m_zinum0 ->setGeometry(100,10,10,30);
    m_zinum1 ->setGeometry(148,10,10,30);
    m_zinum2 ->setGeometry(196,10,10,30);
    m_zinum3 ->setGeometry(244,10,10,30);
    m_zinum4 ->setGeometry(292,10,10,30);
    m_zinum5 ->setGeometry(340,10,10,30);
    m_zinum6 ->setGeometry(388,10,10,30);
    m_zinum7 ->setGeometry(436,10,10,30);

    m_QBtn ->setGeometry(10,50,40,40);
    m_WBtn ->setGeometry(60,50,40,40);
    m_EBtn ->setGeometry(110,50,40,40);
    m_RBtn ->setGeometry(160,50,40,40);
    m_TBtn ->setGeometry(210,50,40,40);
    m_YBtn ->setGeometry(260,50,40,40);
    m_UBtn ->setGeometry(310,50,40,40);
    m_IBtn ->setGeometry(360,50,40,40);
    m_OBtn ->setGeometry(410,50,40,40);
    m_PBtn ->setGeometry(460,50,40,40);
    m_ABtn ->setGeometry(36,100,40,40);
    m_SBtn ->setGeometry(86,100,40,40);
    m_DBtn ->setGeometry(136,100,40,40);
    m_FBtn ->setGeometry(186,100,40,40);
    m_GBtn ->setGeometry(236,100,40,40);
    m_HBtn ->setGeometry(286,100,40,40);
    m_JBtn ->setGeometry(336,100,40,40);
    m_KBtn ->setGeometry(386,100,40,40);
    m_LBtn ->setGeometry(436,100,40,40);

    m_ZBtn ->setGeometry(80,150,40,40);
    m_XBtn ->setGeometry(130,150,40,40);
    m_CBtn ->setGeometry(180,150,40,40);
    m_VBtn ->setGeometry(230,150,40,40);
    m_BBtn ->setGeometry(280,150,40,40);
    m_NBtn ->setGeometry(330,150,40,40);
    m_MBtn ->setGeometry(380,150,40,40);

    m_capsBtn ->setGeometry(10,150,65,40);
    m_deleteBtn ->setGeometry(430,150,70,40);
    m_signBtn ->setGeometry(10,200,60,40);
    m_changeBtn ->setGeometry(80,200,60,40);
    m_spaceBtn ->setGeometry(150,200,220,40);
    m_sureBtn ->setGeometry(380,200,60,40);*/

    /*m_QBtn ->setGeometry(10,50,40,40);
    m_WBtn ->setGeometry(50,50,40,40);
    m_EBtn ->setGeometry(90,50,40,40);


    m_RBtn ->setGeometry(10,90,40,40);
    m_TBtn ->setGeometry(50,90,40,40);
    m_YBtn ->setGeometry(90,90,40,40);


    m_UBtn ->setGeometry(10,130,40,40);
    m_IBtn ->setGeometry(50,130,40,40);
    m_OBtn ->setGeometry(90,130,40,40);

    m_PBtn ->setGeometry(10,170,40,40);
    m_ABtn ->setGeometry(50,170,40,40);
    m_deleteBtn ->setGeometry(90,170,40,40);
    //m_sureBtn ->setGeometry(90,170,40,40);*/

    mainLayout->addWidget(m_QBtn,0,0);
    mainLayout->addWidget(m_WBtn,0,1);
    mainLayout->addWidget(m_EBtn,0,2);

    mainLayout->addWidget(m_RBtn,1,0);
    mainLayout->addWidget(m_TBtn,1,1);
    mainLayout->addWidget(m_YBtn,1,2);

    mainLayout->addWidget(m_UBtn,2,0);
    mainLayout->addWidget(m_IBtn,2,1);
    mainLayout->addWidget(m_OBtn,2,2);

    mainLayout->addWidget(m_PBtn,3,0);
    mainLayout->addWidget(m_KBtn,3,1);
    mainLayout->addWidget(m_deleteBtn,3,2);

}

void KeyBoard::setConnect()
{
    connect(m_leftBtn,SIGNAL(clicked()),this,SLOT(leftFun()));
    connect(m_rightBtn,SIGNAL(clicked()),this,SLOT(rightFun()));

    connect(m_ziBtn0,SIGNAL(clicked()),this,SLOT(ziFun0()));
    connect(m_ziBtn1,SIGNAL(clicked()),this,SLOT(ziFun1()));
    connect(m_ziBtn2,SIGNAL(clicked()),this,SLOT(ziFun2()));
    connect(m_ziBtn3,SIGNAL(clicked()),this,SLOT(ziFun3()));
    connect(m_ziBtn4,SIGNAL(clicked()),this,SLOT(ziFun4()));
    connect(m_ziBtn5,SIGNAL(clicked()),this,SLOT(ziFun5()));
    connect(m_ziBtn6,SIGNAL(clicked()),this,SLOT(ziFun6()));
    connect(m_ziBtn7,SIGNAL(clicked()),this,SLOT(ziFun7()));

    connect(m_QBtn,SIGNAL(clicked()),this,SLOT(qFun()));
    connect(m_WBtn,SIGNAL(clicked()),this,SLOT(wFun()));
    connect(m_EBtn,SIGNAL(clicked()),this,SLOT(eFun()));
    connect(m_RBtn,SIGNAL(clicked()),this,SLOT(rFun()));
    connect(m_TBtn,SIGNAL(clicked()),this,SLOT(tFun()));
    connect(m_YBtn,SIGNAL(clicked()),this,SLOT(yFun()));
    connect(m_UBtn,SIGNAL(clicked()),this,SLOT(uFun()));
    connect(m_IBtn,SIGNAL(clicked()),this,SLOT(iFun()));
    connect(m_OBtn,SIGNAL(clicked()),this,SLOT(oFun()));
    connect(m_PBtn,SIGNAL(clicked()),this,SLOT(pFun()));
    connect(m_ABtn,SIGNAL(clicked()),this,SLOT(aFun()));
    connect(m_SBtn,SIGNAL(clicked()),this,SLOT(sFun()));
    connect(m_DBtn,SIGNAL(clicked()),this,SLOT(dFun()));
    connect(m_FBtn,SIGNAL(clicked()),this,SLOT(fFun()));
    connect(m_GBtn,SIGNAL(clicked()),this,SLOT(gFun()));
    connect(m_HBtn,SIGNAL(clicked()),this,SLOT(hFun()));
    connect(m_JBtn,SIGNAL(clicked()),this,SLOT(jFun()));
    connect(m_KBtn,SIGNAL(clicked()),this,SLOT(kFun()));
    connect(m_LBtn,SIGNAL(clicked()),this,SLOT(lFun()));
    connect(m_ZBtn,SIGNAL(clicked()),this,SLOT(zFun()));
    connect(m_XBtn,SIGNAL(clicked()),this,SLOT(xFun()));
    connect(m_CBtn,SIGNAL(clicked()),this,SLOT(cFun()));
    connect(m_VBtn,SIGNAL(clicked()),this,SLOT(vFun()));
    connect(m_BBtn,SIGNAL(clicked()),this,SLOT(bFun()));
    connect(m_NBtn,SIGNAL(clicked()),this,SLOT(nFun()));
    connect(m_MBtn,SIGNAL(clicked()),this,SLOT(mFun()));

    connect(m_capsBtn,SIGNAL(clicked()),this,SLOT(capsFun()));
    connect(m_deleteBtn,SIGNAL(clicked()),this,SLOT(deleteFun()));
    connect(m_changeBtn,SIGNAL(clicked()),this,SLOT(changeFun()));
    connect(m_spaceBtn,SIGNAL(clicked()),this,SLOT(spaceFun()));
    connect(m_sureBtn,SIGNAL(clicked()),this,SLOT(sureFun()));

    connect(m_spellEdit,SIGNAL(textChanged(QString)),this,SLOT(spellFun(QString)));
}
void KeyBoard::showHanzi()
{
  //  m_selectFlag = flag;
    if(m_storeCount>=m_selectFlag*8)
    {
        m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
        m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
        m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
        m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
        m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
        m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
        m_ziBtn6 ->setText(m_storeHanziStr.at(m_selectFlag*8-2));
        m_ziBtn7 ->setText(m_storeHanziStr.at(m_selectFlag*8-1));
    }
    else
    {
        int extraNum = m_selectFlag*8 - m_storeCount;
        switch(extraNum)
        {
            case 1: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
                    m_ziBtn6 ->setText(m_storeHanziStr.at(m_selectFlag*8-2));
                    m_ziBtn7 ->setText("");break;
            case 2: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 3: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 4: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 5: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 6: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText("");
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 7: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText("");
                    m_ziBtn2 ->setText("");
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
        }
    }
}

void KeyBoard::leftFun()
{
    m_selectFlag --;
    if(m_selectFlag<1)
        m_selectFlag =1;
    showHanzi();
}

void KeyBoard::rightFun()
{
    m_selectFlag ++;
    if(m_selectFlag>(m_storeCount/8+1))
        m_selectFlag =m_storeCount/8+1;
    showHanzi();
}
void KeyBoard::ziFun0()
{
    if(m_stateFlag==0)
        if(m_ziBtn0->text()!=NULL){
            emit input_text_pass(m_ziBtn0->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun1()
{
    if(m_stateFlag==0)
        if(m_ziBtn1->text()!=NULL){
            emit input_text_pass(m_ziBtn1->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun2()
{
    if(m_stateFlag==0)
        if(m_ziBtn2->text()!=NULL){
            emit input_text_pass(m_ziBtn2->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun3()
{
    if(m_stateFlag==0)
        if(m_ziBtn3->text()!=NULL){
            emit input_text_pass(m_ziBtn3->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun4()
{
    if(m_stateFlag==0)
        if(m_ziBtn4->text()!=NULL){
            emit input_text_pass(m_ziBtn4->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun5()
{
    if(m_stateFlag==0)
        if(m_ziBtn5->text()!=NULL){
            emit input_text_pass(m_ziBtn5->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun6()
{
    if(m_stateFlag==0)
        if(m_ziBtn6->text()!=NULL){
            emit input_text_pass(m_ziBtn6->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::ziFun7()
{
    if(m_stateFlag==0)
        if(m_ziBtn7->text()!=NULL){
            emit input_text_pass(m_ziBtn7->text());
            clear_all_m_ziBtn();
            m_spellEdit->clear();
        }
}

void KeyBoard::qFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("q");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("Q");
            else
                emit input_text_pass("q");
            break;
     case 2:emit input_text_pass("1");
     default:break;
    }
}

void KeyBoard::wFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("w");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("W");
            else
                emit input_text_pass("w");
            break;
     case 2:emit input_text_pass("2");
     default:break;
    }
}

void KeyBoard::eFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("e");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("E");
            else
                emit input_text_pass("e");
            break;
     case 2:emit input_text_pass("3");
     default:break;
    }
}

void KeyBoard::rFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("r");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("R");
            else
                emit input_text_pass("r");
            break;
     case 2:emit input_text_pass("4");
     default:break;
    }
}

void KeyBoard::tFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("t");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("T");
            else
                emit input_text_pass("t");
            break;
     case 2:emit input_text_pass("5");
     default:break;
    }
}

void KeyBoard::yFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("y");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("Y");
            else
                emit input_text_pass("y");
            break;
     case 2:emit input_text_pass("6");
     default:break;
    }
}

void KeyBoard::uFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("u");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("U");
            else
                emit input_text_pass("u");
            break;
     case 2:emit input_text_pass("7");
     default:break;
    }
}

void KeyBoard::iFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("i");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("I");
            else
                emit input_text_pass("i");
            break;
     case 2:emit input_text_pass("8");
     default:break;
    }
}

void KeyBoard::oFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("o");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("O");
            else
                emit input_text_pass("o");
            break;
     case 2:emit input_text_pass("9");
     default:break;
    }
}

void KeyBoard::pFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("p");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("P");
            else
                emit input_text_pass("p");
            break;
     case 2:emit input_text_pass("0");
     default:break;
    }
}

void KeyBoard::aFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("a");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("A");
            else
                emit input_text_pass("a");
            break;
     case 2:emit input_text_pass(",");
     default:break;
    }
}

void KeyBoard::sFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("s");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("S");
            else
                emit input_text_pass("s");
            break;
     case 2:emit input_text_pass("。");
     default:break;
    }
}

void KeyBoard::dFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("d");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("D");
            else
                emit input_text_pass("d");
            break;
     case 2:emit input_text_pass("、");
     default:break;
    }
}

void KeyBoard::fFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("f");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("F");
            else
                emit input_text_pass("f");
            break;
     case 2:emit input_text_pass(";");
     default:break;
    }
}

void KeyBoard::gFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("g");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("G");
            else
                emit input_text_pass("g");
            break;
     case 2:emit input_text_pass(":");
     default:break;
    }
}

void KeyBoard::hFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("h");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("H");
            else
                emit input_text_pass("h");
            break;
     case 2:emit input_text_pass("?");
     default:break;
    }
}

void KeyBoard::jFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("j");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("J");
            else
                emit input_text_pass("j");
            break;
     case 2:emit input_text_pass("!");
     default:break;
    }
}

void KeyBoard::kFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("k");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("K");
            else
                emit input_text_pass("k");
            break;
     case 2:emit input_text_pass(".");
     default:break;
    }
}

void KeyBoard::lFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("l");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("L");
            else
                emit input_text_pass("l");
            break;
     case 2:emit input_text_pass("\"");
     default:break;
    }
}

void KeyBoard::zFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("z");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("Z");
            else
                emit input_text_pass("z");
            break;
     case 2:emit input_text_pass("[");
     default:break;
    }
}

void KeyBoard::xFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("x");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("X");
            else
                emit input_text_pass("x");
            break;
     case 2:emit input_text_pass("]");
     default:break;
    }
}

void KeyBoard::cFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("c");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("C");
            else
                emit input_text_pass("c");
            break;
     case 2:emit input_text_pass("●");
     default:break;
    }
}

void KeyBoard::vFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("v");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("V");
            else
                emit input_text_pass("v");
            break;
     case 2:emit input_text_pass("※");
     default:break;
    }
}

void KeyBoard::bFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("b");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("B");
            else
                emit input_text_pass("b");
            break;
     case 2:emit input_text_pass("#");
     default:break;
    }
}

void KeyBoard::nFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("n");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("N");
            else
                emit input_text_pass("n");
            break;
     case 2:emit input_text_pass("@");
     default:break;
    }
}

void KeyBoard::mFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("m");break;
    case 1:if(m_capsFlag)
                emit input_text_pass("M");
            else
                emit input_text_pass("m");
            break;
     case 2:emit input_text_pass("%");
     default:break;
    }
}

void KeyBoard::capsFun()
{
    if(m_stateFlag==1)
    {
        if(m_capsFlag)
        {
            setName_en();
            m_signBtn ->setText("状态：en");
            m_capsFlag = false;
        }
        else
        {
            setName_EN();
            m_signBtn ->setText("状态：EN");
            m_capsFlag = true;
        }
    }
}

void KeyBoard::deleteFun()
{
    if((m_spellEdit -> text()) != NULL)
       m_spellEdit -> backspace();
    else emit input_text_pass("Backspace");
}

void KeyBoard::changeFun()
{
    m_stateFlag ++;
    if(m_stateFlag==3)
        m_stateFlag=0;
    switch(m_stateFlag)
    {
    //case 0:setName_en();m_signBtn ->setText("状态：CN");break;
    //case 1:setName_EN();m_signBtn ->setText("状态：EN");break;
    case 2:setName_sn();m_signBtn ->setText("  */123");break;
    default:break;
    }
    m_storeHanziStr = "";
    m_storeCount =0;
    m_ziBtn0 ->setText("");
    m_ziBtn1 ->setText("");
    m_ziBtn2 ->setText("");
    m_ziBtn3 ->setText("");
    m_ziBtn4 ->setText("");
    m_ziBtn5 ->setText("");
    m_ziBtn6 ->setText("");
    m_ziBtn7 ->setText("");
}

void KeyBoard::spaceFun()
{

}

void KeyBoard::sureFun()
{
    this->close();//退出界面
}

void KeyBoard::spellFun(QString text)
{
    qDebug()<<"spell text = "<<text;
    QString str ,stringLine;
    int ret=0;
    m_in -> seek(0);
    if(text!=NULL)
    {
        while(!m_in->atEnd())
        {
            stringLine = m_in -> readLine();
            str = stringLine.left(text.length());
            ret = QString::compare(str, text);
            if(!ret)
            {
                QStringList strlist = stringLine.split(" ");
                stringLine = strlist[1];
                m_storeHanziStr = stringLine;
                m_storeCount = stringLine.count();
                m_selectFlag = 1;
                qDebug()<<"m_storeHanziStr="<<m_storeHanziStr<<"count ="<<m_storeCount;
                break;
            }
        }
        qDebug()<<"";
        showHanzi();
    }
}

void KeyBoard::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<event->key();
    switch(event->key())
    {
    case Qt::Key_0:
            qDebug()<<"Key:0 Pressed!";
            emit input_text_pass("0");
            break;
    case Qt::Key_1:
            qDebug()<<"Key:1 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("1");
            else {
                emit input_text_pass(m_ziBtn0->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_2:
            qDebug()<<"Key:2 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("2");
            else {
                emit input_text_pass(m_ziBtn1->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_3:
            qDebug()<<"Key:3 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("3");
            else {
                emit input_text_pass(m_ziBtn2->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_4:
            qDebug()<<"Key:4 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("4");
            else {
                emit input_text_pass(m_ziBtn3->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_5:
            qDebug()<<"Key:5 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("5");
            else {
                emit input_text_pass(m_ziBtn4->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_6:
            qDebug()<<"Key:6 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("6");
            else {
                emit input_text_pass(m_ziBtn5->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_7:
            qDebug()<<"Key:7 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("7");
            else {
                emit input_text_pass(m_ziBtn6->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_8:
            qDebug()<<"Key:8 Pressed!";
            if(m_ziBtn0->text() == NULL)
                emit input_text_pass("8");
            else {
                emit input_text_pass(m_ziBtn7->text());
                clear_all_m_ziBtn();
                }
            break;
    case Qt::Key_9:
            qDebug()<<"Key:9 Pressed!";
            emit input_text_pass("9");
            break;
    case  Qt:: Key_A:
            aFun();
            qDebug()<<"Key:A Pressed!";
            break;
    case  Qt:: Key_B:
            bFun();
            qDebug()<<"Key:B Pressed!";
            break;
    case  Qt:: Key_C:
            cFun();
            qDebug()<<"Key:C Pressed!";
            break;
    case  Qt:: Key_D:
            dFun();
            qDebug()<<"Key:D Pressed!";
            break;
    case  Qt:: Key_E:
            eFun();
            qDebug()<<"Key:E Pressed!";
            break;
    case  Qt:: Key_F:
            fFun();
            qDebug()<<"Key:F Pressed!";
            break;
    case  Qt:: Key_G:
            gFun();
            qDebug()<<"Key:G Pressed!";
            break;
    case  Qt:: Key_H:
            hFun();
            qDebug()<<"Key:H Pressed!";
            break;
    case  Qt:: Key_I:
            iFun();
            qDebug()<<"Key:I Pressed!";
            break;
    case  Qt:: Key_J:
            jFun();
            qDebug()<<"Key:J Pressed!";
            break;
    case  Qt:: Key_K:
            kFun();
            qDebug()<<"Key:K Pressed!";
            break;
    case  Qt:: Key_L:
            lFun();
            qDebug()<<"Key:L Pressed!";
            break;
    case  Qt:: Key_M:
            mFun();
            qDebug()<<"Key:M Pressed!";
            break;
    case  Qt:: Key_N:
            nFun();
            qDebug()<<"Key:N Pressed!";
            break;
    case  Qt:: Key_O:
            oFun();
            qDebug()<<"Key:O Pressed!";
            break;
    case  Qt:: Key_P:
            pFun();
            qDebug()<<"Key:P Pressed!";
            break;
    case  Qt:: Key_Q:
            qFun();
            qDebug()<<"Key:Q Pressed!";
            break;
    case  Qt:: Key_R:
            rFun();
            qDebug()<<"Key:R Pressed!";
            break;
    case  Qt:: Key_S:
            sFun();
            qDebug()<<"Key:S Pressed!";
            break;
    case  Qt:: Key_T:
            tFun();
            qDebug()<<"Key:T Pressed!";
            break;
    case  Qt:: Key_U:
            uFun();
            qDebug()<<"Key:U Pressed!";
            break;
    case  Qt:: Key_V:
            vFun();
            qDebug()<<"Key:V Pressed!";
            break;
    case  Qt:: Key_W:
            wFun();
            qDebug()<<"Key:W Pressed!";
            break;
    case  Qt:: Key_X:
            xFun();
            qDebug()<<"Key:X Pressed!";
            break;
    case  Qt:: Key_Y:
            yFun();
            qDebug()<<"Key:Y Pressed!";
            break;
    case  Qt:: Key_Z:
            zFun();
            qDebug()<<"Key:Z Pressed!";
            break;
    case  Qt:: Key_Minus:
            leftFun();
            qDebug()<<"Key:- Pressed!";
            break;
    case  Qt:: Key_Equal:
            rightFun();
            qDebug()<<"Key:= Pressed!";
            break;
    case  Qt:: Key_Backspace:
            deleteFun();
            qDebug()<<"Key:Key_Backspace Pressed!";
            break;
    case  Qt:: Key_Return:
            sureFun();
            qDebug()<<"Key:Enter Pressed!";
            break;
    case  Qt:: Key_Shift:
            changeFun();
            qDebug()<<"Key:shift Pressed!";
            break;
    case  Qt:: Key_CapsLock:
            capsFun();
            qDebug()<<"Key:CapsLock Pressed!";
            break;
    default:
            QWidget::keyPressEvent(event);
            break;
    }
}

void KeyBoard::clear_all_m_ziBtn()
{
    m_ziBtn0 ->setText("");
    m_ziBtn1 ->setText("");
    m_ziBtn2 ->setText("");
    m_ziBtn3 ->setText("");
    m_ziBtn4 ->setText("");
    m_ziBtn5 ->setText("");
    m_ziBtn6 ->setText("");
    m_ziBtn7 ->setText("");
    m_spellEdit->clear();
}



