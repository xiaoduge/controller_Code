#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T06:41:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

TRANSLATIONS = cn.ts
TRANSLATIONS += de.ts
TRANSLATIONS += en.ts

TARGET = controller
TEMPLATE = app

DEFINES += CONFIG_CTRL_IFACE

DEFINES += CONFIG_CTRL_IFACE_UNIX

INCLUDEPATH	+= . .. ../../../../../../860210000/linux/am3358/wpa/wpa_supplicant-0.7.3/src/utils

SOURCES += main.cpp\
        mainwindow.cpp \
    ctrlapplication.cpp \
    list.c \
    log.c \
    CfgDialog.cpp \
    DbDialog.cpp \
    LoginDlg.cpp \
    eco_w.cpp \
    usermanage.cpp \
    keyboard.cpp \
    opt.cpp \
    user.cpp \
    CMDialog.cpp \
    EngDialog.cpp \
    switchbtn.cpp \
    LockupDlg.cpp \
    syszuxim.cpp \
    syszuxpinyin.cpp \
    PermissionDlg.cpp \
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
    historypage.cpp \
    historylistwidgtitem.cpp \
    consumablestatepage.cpp \
    ConsumableinsPage.cpp \
    SterilizePage.cpp \
    reportpage.cpp \
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
    languagepage.cpp \
    soundpage.cpp \
    sendtopage.cpp \
    unitpage.cpp \
    displaypage.cpp \
    msgbox.cpp \
    timepage.cpp \
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
    ex_loginstate.cpp \
    ex_init_languagepage.cpp \
    ex_superpowerpage.cpp \
    ex_init_networkpage.cpp \
    ex_init_handlecfgpage.cpp \
    ex_sysmsgpage.cpp \
    ex_init_timepage.cpp \
    ex_init_tankcfgpage.cpp \
    ex_usercfgpage.cpp \
    ex_comboboxdelegate.cpp \
    ccb.c \
    ex_userinfo.cpp \
    ex_init_syscfgpage.cpp \
    ex_flowchartpage.cpp \
    ex_historypage.cpp \
    ex_rfidcfgpage.cpp \
    ex_screensleepthread.cpp \
    ex_eventfilter.cpp \
    ex_screensleeppage.cpp
HEADERS  += mainwindow.h \
    ctrlapplication.h \
    log.h \
    CfgDialog.h \
    DbDialog.h \
    LoginDlg.h \
    eco_w.h \
    usermanage.h \
    keyboard.h \
    opt.h \
    user.h \
    MyParams.h \
    CMDialog.h \
    switchbtn.h \
    EngDialog.h \
    LockupDlg.h \
    syszuxpinyin.h \
    syszuxim.h \
    PermissionDlg.h \
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
    historypage.h \
    historylistwidgtitem.h \
    consumablestatepage.h \
    ConsumableinsPage.h \
    warnlistwidgtitem.h \
    SterilizePage.h \
    systestlistwidgtitem.h \
    reportpage.h \
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
    soundpage.h \
    languagepage.h \
    sendtopage.h \
    unitpage.h \
    displaypage.h \
    msgbox.h \
    timepage.h \
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
    ExtraDisplay.h \
    ex_loginstate.h \
    ex_init_languagepage.h \
    ex_superpowerpage.h \
    ex_init_networkpage.h \
    ex_init_handlecfgpage.h \
    ex_sysmsgpage.h \
    ex_init_timepage.h \
    ex_init_tankcfgpage.h \
    ex_usercfgpage.h \
    ex_comboboxdelegate.h \
    Ex_Display_c.h \
    ex_userinfo.h \
    ex_init_syscfgpage.h \
    ex_flowchartpage.h \
    ex_historypage.h \
    ex_rfidcfgpage.h \
    ex_screensleepthread.h \
    ex_eventfilter.h \
    ex_screensleeppage.h

FORMS    += mainwindow.ui \
    syszuxpinyin.ui \
    IMEdigi.ui

RESOURCES += \
    ime.qrc \
    syszuxpinyin.qrc \
    app.qrc \
    image.qrc \
    language.qrc \
    other.qrc

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lcommon
else:symbian: LIBS += -lcommon
else:unix: LIBS += -L$$PWD  -lcommon
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/common.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/common.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/libcommon.a

