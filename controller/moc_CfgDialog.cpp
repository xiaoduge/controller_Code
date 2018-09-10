/****************************************************************************
** Meta object code from reading C++ file 'CfgDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CfgDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CfgDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CfgDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      17,    7,    7,    7, 0x08,
      27,    7,    7,    7, 0x08,
      41,   39,   34,    7, 0x0a,
      77,   71,    7,    7, 0x0a,
      95,   71,    7,    7, 0x0a,
     120,  114,    7,    7, 0x0a,
     147,  114,    7,    7, 0x0a,
     174,  114,    7,    7, 0x0a,
     210,  201,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CfgDlg[] = {
    "CfgDlg\0\0CFGCom()\0SaveCfg()\0Exit()\0"
    "bool\0,\0eventFilter(QObject*,QEvent*)\0"
    "state\0Have_Checked(int)\0Alarm_Checked(int)\0"
    "index\0PM_PM1_CmbIndexChange(int)\0"
    "PM_PM2_CmbIndexChange(int)\0"
    "PM_PM3_CmbIndexChange(int)\0gemfield\0"
    "SM_btnGroupClicked(int)\0"
};

void CfgDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CfgDlg *_t = static_cast<CfgDlg *>(_o);
        switch (_id) {
        case 0: _t->CFGCom(); break;
        case 1: _t->SaveCfg(); break;
        case 2: _t->Exit(); break;
        case 3: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->Have_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->Alarm_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->PM_PM1_CmbIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->PM_PM2_CmbIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->PM_PM3_CmbIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->SM_btnGroupClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CfgDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CfgDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CfgDlg,
      qt_meta_data_CfgDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CfgDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CfgDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CfgDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CfgDlg))
        return static_cast<void*>(const_cast< CfgDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CfgDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
