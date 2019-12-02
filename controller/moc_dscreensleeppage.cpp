/****************************************************************************
** Meta object code from reading C++ file 'dscreensleeppage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dscreensleeppage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dscreensleeppage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DScreenSleepPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   17,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DScreenSleepPage[] = {
    "DScreenSleepPage\0\0pageHide()\0"
    "on_SleepPageShow(bool)\0"
};

void DScreenSleepPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DScreenSleepPage *_t = static_cast<DScreenSleepPage *>(_o);
        switch (_id) {
        case 0: _t->pageHide(); break;
        case 1: _t->on_SleepPageShow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DScreenSleepPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DScreenSleepPage::staticMetaObject = {
    { &CPage::staticMetaObject, qt_meta_stringdata_DScreenSleepPage,
      qt_meta_data_DScreenSleepPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DScreenSleepPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DScreenSleepPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DScreenSleepPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DScreenSleepPage))
        return static_cast<void*>(const_cast< DScreenSleepPage*>(this));
    return CPage::qt_metacast(_clname);
}

int DScreenSleepPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DScreenSleepPage::pageHide()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
