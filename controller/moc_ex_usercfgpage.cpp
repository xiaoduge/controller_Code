/****************************************************************************
** Meta object code from reading C++ file 'ex_usercfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_usercfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_usercfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_UserCfgPage[] = {

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
      16,   15,   15,   15, 0x09,
      38,   15,   15,   15, 0x09,
      65,   15,   15,   15, 0x09,
      88,   15,   15,   15, 0x09,
     111,   15,   15,   15, 0x09,
     131,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Ex_UserCfgPage[] = {
    "Ex_UserCfgPage\0\0on_QueryBtn_clicked()\0"
    "on_DisplayAllBtn_Clicked()\0"
    "on_SubmitBtn_clicked()\0on_RevertBtn_clicked()\0"
    "on_AddBtn_clicked()\0on_DeleteRowBtn_clicked()\0"
};

void Ex_UserCfgPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_UserCfgPage *_t = static_cast<Ex_UserCfgPage *>(_o);
        switch (_id) {
        case 0: _t->on_QueryBtn_clicked(); break;
        case 1: _t->on_DisplayAllBtn_Clicked(); break;
        case 2: _t->on_SubmitBtn_clicked(); break;
        case 3: _t->on_RevertBtn_clicked(); break;
        case 4: _t->on_AddBtn_clicked(); break;
        case 5: _t->on_DeleteRowBtn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Ex_UserCfgPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_UserCfgPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_UserCfgPage,
      qt_meta_data_Ex_UserCfgPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_UserCfgPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_UserCfgPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_UserCfgPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_UserCfgPage))
        return static_cast<void*>(const_cast< Ex_UserCfgPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_UserCfgPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
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
