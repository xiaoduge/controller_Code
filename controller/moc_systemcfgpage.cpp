/****************************************************************************
** Meta object code from reading C++ file 'systemcfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "systemcfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemcfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SystemCfgPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x09,
      57,   14,   14,   14, 0x0a,
      77,   15,   14,   14, 0x0a,
     103,   15,   14,   14, 0x0a,
     129,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SystemCfgPage[] = {
    "SystemCfgPage\0\0index\0"
    "on_CmbIndexChange_DefaultState(int)\0"
    "on_btn_clicked(int)\0on_CmbIndexChange_pw(int)\0"
    "on_CmbIndexChange_sw(int)\0Restart()\0"
};

void SystemCfgPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SystemCfgPage *_t = static_cast<SystemCfgPage *>(_o);
        switch (_id) {
        case 0: _t->on_CmbIndexChange_DefaultState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_CmbIndexChange_pw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_CmbIndexChange_sw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Restart(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SystemCfgPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SystemCfgPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_SystemCfgPage,
      qt_meta_data_SystemCfgPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SystemCfgPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SystemCfgPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SystemCfgPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SystemCfgPage))
        return static_cast<void*>(const_cast< SystemCfgPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int SystemCfgPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
