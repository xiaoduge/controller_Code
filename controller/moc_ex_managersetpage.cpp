/****************************************************************************
** Meta object code from reading C++ file 'ex_managersetpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_managersetpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_managersetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_ManagerSetPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x09,
      44,   18,   18,   18, 0x09,
      69,   18,   18,   18, 0x09,
      94,   18,   18,   18, 0x09,
     117,   18,   18,   18, 0x09,
     144,   18,   18,   18, 0x09,
     169,   18,   18,   18, 0x09,
     199,  193,   18,   18, 0x09,
     239,  233,   18,   18, 0x09,
     268,   18,   18,   18, 0x09,
     297,   18,   18,   18, 0x09,
     330,  323,   18,   18, 0x09,
     363,   18,   18,   18, 0x09,
     387,  233,   18,   18, 0x09,
     424,  193,   18,   18, 0x09,
     461,   18,   18,   18, 0x09,
     475,   18,   18,   18, 0x09,
     506,  233,   18,   18, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Ex_ManagerSetPage[] = {
    "Ex_ManagerSetPage\0\0on_flowSaveBtn_clicked()\0"
    "on_timeDateSet_clicked()\0"
    "on_timeTimeSet_clicked()\0"
    "on_timeOkBtn_clicked()\0"
    "on_timeCancelBtn_clicked()\0"
    "on_caliSaveBtn_clicked()\0"
    "on_lanSaveBtn_clicked()\0index\0"
    "on_cbLan_currentIndexChanged(int)\0"
    "state\0on_checkBox_changeState(int)\0"
    "on_audioBtnSavebtn_clicked()\0"
    "on_unitsSaveBtn_clicked()\0button\0"
    "on_btn_clicked(QAbstractButton*)\0"
    "on_LcdSaveBtn_clicked()\0"
    "on_CheckEnergySave_stateChanged(int)\0"
    "on_comboBox_currentIndexChanged(int)\0"
    "setValue(int)\0on_AdditionalBtnSave_clicked()\0"
    "on_HPCircheckBox_changeState(int)\0"
};

void Ex_ManagerSetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_ManagerSetPage *_t = static_cast<Ex_ManagerSetPage *>(_o);
        switch (_id) {
        case 0: _t->on_flowSaveBtn_clicked(); break;
        case 1: _t->on_timeDateSet_clicked(); break;
        case 2: _t->on_timeTimeSet_clicked(); break;
        case 3: _t->on_timeOkBtn_clicked(); break;
        case 4: _t->on_timeCancelBtn_clicked(); break;
        case 5: _t->on_caliSaveBtn_clicked(); break;
        case 6: _t->on_lanSaveBtn_clicked(); break;
        case 7: _t->on_cbLan_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_checkBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_audioBtnSavebtn_clicked(); break;
        case 10: _t->on_unitsSaveBtn_clicked(); break;
        case 11: _t->on_btn_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 12: _t->on_LcdSaveBtn_clicked(); break;
        case 13: _t->on_CheckEnergySave_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_AdditionalBtnSave_clicked(); break;
        case 17: _t->on_HPCircheckBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_ManagerSetPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_ManagerSetPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_ManagerSetPage,
      qt_meta_data_Ex_ManagerSetPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_ManagerSetPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_ManagerSetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_ManagerSetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_ManagerSetPage))
        return static_cast<void*>(const_cast< Ex_ManagerSetPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_ManagerSetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
