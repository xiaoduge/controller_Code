/****************************************************************************
** Meta object code from reading C++ file 'ex_init_syscfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_init_syscfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_init_syscfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_Init_Syscfgpage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   19,   19,   19, 0x0a,
      72,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_Init_Syscfgpage[] = {
    "Ex_Init_Syscfgpage\0\0initcfgSwitchBtnClicked(int)\0"
    "on_ExNextBtn_clicked()\0on_ExBackBtn_clicked()\0"
};

void Ex_Init_Syscfgpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_Init_Syscfgpage *_t = static_cast<Ex_Init_Syscfgpage *>(_o);
        switch (_id) {
        case 0: _t->initcfgSwitchBtnClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_ExNextBtn_clicked(); break;
        case 2: _t->on_ExBackBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_Init_Syscfgpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_Init_Syscfgpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_Init_Syscfgpage,
      qt_meta_data_Ex_Init_Syscfgpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_Init_Syscfgpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_Init_Syscfgpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_Init_Syscfgpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_Init_Syscfgpage))
        return static_cast<void*>(const_cast< Ex_Init_Syscfgpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_Init_Syscfgpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Ex_Init_Syscfgpage::initcfgSwitchBtnClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
