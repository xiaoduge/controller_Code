/****************************************************************************
** Meta object code from reading C++ file 'ex_superpowerpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_superpowerpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_superpowerpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_SuperPowerPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      45,   39,   18,   18, 0x0a,
      81,   18,   18,   18, 0x0a,
      91,   39,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_SuperPowerPage[] = {
    "Ex_SuperPowerPage\0\0on_btn_clicked(int)\0"
    "index\0on_CmbIndexChange_DefaultState(int)\0"
    "Restart()\0on_CmbIndexChange_deviceType(int)\0"
};

void Ex_SuperPowerPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_SuperPowerPage *_t = static_cast<Ex_SuperPowerPage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_CmbIndexChange_DefaultState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->Restart(); break;
        case 3: _t->on_CmbIndexChange_deviceType((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_SuperPowerPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_SuperPowerPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_SuperPowerPage,
      qt_meta_data_Ex_SuperPowerPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_SuperPowerPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_SuperPowerPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_SuperPowerPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_SuperPowerPage))
        return static_cast<void*>(const_cast< Ex_SuperPowerPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_SuperPowerPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
