/****************************************************************************
** Meta object code from reading C++ file 'dloginwarningdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dloginwarningdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dloginwarningdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DLoginWarningDialog[] = {

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
      21,   20,   20,   20, 0x09,
      43,   20,   20,   20, 0x09,
      66,   60,   20,   20, 0x09,
      96,   60,   20,   20, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DLoginWarningDialog[] = {
    "DLoginWarningDialog\0\0on_closeBtn_clicked()\0"
    "on_timer_event()\0event\0"
    "mousePressEvent(QMouseEvent*)\0"
    "mouseMoveEvent(QMouseEvent*)\0"
};

void DLoginWarningDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DLoginWarningDialog *_t = static_cast<DLoginWarningDialog *>(_o);
        switch (_id) {
        case 0: _t->on_closeBtn_clicked(); break;
        case 1: _t->on_timer_event(); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DLoginWarningDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DLoginWarningDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DLoginWarningDialog,
      qt_meta_data_DLoginWarningDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DLoginWarningDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DLoginWarningDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DLoginWarningDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DLoginWarningDialog))
        return static_cast<void*>(const_cast< DLoginWarningDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int DLoginWarningDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
