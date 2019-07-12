/****************************************************************************
** Meta object code from reading C++ file 'ex_init_tankcfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_init_tankcfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_init_tankcfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_Init_Tankcfgpage[] = {

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
      27,   21,   20,   20, 0x0a,
      53,   21,   20,   20, 0x0a,
      79,   20,   20,   20, 0x0a,
     102,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_Init_Tankcfgpage[] = {
    "Ex_Init_Tankcfgpage\0\0index\0"
    "on_CmbIndexChange_pw(int)\0"
    "on_CmbIndexChange_sw(int)\0"
    "on_ExNextBtn_clicked()\0on_ExBackBtn_clicked()\0"
};

void Ex_Init_Tankcfgpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_Init_Tankcfgpage *_t = static_cast<Ex_Init_Tankcfgpage *>(_o);
        switch (_id) {
        case 0: _t->on_CmbIndexChange_pw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_CmbIndexChange_sw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_ExNextBtn_clicked(); break;
        case 3: _t->on_ExBackBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_Init_Tankcfgpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_Init_Tankcfgpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_Init_Tankcfgpage,
      qt_meta_data_Ex_Init_Tankcfgpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_Init_Tankcfgpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_Init_Tankcfgpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_Init_Tankcfgpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_Init_Tankcfgpage))
        return static_cast<void*>(const_cast< Ex_Init_Tankcfgpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_Init_Tankcfgpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
