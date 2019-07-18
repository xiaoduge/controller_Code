/****************************************************************************
** Meta object code from reading C++ file 'servicepage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "servicepage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'servicepage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServicePage[] = {

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
      17,   13,   12,   12, 0x0a,
      37,   13,   12,   12, 0x0a,
      58,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ServicePage[] = {
    "ServicePage\0\0tmp\0on_btn_clicked(int)\0"
    "on_navi_clicked(int)\0on_logoutBtn_clicked()\0"
};

void ServicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ServicePage *_t = static_cast<ServicePage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_navi_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_logoutBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ServicePage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ServicePage::staticMetaObject = {
    { &CPage::staticMetaObject, qt_meta_stringdata_ServicePage,
      qt_meta_data_ServicePage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServicePage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServicePage))
        return static_cast<void*>(const_cast< ServicePage*>(this));
    return CPage::qt_metacast(_clname);
}

int ServicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPage::qt_metacall(_c, _id, _a);
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
