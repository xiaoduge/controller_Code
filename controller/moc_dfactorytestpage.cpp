/****************************************************************************
** Meta object code from reading C++ file 'dfactorytestpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dfactorytestpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dfactorytestpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DFactoryTestPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      39,   17,   17,   17, 0x08,
      61,   17,   17,   17, 0x08,
      81,   17,   17,   17, 0x08,
     103,   17,   17,   17, 0x08,
     124,   17,   17,   17, 0x08,
     146,   17,   17,   17, 0x08,
     175,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DFactoryTestPage[] = {
    "DFactoryTestPage\0\0on_flowBtn_clicked()\0"
    "on_iLoopBtn_clicked()\0on_keyBtn_clicked()\0"
    "on_writeBtn_clicked()\0on_readBtn_clicked()\0"
    "on_clearBtn_clicked()\0"
    "on_clearWifiMsgBtn_clicked()\0"
    "on_updZigbeeBtn_clicked()\0"
};

void DFactoryTestPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DFactoryTestPage *_t = static_cast<DFactoryTestPage *>(_o);
        switch (_id) {
        case 0: _t->on_flowBtn_clicked(); break;
        case 1: _t->on_iLoopBtn_clicked(); break;
        case 2: _t->on_keyBtn_clicked(); break;
        case 3: _t->on_writeBtn_clicked(); break;
        case 4: _t->on_readBtn_clicked(); break;
        case 5: _t->on_clearBtn_clicked(); break;
        case 6: _t->on_clearWifiMsgBtn_clicked(); break;
        case 7: _t->on_updZigbeeBtn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DFactoryTestPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DFactoryTestPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_DFactoryTestPage,
      qt_meta_data_DFactoryTestPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DFactoryTestPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DFactoryTestPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DFactoryTestPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DFactoryTestPage))
        return static_cast<void*>(const_cast< DFactoryTestPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int DFactoryTestPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
