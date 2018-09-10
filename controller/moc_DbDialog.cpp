/****************************************************************************
** Meta object code from reading C++ file 'DbDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DbDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DbDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DbDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,    7,    6,    6, 0x08,
      25,    6,    6,    6, 0x28,
      35,    6,    6,    6, 0x08,
      44,    6,    6,    6, 0x08,
      51,    6,    6,    6, 0x08,
      61,    6,    6,    6, 0x08,
      71,    6,    6,    6, 0x08,
      80,    6,    6,    6, 0x08,
      88,    6,    6,    6, 0x08,
      97,    6,    6,    6, 0x08,
     104,    6,    6,    6, 0x08,
     114,    6,    6,    6, 0x08,
     124,    6,    6,    6, 0x08,
     133,    6,    6,    6, 0x08,
     143,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DbDlg[] = {
    "DbDlg\0\0iIdx\0QueryDb(int)\0QueryDb()\0"
    "QStart()\0QEnd()\0QStaCal()\0QEndCal()\0"
    "QQuery()\0DelDb()\0DStart()\0DEnd()\0"
    "DStaCal()\0DEndCal()\0DQuery()\0DAction()\0"
    "Exit()\0"
};

void DbDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DbDlg *_t = static_cast<DbDlg *>(_o);
        switch (_id) {
        case 0: _t->QueryDb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->QueryDb(); break;
        case 2: _t->QStart(); break;
        case 3: _t->QEnd(); break;
        case 4: _t->QStaCal(); break;
        case 5: _t->QEndCal(); break;
        case 6: _t->QQuery(); break;
        case 7: _t->DelDb(); break;
        case 8: _t->DStart(); break;
        case 9: _t->DEnd(); break;
        case 10: _t->DStaCal(); break;
        case 11: _t->DEndCal(); break;
        case 12: _t->DQuery(); break;
        case 13: _t->DAction(); break;
        case 14: _t->Exit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DbDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DbDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DbDlg,
      qt_meta_data_DbDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DbDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DbDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DbDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DbDlg))
        return static_cast<void*>(const_cast< DbDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int DbDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
