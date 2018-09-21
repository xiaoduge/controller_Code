#include "ex_screensleeppage.h"
#include "mainwindow.h"
#include <QLabel>


Ex_ScreenSleepPage::Ex_ScreenSleepPage(QObject *parent, CBaseWidget *widget, MainWindow  *wndMain)
    :CPage(parent, widget, wndMain)
{
    initUi();
    buildTranslation();
}

void Ex_ScreenSleepPage::buildTranslation()
{
    QString titleMsg;
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        titleMsg = tr("SuperGenie G");
        break;
    case MACHINE_L_UP:
        titleMsg = tr("SuperGenie U");
        break;
    case MACHINE_L_EDI_LOOP:
        titleMsg = tr("SuperGenie E");
        break;
    case MACHINE_L_RO_LOOP:
        titleMsg = tr("SuperGenie R");
        break;
    case MACHINE_Genie:
        titleMsg = tr("Genie G");
        break;
    case MACHINE_UP:
        titleMsg = tr("Genie U");
        break;
    case MACHINE_EDI:
        titleMsg = tr("Genie E");
        break;
    case MACHINE_RO:
        titleMsg = tr("Genie R");
        break;
    case MACHINE_PURIST:
        titleMsg = tr("PURIST U");
        break;
     case MACHINE_ADAPT:
        titleMsg = tr("Genie A");
        break;
    }
    m_msgLabel->setText(titleMsg);
}

void Ex_ScreenSleepPage::initUi()
{
    QWidget* tempWidget = new QWidget(m_widget);
//    QPalette palette = tempWidget->palette();
//    palette.setBrush(QColor());

    m_msgLabel = new QLabel(tempWidget);
    m_msgLabel->setGeometry(0, 0, 800, 600);
    m_msgLabel->setAlignment(Qt::AlignHCenter);
    QString qss = "QLabel{font-family: Arial; \
                          font-size: 72px;\
                          font-weight: bold;\
                          font-style:italic;\
                          color: blue;}";
    m_msgLabel->setStyleSheet(qss);
}

void Ex_ScreenSleepPage::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit pageHide();
}
