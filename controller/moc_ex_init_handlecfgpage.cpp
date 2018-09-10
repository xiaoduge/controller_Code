/****************************************************************************
** Meta object code from reading C++ file 'ex_init_handlecfgpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_init_handlecfgpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_init_handlecfgpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_Init_HandleCfgpage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      40,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   22,   22,   22, 0x0a,
      92,   22,   22,   22, 0x0a,
     121,   22,   22,   22, 0x0a,
     148,   22,   22,   22, 0x0a,
     177,   22,   22,   22, 0x0a,
     204,   22,   22,   22, 0x0a,
     238,  232,   22,   22, 0x0a,
     267,   22,   22,   22, 0x0a,
     303,  297,   22,   22, 0x0a,
     335,   22,   22,   22, 0x0a,
     361,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_Init_HandleCfgpage[] = {
    "Ex_Init_HandleCfgpage\0\0exInitFinished()\0"
    "handlercfgSwitchBtnClicked(int)\0"
    "on_btn_clicked(int)\0on_pushButton_QueryHandler()\0"
    "on_pushButton_CfgHandler()\0"
    "on_pushButton_ResetHandler()\0"
    "on_pushButton_RmvHandler()\0"
    "on_pushButton_SaveHandler()\0state\0"
    "on_checkBox_changeState(int)\0"
    "on_pushButton_DeleteHandler()\0index\0"
    "on_CmbIndexChange_trx_type(int)\0"
    "on_pushButton_FinishBtn()\0"
    "on_pushButton_BackBtn()\0"
};

void Ex_Init_HandleCfgpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_Init_HandleCfgpage *_t = static_cast<Ex_Init_HandleCfgpage *>(_o);
        switch (_id) {
        case 0: _t->exInitFinished(); break;
        case 1: _t->handlercfgSwitchBtnClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_QueryHandler(); break;
        case 4: _t->on_pushButton_CfgHandler(); break;
        case 5: _t->on_pushButton_ResetHandler(); break;
        case 6: _t->on_pushButton_RmvHandler(); break;
        case 7: _t->on_pushButton_SaveHandler(); break;
        case 8: _t->on_checkBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_pushButton_DeleteHandler(); break;
        case 10: _t->on_CmbIndexChange_trx_type((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_pushButton_FinishBtn(); break;
        case 12: _t->on_pushButton_BackBtn(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_Init_HandleCfgpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_Init_HandleCfgpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_Init_HandleCfgpage,
      qt_meta_data_Ex_Init_HandleCfgpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_Init_HandleCfgpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_Init_HandleCfgpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_Init_HandleCfgpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_Init_HandleCfgpage))
        return static_cast<void*>(const_cast< Ex_Init_HandleCfgpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_Init_HandleCfgpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Ex_Init_HandleCfgpage::exInitFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Ex_Init_HandleCfgpage::handlercfgSwitchBtnClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
