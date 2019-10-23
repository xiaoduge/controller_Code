/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   12,   11,   11, 0x05,
      63,   11,   11,   11, 0x05,
      86,   75,   11,   11, 0x05,
     118,   11,   11,   11, 0x05,
     138,   11,   11,   11, 0x05,
     163,  153,   11,   11, 0x05,
     198,   11,   11,   11, 0x05,
     226,  214,   11,   11, 0x05,
     258,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     292,   11,   11,   11, 0x0a,
     314,   11,   11,   11, 0x08,
     330,   11,   11,   11, 0x08,
     352,   11,   11,   11, 0x08,
     374,   11,   11,   11, 0x08,
     401,   12,   11,   11, 0x08,
     443,   11,  439,   11, 0x08,
     458,   11,   11,   11, 0x08,
     476,   11,   11,   11, 0x08,
     494,   11,   11,   11, 0x08,
     513,   11,   11,   11, 0x08,
     528,   75,   11,   11, 0x08,
     567,  563,   11,   11, 0x08,
     587,   11,   11,   11, 0x08,
     609,   11,   11,   11, 0x08,
     631,   11,   11,   11, 0x08,
     650,   11,   11,   11, 0x08,
     673,   11,   11,   11, 0x08,
     699,   11,   11,   11, 0x08,
     731,  722,   11,   11, 0x08,
     767,   11,   11,   11, 0x08,
     812,  792,   11,   11, 0x08,
     868,  862,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0pucData,iLength\0"
    "dispIndication(unsigned char*,int)\0"
    "autoLogin()\0pIapNotify\0"
    "iapIndication(IAP_NOTIFY_STRU*)\0"
    "SleepPageShow(bool)\0unitsChanged()\0"
    "msg,isAdd\0updateFlowChartAlarm(QString,bool)\0"
    "userNeedLogin()\0networkData\0"
    "sendHttpHeartData(DNetworkData)\0"
    "sendHttpAlarm(DNetworkAlaramInfo)\0"
    "retriveLastRunState()\0on_timerEvent()\0"
    "on_timerPeriodEvent()\0on_timerSecondEvent()\0"
    "on_timerScreenSleepEvent()\0"
    "on_dispIndication(unsigned char*,int)\0"
    "int\0on_userLogin()\0on_Stop_clicked()\0"
    "on_Exit_clicked()\0on_pbRun_clicked()\0"
    "on_AutoLogin()\0on_IapIndication(IAP_NOTIFY_STRU*)\0"
    "tmp\0on_btn_clicked(int)\0on_Gif_State_Change()\0"
    "on_timerBuzzerEvent()\0buildTranslation()\0"
    "on_Ex_ScreenPageHide()\0on_updateText(QByteArray)\0"
    "on_timerNetworkEvent()\0fileName\0"
    "on_mqttWatcher_fileChanged(QString)\0"
    "on_mqttProcess_started()\0exitCode,exitStatus\0"
    "on_mqttProcess_finished(int,QProcess::ExitStatus)\0"
    "sleep\0on_ScreenSleep(bool)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->dispIndication((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->autoLogin(); break;
        case 2: _t->iapIndication((*reinterpret_cast< IAP_NOTIFY_STRU*(*)>(_a[1]))); break;
        case 3: _t->SleepPageShow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->unitsChanged(); break;
        case 5: _t->updateFlowChartAlarm((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->userNeedLogin(); break;
        case 7: _t->sendHttpHeartData((*reinterpret_cast< const DNetworkData(*)>(_a[1]))); break;
        case 8: _t->sendHttpAlarm((*reinterpret_cast< const DNetworkAlaramInfo(*)>(_a[1]))); break;
        case 9: _t->retriveLastRunState(); break;
        case 10: _t->on_timerEvent(); break;
        case 11: _t->on_timerPeriodEvent(); break;
        case 12: _t->on_timerSecondEvent(); break;
        case 13: _t->on_timerScreenSleepEvent(); break;
        case 14: _t->on_dispIndication((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: { int _r = _t->on_userLogin();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: _t->on_Stop_clicked(); break;
        case 17: _t->on_Exit_clicked(); break;
        case 18: _t->on_pbRun_clicked(); break;
        case 19: _t->on_AutoLogin(); break;
        case 20: _t->on_IapIndication((*reinterpret_cast< IAP_NOTIFY_STRU*(*)>(_a[1]))); break;
        case 21: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->on_Gif_State_Change(); break;
        case 23: _t->on_timerBuzzerEvent(); break;
        case 24: _t->buildTranslation(); break;
        case 25: _t->on_Ex_ScreenPageHide(); break;
        case 26: _t->on_updateText((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 27: _t->on_timerNetworkEvent(); break;
        case 28: _t->on_mqttWatcher_fileChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 29: _t->on_mqttProcess_started(); break;
        case 30: _t->on_mqttProcess_finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 31: _t->on_ScreenSleep((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::dispIndication(unsigned char * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::autoLogin()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MainWindow::iapIndication(IAP_NOTIFY_STRU * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::SleepPageShow(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::unitsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MainWindow::updateFlowChartAlarm(const QString & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::userNeedLogin()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void MainWindow::sendHttpHeartData(const DNetworkData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MainWindow::sendHttpAlarm(const DNetworkAlaramInfo & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
