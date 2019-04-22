/****************************************************************************
** Meta object code from reading C++ file 'CMDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CMDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CMDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMDlg[] = {

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
       7,    6,    6,    6, 0x08,
      16,    6,    6,    6, 0x08,
      26,    6,    6,    6, 0x08,
      40,   38,   33,    6, 0x0a,
      74,   70,    6,    6, 0x0a,
     102,   90,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CMDlg[] = {
    "CMDlg\0\0CFGCom()\0SaveCfg()\0Exit()\0bool\0"
    ",\0eventFilter(QObject*,QEvent*)\0iId\0"
    "resetClick(int)\0iId,strName\0"
    "resetItem(int,QString&)\0"
};

void CMDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMDlg *_t = static_cast<CMDlg *>(_o);
        switch (_id) {
        case 0: _t->CFGCom(); break;
        case 1: _t->SaveCfg(); break;
        case 2: _t->Exit(); break;
        case 3: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->resetClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->resetItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CMDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CMDlg,
      qt_meta_data_CMDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMDlg))
        return static_cast<void*>(const_cast< CMDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CMDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
