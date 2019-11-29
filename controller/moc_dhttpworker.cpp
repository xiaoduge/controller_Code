/****************************************************************************
** Meta object code from reading C++ file 'dhttpworker.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dhttpworker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dhttpworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DHttpWorker[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   34,   12,   12, 0x0a,
      83,   12,   12,   12, 0x0a,
     116,   12,   12,   12, 0x0a,
     161,  153,   12,   12, 0x0a,
     190,   12,   12,   12, 0x0a,
     209,  153,   12,   12, 0x0a,
     241,   12,   12,   12, 0x0a,
     255,   12,   12,   12, 0x08,
     278,   12,   12,   12, 0x08,
     301,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DHttpWorker[] = {
    "DHttpWorker\0\0feedback(QByteArray)\0"
    "alarmInfo\0on_updateAlarmList(DNetworkAlaramInfo)\0"
    "on_updateHeartList(DNetworkData)\0"
    "on_updateDispenseList(DDispenseData)\0"
    "xmlByte\0on_heartHttpPost(QByteArray)\0"
    "on_alarmHttpPost()\0on_dispenseHttpPost(QByteArray)\0"
    "on_initHttp()\0onHeartReplyFinished()\0"
    "onAlarmReplyFinished()\0onDispenseReplyFinished()\0"
};

void DHttpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DHttpWorker *_t = static_cast<DHttpWorker *>(_o);
        switch (_id) {
        case 0: _t->feedback((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->on_updateAlarmList((*reinterpret_cast< const DNetworkAlaramInfo(*)>(_a[1]))); break;
        case 2: _t->on_updateHeartList((*reinterpret_cast< const DNetworkData(*)>(_a[1]))); break;
        case 3: _t->on_updateDispenseList((*reinterpret_cast< const DDispenseData(*)>(_a[1]))); break;
        case 4: _t->on_heartHttpPost((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->on_alarmHttpPost(); break;
        case 6: _t->on_dispenseHttpPost((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->on_initHttp(); break;
        case 8: _t->onHeartReplyFinished(); break;
        case 9: _t->onAlarmReplyFinished(); break;
        case 10: _t->onDispenseReplyFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DHttpWorker::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DHttpWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DHttpWorker,
      qt_meta_data_DHttpWorker, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DHttpWorker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DHttpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DHttpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DHttpWorker))
        return static_cast<void*>(const_cast< DHttpWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int DHttpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DHttpWorker::feedback(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
