#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T06:41:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

QT       += network

QT       += xml

TRANSLATIONS = cn.ts
TRANSLATIONS += de.ts
TRANSLATIONS += en.ts
TRANSLATIONS += sp.ts
TRANSLATIONS += fr.ts
TRANSLATIONS += it.ts
TRANSLATIONS += kr.ts
TRANSLATIONS += rus.ts
TRANSLATIONS += por.ts

TARGET = controller
TEMPLATE = app

DEFINES += CONFIG_CTRL_IFACE

DEFINES += CONFIG_CTRL_IFACE_UNIX

#INCLUDEPATH	+= . .. ../../../../../../860210000/linux/am3358/wpa/wpa_supplicant-0.7.3/src/utils
INCLUDEPATH	+= . .. ./ecspos ../../../../../../860210000/linux/am3358/wpa/wpa_supplicant-0.7.3/src/utils

SOURCES += main.cpp\
        mainwindow.cpp \
    ctrlapplication.cpp \
    list.c \
    log.c \
    LoginDlg.cpp \
    LockupDlg.cpp \
    syszuxim.cpp \
    syszuxpinyin.cpp \
    navigatorbar.cpp \
    cbitmapbutton.cpp \
    basewidget.cpp \
    mainpage.cpp \
    subpage.cpp \
    setpage.cpp \
    servicepage.cpp \
    page.cpp \
    titlebar.cpp \
    menupage.cpp \
    waterqualitypage.cpp \
    systestlistwidgtitem.cpp \
    consumablestatepage.cpp \
    ConsumableinsPage.cpp \
    SterilizePage.cpp \
    Cslistitem.cpp \
    warnlistwidgtitem.cpp \
    setdevicepage.cpp \
    setpoint.cpp \
    setplistwidgtitem.cpp \
    systestpage.cpp \
    systemcfgpage.cpp \
    maintenancecycle.cpp \
    cosumableinslistwidgtitem.cpp \
    maintenancelistwidgtitem.cpp \
    parametercalibration.cpp \
    parameterlistwidgtitem.cpp \
    sendtopage.cpp \
    unitpage.cpp \
    msgbox.cpp \
    networkpage.cpp \
    allocationsetpage.cpp \
    alarmsetpage.cpp \
    alarmsetlistwidgtitem.cpp \
    alarmdisplaypage.cpp \
    alarmdisplaylistitem.cpp \
    sysmsgpage.cpp \
    systemmonitorpage.cpp \
    systemmonitorlistwidgtitem.cpp \
    ToastDlg.cpp \
    canItf.c \
    zb.c \
    SerialItf4zb.c \
    helper.c \
    ccb.c \
    dtextticker.cpp \
    dnetworkaccessmanager.cpp \
    dhttpworker.cpp \
    dwificonfigwidget.cpp \
    dwificonfigdialog.cpp \
    dwaterqualitywidget.cpp \
    dlineedit.cpp \
    dsqltablemodelforuser.cpp \
    dloginwarningdialog.cpp \
    dconsumableinstallwidget.cpp \
    dpushbutton.cpp \
    drunwarningdialog.cpp \
    printer.cpp \
    dxmlgenerator.cpp \
    dinithandlecfgpage.cpp \
    dmanagersetpage.cpp \
    dbasewindow.cpp \
    dcalcpackflow.cpp \
    dcheckconsumaleinstall.cpp \
    dcomboboxdelegate.cpp \
    dconsumableinstalldialog.cpp \
    ddialogtitle.cpp \
    dsleepwidget.cpp \
    deventfilter.cpp \
    dfactorytestpage.cpp \
    dflowchartpage.cpp \
    dflowchartwidget.cpp \
    dhintdialog.cpp \
    dhistorypage.cpp \
    dinitlanguagepage.cpp \
    dinitnetworkpage.cpp \
    dinitsyscfgpage.cpp \
    dinittankcfgpage.cpp \
    dinittimepage.cpp \
    dinitconsumableinspage.cpp \
    dlanguagepage.cpp \
    dwaterqualitypage.cpp \
    dusermanualpage.cpp \
    dloginstate.cpp \
    dpermissionsetpage.cpp \
    drfidcfgpage.cpp \
    dscreensleeppage.cpp \
    dscreensleepthread.cpp \
    dsuperpowerpage.cpp \
    dsysmsgpage.cpp \
    dusercfgpage.cpp \
    duserinfochecker.cpp
