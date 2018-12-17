#include "ctrlapplication.h"
#include "mainwindow.h"

#include <QStringList>
#include <QFontDatabase>


CtrlApplication::CtrlApplication( int argc, char **argv ):
    QApplication( argc, argv )
{
    m_pTranslator = NULL;
    m_pMainWnd    = NULL;
    m_bFontLoaded = false;
    m_iFontNum    = 0;

    /* variable init */
    MainRetriveGlobalParam();

}


bool CtrlApplication::qwsEventFilter(QWSEvent *event)
{

    if (QWSEvent::Mouse  == event->type
        || QWSEvent::Key == event->type)
    {

    }

    if(QWSEvent::Key == event->type)
    {
        QWSKeyEvent *ke = static_cast<QWSKeyEvent*>(event);
        QEvent::Type type = ke->simpleData.is_press ?
                            QEvent::KeyPress : QEvent::KeyRelease;


        // qDebug("Key: %d,%d\n",ke->simpleData.keycode,type);
        switch(ke->simpleData.keycode)
        {
        case Qt::Key_Home: // for lcd screen on off, code: 0x01000010
            if (type == QEvent::KeyRelease)
            {
            }
            return TRUE;
        case Qt::Key_Down: // code: 0x01000015
            // do someting, later implement
            return TRUE;
        case Qt::Key_Up: // code : 0x01000013
            // do someting, later implement
            return TRUE;
        case Qt::Key_End: // code : 0x01000011
            // enter into sleep mode
            return TRUE;
        }
    }
    return false;
}

void CtrlApplication::installTranslators(int iLanguage,bool bSwitchFont)
{
    if (m_pTranslator)
    {
        delete m_pTranslator;
        m_pTranslator = NULL;
    }

    if (!m_bFontLoaded)
    {
        char *fontdir = getenv("QT_QWS_FONTDIR");
        char fontname[256];
        int loadedFontID;
        int fontNum = 0;
        int iLoop;
        QString strFontName;
        char * astrFontFile[] = {
            "FZZDXJW.TTF",
            "myriadproregular.ttf",
            "myriadprobold.ttf",
            "myriadcjk3.ttf",
            NULL,
        };

        for (iLoop = 0; astrFontFile[iLoop] ;iLoop++)
        {
            sprintf(fontname,"%s/%s",fontdir,astrFontFile[iLoop]);
            loadedFontID = QFontDatabase::addApplicationFont(fontname);  
            QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
            if(!loadedFontFamilies.empty())
            {
                strFontName = loadedFontFamilies.at(0);  
                m_strLstFont << strFontName;
                fontNum++;
                qDebug() << astrFontFile[iLoop] << endl;
            }
        }

        m_bFontLoaded = true;
        m_iFontNum = fontNum;
    }
    {

        m_pTranslator = new QTranslator;

        switch(iLanguage)
        {
        case APP_LAN_CHN:
            m_pTranslator->load(":/language/cn.qm");
            installTranslator(m_pTranslator);
            break;
        case APP_LAN_SPA:
            m_pTranslator->load(":/language/sp.qm");
            installTranslator(m_pTranslator);
            break;
        case APP_LAN_FRE:
            m_pTranslator->load(":/language/fr.qm");
            installTranslator(m_pTranslator);
            break;
        case APP_LAN_ITA:
            m_pTranslator->load(":/language/it.qm");
            installTranslator(m_pTranslator);
            break;
        case APP_LAN_SKR:
            m_pTranslator->load(":/language/kr.qm");
            installTranslator(m_pTranslator);
            break;
        case APP_LAN_GER:
            m_pTranslator->load(":/language/de.qm");
            installTranslator(m_pTranslator);
            break;
        default:
            m_pTranslator->load(":/language/en.qm");
            installTranslator(m_pTranslator);
            
            break;
        }

        if (bSwitchFont && m_iFontNum >= 2)
        {
            switch(iLanguage)
            {
            default:
            case APP_LAN_CHN:
                {
                    QFont font(m_strLstFont[0],16);
                    QApplication::setFont(font);
                }
                break;
            case APP_LAN_ENG:
            case APP_LAN_SPA:
            case APP_LAN_FRE:
            case APP_LAN_GER:
            case APP_LAN_ITA:
                {
                    QFont font(m_strLstFont[1],16);
                    QApplication::setFont(font);
                }
                break;
            }
        }
        else
        {
            QFont font  = this->font();
            font.setPointSize(16);
            setFont(font);
        }
        
    }    
}

