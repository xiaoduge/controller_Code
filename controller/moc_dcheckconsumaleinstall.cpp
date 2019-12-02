/****************************************************************************
** Meta object code from reading C++ file 'dcheckconsumaleinstall.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dcheckconsumaleinstall.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dcheckconsumaleinstall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DCheckConsumaleInstall[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   24,   23,   23, 0x05,
      77,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     108,   23,   23,   23, 0x0a,
     136,  129,   23,   23, 0x0a,
     162,  156,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DCheckConsumaleInstall[] = {
    "DCheckConsumaleInstall\0\0iType,catNo,lotNo\0"
    "consumableMsg(int,QString,QString)\0"
    "consumableInstallFinished(int)\0"
    "updateConsumaleMsg()\0isBusy\0"
    "setBusystatus(bool)\0iType\0"
    "updateConsumableType(int)\0"
};

void DCheckConsumaleInstall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DCheckConsumaleInstall *_t = static_cast<DCheckConsumaleInstall *>(_o);
        switch (_id) {
        case 0: _t->consumableMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->consumableInstallFinished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateConsumaleMsg(); break;
        case 3: _t->setBusystatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->updateConsumableType((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DCheckConsumaleInstall::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DCheckConsumaleInstall::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DCheckConsumaleInstall,
      qt_meta_data_DCheckConsumaleInstall, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DCheckConsumaleInstall::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DCheckConsumaleInstall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DCheckConsumaleInstall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DCheckConsumaleInstall))
        return static_cast<void*>(const_cast< DCheckConsumaleInstall*>(this));
    return QObject::qt_metacast(_clname);
}

int DCheckConsumaleInstall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DCheckConsumaleInstall::consumableMsg(int _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DCheckConsumaleInstall::consumableInstallFinished(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
