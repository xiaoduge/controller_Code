/****************************************************************************
** Meta object code from reading C++ file 'alarmsetpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "alarmsetpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alarmsetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AlarmSetPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x0a,
      49,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AlarmSetPage[] = {
    "AlarmSetPage\0\0item\0ItemClicked(QListWidgetItem*)\0"
    "on_btn_clicked(int)\0"
};

void AlarmSetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AlarmSetPage *_t = static_cast<AlarmSetPage *>(_o);
        switch (_id) {
        case 0: _t->ItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AlarmSetPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AlarmSetPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_AlarmSetPage,
      qt_meta_data_AlarmSetPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AlarmSetPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AlarmSetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AlarmSetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AlarmSetPage))
        return static_cast<void*>(const_cast< AlarmSetPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int AlarmSetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
