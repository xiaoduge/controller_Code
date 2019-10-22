/****************************************************************************
** Meta object code from reading C++ file 'ex_permissionsetpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_permissionsetpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_permissionsetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_PermissionSetPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x09,
      47,   21,   21,   21, 0x09,
      84,   78,   21,   21, 0x09,
     118,   21,   21,   21, 0x09,
     140,   21,   21,   21, 0x09,
     167,   21,   21,   21, 0x09,
     193,   21,   21,   21, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Ex_PermissionSetPage[] = {
    "Ex_PermissionSetPage\0\0on_RfidsaveBtn_clicked()\0"
    "on_PermissionsaveBtn_clicked()\0state\0"
    "on_chSubAccount_stateChanged(int)\0"
    "on_QueryBtn_clicked()\0on_DisplayAllBtn_clicked()\0"
    "on_DeleteOneBtn_clicked()\0"
    "on_DeleteAllBtn_clicked()\0"
};

void Ex_PermissionSetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_PermissionSetPage *_t = static_cast<Ex_PermissionSetPage *>(_o);
        switch (_id) {
        case 0: _t->on_RfidsaveBtn_clicked(); break;
        case 1: _t->on_PermissionsaveBtn_clicked(); break;
        case 2: _t->on_chSubAccount_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_QueryBtn_clicked(); break;
        case 4: _t->on_DisplayAllBtn_clicked(); break;
        case 5: _t->on_DeleteOneBtn_clicked(); break;
        case 6: _t->on_DeleteAllBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_PermissionSetPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_PermissionSetPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_PermissionSetPage,
      qt_meta_data_Ex_PermissionSetPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_PermissionSetPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_PermissionSetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_PermissionSetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_PermissionSetPage))
        return static_cast<void*>(const_cast< Ex_PermissionSetPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_PermissionSetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
