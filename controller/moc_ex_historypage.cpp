/****************************************************************************
** Meta object code from reading C++ file 'ex_historypage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_historypage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_historypage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_HistoryPage[] = {

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
      16,   15,   15,   15, 0x08,
      55,   15,   15,   15, 0x08,
      94,   15,   15,   15, 0x08,
     127,   15,   15,   15, 0x08,
     161,   15,   15,   15, 0x08,
     195,   15,   15,   15, 0x08,
     235,  229,   15,   15, 0x08,
     280,   15,   15,   15, 0x08,
     321,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Ex_HistoryPage[] = {
    "Ex_HistoryPage\0\0on_HISTORYPAGE_BTN_QRY_STIME_clicked()\0"
    "on_HISTORYPAGE_BTN_QRY_ETIME_clicked()\0"
    "on_HISTORYPAGE_BTN_QRY_clicked()\0"
    "on_HISTORYPAGE_BTN_PREV_clicked()\0"
    "on_HISTORYPAGE_BTN_NEXT_clicked()\0"
    "on_HISTORYPAGE_BTN_SEND_clicked()\0"
    "index\0on_m_QryObjNameCbox_currentIndexChanged(int)\0"
    "on_HISTORYPAGE_TIME_BTN_CANCEL_clicked()\0"
    "on_HISTORYPAGE_TIME_BTN_OK_clicked()\0"
};

void Ex_HistoryPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_HistoryPage *_t = static_cast<Ex_HistoryPage *>(_o);
        switch (_id) {
        case 0: _t->on_HISTORYPAGE_BTN_QRY_STIME_clicked(); break;
        case 1: _t->on_HISTORYPAGE_BTN_QRY_ETIME_clicked(); break;
        case 2: _t->on_HISTORYPAGE_BTN_QRY_clicked(); break;
        case 3: _t->on_HISTORYPAGE_BTN_PREV_clicked(); break;
        case 4: _t->on_HISTORYPAGE_BTN_NEXT_clicked(); break;
        case 5: _t->on_HISTORYPAGE_BTN_SEND_clicked(); break;
        case 6: _t->on_m_QryObjNameCbox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_HISTORYPAGE_TIME_BTN_CANCEL_clicked(); break;
        case 8: _t->on_HISTORYPAGE_TIME_BTN_OK_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_HistoryPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_HistoryPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_Ex_HistoryPage,
      qt_meta_data_Ex_HistoryPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_HistoryPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_HistoryPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_HistoryPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_HistoryPage))
        return static_cast<void*>(const_cast< Ex_HistoryPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int Ex_HistoryPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
