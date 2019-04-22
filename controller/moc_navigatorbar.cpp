/****************************************************************************
** Meta object code from reading C++ file 'navigatorbar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "navigatorbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'navigatorbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_navigatorBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      24,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   37,   13,   13, 0x0a,
      60,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_navigatorBar[] = {
    "navigatorBar\0\0clicked()\0clicked(int)\0"
    "id\0on_btn_clicked(int)\0on_timerEvent()\0"
};

void navigatorBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        navigatorBar *_t = static_cast<navigatorBar *>(_o);
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_timerEvent(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData navigatorBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject navigatorBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_navigatorBar,
      qt_meta_data_navigatorBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &navigatorBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *navigatorBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *navigatorBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_navigatorBar))
        return static_cast<void*>(const_cast< navigatorBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int navigatorBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void navigatorBar::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void navigatorBar::clicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
