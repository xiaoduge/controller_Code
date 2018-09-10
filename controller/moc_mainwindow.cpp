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
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   12,   11,   11, 0x05,
      63,   11,   11,   11, 0x05,
      86,   75,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     118,   11,   11,   11, 0x08,
     134,   11,   11,   11, 0x08,
     156,   11,   11,   11, 0x08,
     178,   12,   11,   11, 0x08,
     216,   11,   11,   11, 0x08,
     233,   11,   11,   11, 0x08,
     251,   11,   11,   11, 0x08,
     269,   11,   11,   11, 0x08,
     288,   11,   11,   11, 0x08,
     303,   75,   11,   11, 0x08,
     338,   11,   11,   11, 0x08,
     364,  360,   11,   11, 0x08,
     384,   11,   11,   11, 0x08,
     406,   11,   11,   11, 0x08,
     428,   11,   11,   11, 0x08,
     447,   11,   11,   11, 0x08,
     467,   11,   11,   11, 0x08,
     491,   11,   11,   11, 0x08,
     512,   11,   11,   11, 0x08,
     536,   11,   11,   11, 0x08,
     559,   11,   11,   11, 0x08,
     581,   11,   11,   11, 0x08,
     611,  605,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0pucData,iLength\0"
    "dispIndication(unsigned char*,int)\0"
    "autoLogin()\0pIapNotify\0"
    "iapIndication(IAP_NOTIFY_STRU*)\0"
    "on_timerEvent()\0on_timerPeriodEvent()\0"
    "on_timerSecondEvent()\0"
    "on_dispIndication(unsigned char*,int)\0"
    "on_Opt_clicked()\0on_Stop_clicked()\0"
    "on_Exit_clicked()\0on_pbRun_clicked()\0"
    "on_AutoLogin()\0on_IapIndication(IAP_NOTIFY_STRU*)\0"
    "on_pbDevice_clicked()\0tmp\0on_btn_clicked(int)\0"
    "on_Gif_State_Change()\0on_timerBuzzerEvent()\0"
    "buildTranslation()\0on_Ex_Init_Lan(int)\0"
    "on_Ex_Init_Network(int)\0on_Ex_Init_Time(int)\0"
    "on_Ex_Init_TankCfg(int)\0on_Ex_Init_Syscfg(int)\0"
    "on_Ex_Init_Finished()\0on_Ex_Init_Handler(int)\0"
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
        case 3: _t->on_timerEvent(); break;
        case 4: _t->on_timerPeriodEvent(); break;
        case 5: _t->on_timerSecondEvent(); break;
        case 6: _t->on_dispIndication((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->on_Opt_clicked(); break;
        case 8: _t->on_Stop_clicked(); break;
        case 9: _t->on_Exit_clicked(); break;
        case 10: _t->on_pbRun_clicked(); break;
        case 11: _t->on_AutoLogin(); break;
        case 12: _t->on_IapIndication((*reinterpret_cast< IAP_NOTIFY_STRU*(*)>(_a[1]))); break;
        case 13: _t->on_pbDevice_clicked(); break;
        case 14: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_Gif_State_Change(); break;
        case 16: _t->on_timerBuzzerEvent(); break;
        case 17: _t->buildTranslation(); break;
        case 18: _t->on_Ex_Init_Lan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_Ex_Init_Network((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_Ex_Init_Time((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_Ex_Init_TankCfg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->on_Ex_Init_Syscfg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->on_Ex_Init_Finished(); break;
        case 24: _t->on_Ex_Init_Handler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->on_ScreenSleep((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
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
QT_END_MOC_NAMESPACE
