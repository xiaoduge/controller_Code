/****************************************************************************
** Meta object code from reading C++ file 'PermissionDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PermissionDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PermissionDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PermissionDlg[] = {

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
      22,   20,   15,   14, 0x0a,
      52,   14,   14,   14, 0x08,
      76,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PermissionDlg[] = {
    "PermissionDlg\0\0bool\0,\0"
    "eventFilter(QObject*,QEvent*)\0"
    "on_pushButton_Confirm()\0on_pushButton_Cancel()\0"
};

void PermissionDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PermissionDlg *_t = static_cast<PermissionDlg *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->on_pushButton_Confirm(); break;
        case 2: _t->on_pushButton_Cancel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PermissionDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PermissionDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PermissionDlg,
      qt_meta_data_PermissionDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PermissionDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PermissionDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PermissionDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PermissionDlg))
        return static_cast<void*>(const_cast< PermissionDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int PermissionDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
