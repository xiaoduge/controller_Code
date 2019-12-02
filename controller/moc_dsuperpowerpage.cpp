/****************************************************************************
** Meta object code from reading C++ file 'dsuperpowerpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dsuperpowerpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dsuperpowerpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DSuperPowerPage[] = {

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
      17,   16,   16,   16, 0x0a,
      43,   37,   16,   16, 0x0a,
      79,   37,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     135,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DSuperPowerPage[] = {
    "DSuperPowerPage\0\0on_btn_clicked(int)\0"
    "index\0on_CmbIndexChange_DefaultState(int)\0"
    "on_CmbIndexChange_deviceType(int)\0"
    "on_btnDbDel_clicked()\0on_btnDelCfg_clicked()\0"
};

void DSuperPowerPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DSuperPowerPage *_t = static_cast<DSuperPowerPage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_CmbIndexChange_DefaultState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_CmbIndexChange_deviceType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_btnDbDel_clicked(); break;
        case 4: _t->on_btnDelCfg_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DSuperPowerPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DSuperPowerPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_DSuperPowerPage,
      qt_meta_data_DSuperPowerPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DSuperPowerPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DSuperPowerPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DSuperPowerPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DSuperPowerPage))
        return static_cast<void*>(const_cast< DSuperPowerPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int DSuperPowerPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
