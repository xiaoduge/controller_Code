/****************************************************************************
** Meta object code from reading C++ file 'usermanage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usermanage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserManageDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x0a,
      41,   15,   14,   14, 0x0a,
      61,   15,   14,   14, 0x0a,
      82,   15,   14,   14, 0x0a,
     105,   14,   14,   14, 0x0a,
     126,   14,   14,   14, 0x0a,
     147,   14,   14,   14, 0x0a,
     178,  176,  171,   14, 0x0a,
     212,  208,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UserManageDlg[] = {
    "UserManageDlg\0\0state\0ROPBOX_Checked(int)\0"
    "PHPBOX_Checked(int)\0DelPBOX_Checked(int)\0"
    "AdminPBOX_Checked(int)\0on_pushButton_AddU()\0"
    "on_pushButton_DelU()\0on_pushButton_CancelU()\0"
    "bool\0,\0eventFilter(QObject*,QEvent*)\0"
    "str\0write_back_str(QString)\0"
};

void UserManageDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UserManageDlg *_t = static_cast<UserManageDlg *>(_o);
        switch (_id) {
        case 0: _t->ROPBOX_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->PHPBOX_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->DelPBOX_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->AdminPBOX_Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_pushButton_AddU(); break;
        case 5: _t->on_pushButton_DelU(); break;
        case 6: _t->on_pushButton_CancelU(); break;
        case 7: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->write_back_str((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UserManageDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UserManageDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UserManageDlg,
      qt_meta_data_UserManageDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UserManageDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UserManageDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UserManageDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserManageDlg))
        return static_cast<void*>(const_cast< UserManageDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int UserManageDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
