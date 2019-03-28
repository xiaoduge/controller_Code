/****************************************************************************
** Meta object code from reading C++ file 'EngDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EngDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EngDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EngineerDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      35,   20,   12,   12, 0x08,
      73,   64,   12,   12, 0x08,
     101,   98,   12,   12, 0x08,
     126,  120,   12,   12, 0x08,
     158,  156,  151,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EngineerDlg[] = {
    "EngineerDlg\0\0Exit()\0bState,iCtrlId\0"
    "onSwitchBtnChanged(bool,int)\0id,value\0"
    "onSliderChanged(int,int)\0id\0"
    "rfBtnsClicked(int)\0index\0"
    "RfId_CmbIndexChange(int)\0bool\0,\0"
    "eventFilter(QObject*,QEvent*)\0"
};

void EngineerDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EngineerDlg *_t = static_cast<EngineerDlg *>(_o);
        switch (_id) {
        case 0: _t->Exit(); break;
        case 1: _t->onSwitchBtnChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onSliderChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->rfBtnsClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->RfId_CmbIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EngineerDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EngineerDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EngineerDlg,
      qt_meta_data_EngineerDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EngineerDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EngineerDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EngineerDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EngineerDlg))
        return static_cast<void*>(const_cast< EngineerDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int EngineerDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
