/****************************************************************************
** Meta object code from reading C++ file 'dinithandlecfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dinithandlecfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dinithandlecfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DInitHandleCfgpage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      40,   19,   19,   19, 0x0a,
      69,   19,   19,   19, 0x0a,
      96,   19,   19,   19, 0x0a,
     125,   19,   19,   19, 0x0a,
     152,   19,   19,   19, 0x0a,
     186,  180,   19,   19, 0x0a,
     215,   19,   19,   19, 0x0a,
     251,  245,   19,   19, 0x0a,
     283,   19,   19,   19, 0x0a,
     309,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DInitHandleCfgpage[] = {
    "DInitHandleCfgpage\0\0on_btn_clicked(int)\0"
    "on_pushButton_QueryHandler()\0"
    "on_pushButton_CfgHandler()\0"
    "on_pushButton_ResetHandler()\0"
    "on_pushButton_RmvHandler()\0"
    "on_pushButton_SaveHandler()\0state\0"
    "on_checkBox_changeState(int)\0"
    "on_pushButton_DeleteHandler()\0index\0"
    "on_CmbIndexChange_trx_type(int)\0"
    "on_pushButton_FinishBtn()\0"
    "on_pushButton_BackBtn()\0"
};

void DInitHandleCfgpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DInitHandleCfgpage *_t = static_cast<DInitHandleCfgpage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_QueryHandler(); break;
        case 2: _t->on_pushButton_CfgHandler(); break;
        case 3: _t->on_pushButton_ResetHandler(); break;
        case 4: _t->on_pushButton_RmvHandler(); break;
        case 5: _t->on_pushButton_SaveHandler(); break;
        case 6: _t->on_checkBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_pushButton_DeleteHandler(); break;
        case 8: _t->on_CmbIndexChange_trx_type((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_pushButton_FinishBtn(); break;
        case 10: _t->on_pushButton_BackBtn(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DInitHandleCfgpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DInitHandleCfgpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_DInitHandleCfgpage,
      qt_meta_data_DInitHandleCfgpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DInitHandleCfgpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DInitHandleCfgpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DInitHandleCfgpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DInitHandleCfgpage))
        return static_cast<void*>(const_cast< DInitHandleCfgpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int DInitHandleCfgpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
