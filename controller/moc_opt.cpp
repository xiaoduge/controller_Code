/****************************************************************************
** Meta object code from reading C++ file 'opt.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "opt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'opt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_COptDlg[] = {

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
       9,    8,    8,    8, 0x0a,
      31,    8,    8,    8, 0x0a,
      53,    8,    8,    8, 0x0a,
      73,    8,    8,    8, 0x0a,
      92,    8,    8,    8, 0x0a,
     111,    8,    8,    8, 0x0a,
     132,    8,    8,    8, 0x0a,
     153,    8,    8,    8, 0x0a,
     172,    8,    8,    8, 0x0a,
     194,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_COptDlg[] = {
    "COptDlg\0\0on_pushButton_UserM()\0"
    "on_pushButton_HInfQ()\0on_pushButton_CFG()\0"
    "on_pushButton_RO()\0on_pushButton_RH()\0"
    "on_pushButton_RepS()\0on_pushButton_Exit()\0"
    "on_pushButton_CM()\0on_pushButton_Reset()\0"
    "on_pushButton_Engineer()\0"
};

void COptDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        COptDlg *_t = static_cast<COptDlg *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_UserM(); break;
        case 1: _t->on_pushButton_HInfQ(); break;
        case 2: _t->on_pushButton_CFG(); break;
        case 3: _t->on_pushButton_RO(); break;
        case 4: _t->on_pushButton_RH(); break;
        case 5: _t->on_pushButton_RepS(); break;
        case 6: _t->on_pushButton_Exit(); break;
        case 7: _t->on_pushButton_CM(); break;
        case 8: _t->on_pushButton_Reset(); break;
        case 9: _t->on_pushButton_Engineer(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData COptDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject COptDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_COptDlg,
      qt_meta_data_COptDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &COptDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *COptDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *COptDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COptDlg))
        return static_cast<void*>(const_cast< COptDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int COptDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
