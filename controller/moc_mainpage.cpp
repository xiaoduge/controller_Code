/****************************************************************************
** Meta object code from reading C++ file 'mainpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainPage[] = {

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
      14,   10,    9,    9, 0x0a,
      34,   10,    9,    9, 0x0a,
      61,   55,    9,    9, 0x0a,
      82,    9,    9,    9, 0x0a,
      98,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainPage[] = {
    "MainPage\0\0tmp\0on_btn_clicked(int)\0"
    "on_navi_clicked(int)\0index\0"
    "on_btn_clicking(int)\0on_timerEvent()\0"
    "on_logoutBtn_clicked()\0"
};

void MainPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainPage *_t = static_cast<MainPage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_navi_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_btn_clicking((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_timerEvent(); break;
        case 4: _t->on_logoutBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainPage::staticMetaObject = {
    { &CPage::staticMetaObject, qt_meta_stringdata_MainPage,
      qt_meta_data_MainPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainPage))
        return static_cast<void*>(const_cast< MainPage*>(this));
    return CPage::qt_metacast(_clname);
}

int MainPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPage::qt_metacall(_c, _id, _a);
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
