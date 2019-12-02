/****************************************************************************
** Meta object code from reading C++ file 'dinitnetworkpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dinitnetworkpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dinitnetworkpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DInitNetworkpage[] = {

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
      18,   17,   17,   17, 0x0a,
      44,   38,   17,   17, 0x0a,
      73,   17,   17,   17, 0x0a,
      99,   17,   17,   17, 0x0a,
     125,   17,   17,   17, 0x0a,
     149,   17,   17,   17, 0x0a,
     177,   17,   17,   17, 0x0a,
     202,  197,   17,   17, 0x0a,
     250,   38,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DInitNetworkpage[] = {
    "DInitNetworkpage\0\0on_btn_clicked(int)\0"
    "state\0on_checkBox_changeState(int)\0"
    "on_m_pExNextBtn_clicked()\0"
    "on_m_pExBackBtn_clicked()\0"
    "on_addSSIDBtn_clicked()\0"
    "on_wifiRefreshBtn_clicked()\0"
    "on_refreshWifiMsg()\0item\0"
    "on_wifiListWidget_itemClicked(QListWidgetItem*)\0"
    "on_addCheckBox_stateChanged(int)\0"
};

void DInitNetworkpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DInitNetworkpage *_t = static_cast<DInitNetworkpage *>(_o);
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

const QMetaObjectExtraData DInitNetworkpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DInitNetworkpage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_DInitNetworkpage,
      qt_meta_data_DInitNetworkpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DInitNetworkpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DInitNetworkpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DInitNetworkpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DInitNetworkpage))
        return static_cast<void*>(const_cast< DInitNetworkpage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int DInitNetworkpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
