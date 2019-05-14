#ifndef CTRLAPPLICATION_H
#define CTRLAPPLICATION_H

#include <QApplication>
#include <QWSEvent>
#include <QTranslator>
#include <QStringList>

class MainWindow;

class CtrlApplication : public QApplication
{
    Q_OBJECT

public:
    explicit CtrlApplication( int argc, char **argv );
    virtual bool qwsEventFilter(QWSEvent *event);
    void installTranslators(int iLanguage = 0,bool bSwitchFont = false);
    QTranslator *m_pTranslator;
    MainWindow  *m_pMainWnd;
    QStringList m_strLstFont;

	bool    m_bFontLoaded;
	int     m_iFontNum;
    
signals:

public slots:

};

extern CtrlApplication *gApp;

#endif // CTRLAPPLICATION_H
