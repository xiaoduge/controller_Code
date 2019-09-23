/****************************************************************************
** Meta object code from reading C++ file 'ex_init_networkpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_init_networkpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_init_networkpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_Init_Networkpage[] = {

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
      21,   20,   20,   20, 0x0a,
      47,   41,   20,   20, 0x0a,
      76,   20,   20,   20, 0x0a,
     102,   20,   20,   20, 0x0a,
     128,   20,   20,   20, 0x0a,
     152,   20,   20,   20, 0x0a,
     180,   20,   20,   20, 0x0a,
     205,  200,   20,   20, 0x0a,
     253,   41,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_Init_Networkpage[] = {
    "Ex_Init_Networkpage\0\0on_btn_clicked(int)\0"
    "state\0on_checkBox_changeState(int)\0"
    "on_m_pExNextBtn_clicked()\0"
    "on_m_pExBackBtn_clicked()\0"
    "on_addSSIDBtn_clicked()\0"
    "on_wifiRefreshBtn_clicked()\0"
    "on_refreshWifiMsg()\0item\0"
    "on_wifiListWidget_itemClicked(QListWidgetItem*)\0"
    "on_addCheckBox_stateChanged(int)\0"
};

void Ex_Init_Networkpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_Init_Networkpage *_t = static_cast<Ex_Init_Networkpage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_checkBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_m_pExNextBtn_clicked(); break;
        case 3: _t->on_m_pExBackBtn_clicked(); break;
        case 4: _t->on_addSSIDBtn_clicked(); break;
        case 5: _t->on_wifiRefreshBtn_clicked(); break;
        case 6: _t->on_refreshWifiMsg(); break;
        case 7: _t->on_wifiListWidget_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->on_addCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_Init_Networkpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_Init_Networkpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_Init_Networkpage,
      qt_meta_data_Ex_Init_Networkpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_Init_Networkpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_Init_Networkpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_Init_Networkpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_Init_Networkpage))
        return static_cast<void*>(const_cast< Ex_Init_Networkpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_Init_Networkpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
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
