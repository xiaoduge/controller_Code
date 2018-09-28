/****************************************************************************
** Meta object code from reading C++ file 'ex_factorytestpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_factorytestpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_factorytestpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_FactoryTestPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      41,   19,   19,   19, 0x08,
      63,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Ex_FactoryTestPage[] = {
    "Ex_FactoryTestPage\0\0on_flowBtn_clicked()\0"
    "on_iLoopBtn_clicked()\0on_keyBtn_clicked()\0"
};

void Ex_FactoryTestPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_FactoryTestPage *_t = static_cast<Ex_FactoryTestPage *>(_o);
        switch (_id) {
        case 0: _t->on_flowBtn_clicked(); break;
        case 1: _t->on_iLoopBtn_clicked(); break;
        case 2: _t->on_keyBtn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Ex_FactoryTestPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_FactoryTestPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_FactoryTestPage,
      qt_meta_data_Ex_FactoryTestPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_FactoryTestPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_FactoryTestPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_FactoryTestPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_FactoryTestPage))
        return static_cast<void*>(const_cast< Ex_FactoryTestPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_FactoryTestPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