HEADERS  += mainwindow.h \
    ctrlapplication.h \
    log.h \
    LoginDlg.h \
    eco_w.h \
    MyParams.h \
    LockupDlg.h \
    syszuxpinyin.h \
    syszuxim.h \
    qsliderex.h \
    navigatorbar.h \
    memory.h \
    cbitmapbutton.h \
    setpage.h \
    menupage.h \
    mainpage.h \
    basewidget.h \
    page.h \
    subpage.h \
    servicepage.h \
    waterqualitypage.h \
    titlebar.h \
    consumablestatepage.h \
    ConsumableinsPage.h \
    warnlistwidgtitem.h \
    SterilizePage.h \
    systestlistwidgtitem.h \
    Cslistitem.h \
    setdevicepage.h \
    setpoint.h \
    setplistwidgtitem.h \
    systestpage.h \
    systemcfgpage.h \
    maintenancelistwidgtitem.h \
    maintenancecycle.h \
    consumableinslistwidgtitem.h \
    parametercalibration.h \
    parameterlistwidgtitem.h \
    sendtopage.h \
    unitpage.h \
    msgbox.h \
    networkpage.h \
    allocationsetpage.h \
    alarmsetpage.h \
    alarmsetlistwidgtitem.h \
    alarmdisplaypage.h \
    alarmdisplaylistitem.h \
    sysmsgpage.h \
    systemmonitorpage.h \
    systemmonitorlistwidgtitem.h \
    ToastDlg.h \
    dtextticker.h \
    DNetworkConfig.h \
    dnetworkaccessmanager.h \
    dhttpworker.h \
    dwificonfigwidget.h \
    dwificonfigdialog.h \
    dwaterqualitywidget.h \
    dlineedit.h \
    dsqltablemodelforuser.h \
    dloginwarningdialog.h \
    dconsumableinstallwidget.h \
    dpushbutton.h \
    drunwarningdialog.h \
    escpos/init_parser.h \
    printer.h \
    dxmlgenerator.h \
    exdisplay.h \
    exconfig.h \
    dinithandlecfgpage.h \
    dmanagersetpage.h \
    dbasewindow.h \
    dcalcpackflow.h \
    dcheckconsumaleinstall.h \
    dcomboboxdelegate.h \
    dconsumableinstalldialog.h \
    ddialogtitle.h \
    dsleepwidget.h \
    deventfilter.h \
    dfactorytestpage.h \
    dflowchartpage.h \
    dflowchartwidget.h \
    dhintdialog.h \
    dhistorypage.h \
    dinitlanguagepage.h \
    dinitnetworkpage.h \
    dinitsyscfgpage.h \
    dinittankcfgpage.h \
    dinittimepage.h \
    dinitconsumableinspage.h \
    dlanguagepage.h \
    dloginstate.h \
    dpermissionsetpage.h \
    drfidcfgpage.h \
    dscreensleeppage.h \
    dscreensleepthread.h \
    dsuperpowerpage.h \
    dsysmsgpage.h \
    dusercfgpage.h \
    dusermanualpage.h \
    dwaterqualitypage.h \
    duserinfochecker.h

FORMS    += mainwindow.ui \
    syszuxpinyin.ui \
    IMEdigi.ui

RESOURCES += \
    ime.qrc \
    syszuxpinyin.qrc \
    app.qrc \
    image.qrc \
    language.qrc \
    other.qrc \
    json.qrc

OTHER_FILES += \
    pics/image/arrow_down_active.png \
    pics/image/battery0.png \
    slider.qss \
    musicslider.qss \
    cn.ts \
    de.ts \
    en.ts \
    Table.qss \
    combox.qss

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lcommon
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lcommon
#else:symbian: LIBS += -lcommon
#else:unix: LIBS += -L$$PWD  -lcommon
#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/common.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/common.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/libcommon.a

##
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lcommon -lescpos
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lcommon -lescpos
else:symbian: LIBS += -lcommon
else:unix: LIBS += -L$$PWD  -lcommon -lescpos
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/common.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/common.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/libcommon.a

