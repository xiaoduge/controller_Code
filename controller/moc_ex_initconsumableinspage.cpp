/****************************************************************************
** Meta object code from reading C++ file 'ex_initconsumableinspage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_initconsumableinspage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_initconsumableinspage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_InitConsumableInsPage[] = {

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
      31,   26,   25,   25, 0x0a,
      60,   25,   25,   25, 0x08,
      83,   25,   25,   25, 0x08,
     106,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Ex_InitConsumableInsPage[] = {
    "Ex_InitConsumableInsPage\0\0type\0"
    "updateConsumableInstall(int)\0"
    "on_ExNextBtn_clicked()\0on_ExBackBtn_clicked()\0"
    "on_timer_event()\0"
};

void Ex_InitConsumableInsPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_InitConsumableInsPage *_t = static_cast<Ex_InitConsumableInsPage *>(_o);
        switch (_id) {
        case 0: _t->updateConsumableInstall((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_ExNextBtn_clicked(); break;
        case 2: _t->on_ExBackBtn_clicked(); break;
        case 3: _t->on_timer_event(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_InitConsumableInsPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_InitConsumableInsPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_InitConsumableInsPage,
      qt_meta_data_Ex_InitConsumableInsPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_InitConsumableInsPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_InitConsumableInsPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_InitConsumableInsPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_InitConsumableInsPage))
        return static_cast<void*>(const_cast< Ex_InitConsumableInsPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_InitConsumableInsPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
