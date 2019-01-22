/****************************************************************************
** Meta object code from reading C++ file 'ex_checkconsumaleinstall.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_checkconsumaleinstall.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_checkconsumaleinstall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_CheckConsumaleInstall[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   26,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   25,   25,   25, 0x0a,
     107,  100,   25,   25, 0x0a,
     133,  127,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_CheckConsumaleInstall[] = {
    "Ex_CheckConsumaleInstall\0\0iType,catNo,lotNo\0"
    "consumableMsg(int,QString,QString)\0"
    "updateConsumaleMsg()\0isBusy\0"
    "setBusystatus(bool)\0iType\0"
    "updateConsumableType(int)\0"
};

void Ex_CheckConsumaleInstall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_CheckConsumaleInstall *_t = static_cast<Ex_CheckConsumaleInstall *>(_o);
        switch (_id) {
        case 0: _t->consumableMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->updateConsumaleMsg(); break;
        case 2: _t->setBusystatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->updateConsumableType((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_CheckConsumaleInstall::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_CheckConsumaleInstall::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Ex_CheckConsumaleInstall,
      qt_meta_data_Ex_CheckConsumaleInstall, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_CheckConsumaleInstall::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_CheckConsumaleInstall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_CheckConsumaleInstall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_CheckConsumaleInstall))
        return static_cast<void*>(const_cast< Ex_CheckConsumaleInstall*>(this));
    return QObject::qt_metacast(_clname);
}

int Ex_CheckConsumaleInstall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Ex_CheckConsumaleInstall::consumableMsg(int _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
